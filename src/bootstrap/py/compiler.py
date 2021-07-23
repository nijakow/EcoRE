import enum


class Bytecodes(enum.IntEnum):
    NOOP    = 0x00
    SELF    = 0x01
    CONST   = 0x02
    PUSH    = 0x03
    POP     = 0x04
    DROP    = 0x05
    DUP     = 0x06
    R2R     = 0x07
    R2L     = 0x08
    L2R     = 0x09
    SEND    = 0x0a
    ASSIGN  = 0x0b
    RETURN  = 0x0c
    CLOSURE = 0x0d


class CompilationException(Exception):

    def __init__(self, ast, message):
        super().__init__(message)
        self._ast = ast


class StorageLocation:

    def is_register(self):
        return False
    
    def is_return_storage_slot(self):
        return False

    def free(self):
        pass

    def __init__(self):
        pass

class Register(StorageLocation):

    def is_register(self):
        return True

    def get_register_number(self):
        return self._reg_num

    def get_depth(self):
        return self._depth
    
    def is_lexical(self):
        return self.get_depth() != 0

    def free(self):
        self._allocator.free(self)

    def __init__(self, allocator, reg_num, depth=0):
        super().__init__()
        self._allocator = allocator
        self._reg_num = reg_num
        self._depth = depth

class Registers:

    def get(self, r):
        return self._registers[r]
    
    def free(self):
        for r in self._registers:
            r.free()

    def __init__(self, registers):
        self._registers = registers



class RegisterAllocator:

    def _allocate_new_reg(self):
        r = Register(self, len(self._registers))
        self._registers.append(r)
        return r

    def allocate(self):
        i = 0
        while i < len(self._registers):
            if self._registers[i] is None:
                self._registers[i] = Register(self, i)
                return self._registers[i]
            x = x + 1
        return self._allocate_new_reg()
    
    def allocateN(self, n):
        regs = []
        for i in range(0, n):
            regs.append(self._allocate_new_reg())
        return Registers(regs)
    
    def free(self, r):
        self._registers[r.get_number()] = None
        while self._registers and self._registers[-1] is None:
            self._registers.pop()

    def __init__(self):
        self._registers = []


class Scope:

    def __init__(self, parent, lexical, register_allocator):
        self._parent = parent
        self._lexical = lexical
        self._bindings = {}


class CodeGenerator:

    def _add_u8(self, u8):
        self._bytes.append(u8 & 0xff)
    
    def _add_u16(self, u16):
        self._add_u8((u16 >> 8) & 0xff)
        self._add_u8(u16 & 0xff)
    
    def _add_constant(self, c):
        index = len(self._constants)
        self._constants.append(c)
        self._add_u16(index)
        return index
    
    def _add_closure(self, c):
        index = len(self._closures)
        self._closures.append(c)
        self._add_u8(index)
        return index
    
    def _compile_self2r(self, reg):
        self._add_u8(Bytecodes.SELF)
        self._add_u8(reg)
    
    def _compile_const2r(self, to, value):
        self._add_u8(Bytecodes.C2R)
        self._add_u8(to)
        self._add_constant(value)

    def _compile_push(self, reg):
        self._add_u8(Bytecodes.PUSH)
        self._add_u8(reg)
    
    def _compile_pop(self, reg):
        self._add_u8(Bytecodes.POP)
        self._add_u8(reg)

    def _compile_r2r(self, to, from):
        self._add_u8(Bytecodes.R2R)
        self._add_u8(to)
        self._add_u8(from)
    
    def _compile_r2l(self, to, depth, from):
        self._add_u8(Bytecodes.R2L)
        self._add_u8(to)
        self._add_u8(depth)
        self._add_u8(from)
    
    def _compile_l2r(self, to, from, depth):
        self._add_u8(Bytecodes.L2R)
        self._add_u8(to)
        self._add_u8(from)
        self._add_u8(depth)
    
    def _compile_send(self, arg_cnt, key):
        self._add_u8(Bytecodes.SEND)
        self._add_u8(arg_cnt)
        self._add_constant(key)
    
    def _compile_assign(self, key):
        self._add_u8(Bytecodes.ASSIGN)
        self._add_constant(key)

    def _compile_return(self, depth):
        self._add_u8(Bytecodes.RETURN)
        self._add_u8(depth)

    def _compile_make_closure(self, dest, code_id):
        self._add_u8(Bytecodes.CLOSURE)
        self._add_u8(dest)
        self._add_closure(code_id)

    def compile_const(self, to, value):
        assert to.is_register() and not to.is_lexical()
        self._compile_c2r(to.get_number(), value)
    
    def compile_push(self, reg):
        assert reg.is_register() and not reg.is_lexical()
        self._compile_push(reg.get_number())
    
    def compile_pop(self, reg):
        assert reg.is_register() and not reg.is_lexical()
        self._compile_pop(reg.get_number())

    def compile_dup(self):
        self._add_u8(Bytecodes.DUP)

    def compile_drop(self):
        self._add_u8(Bytecodes.DROP)

    def compile_mov(self, to, from):
        assert from.is_register() and to.is_register()
        if from.is_lexical():
            assert not to.is_lexical()
            self._compile_l2r(to.get_number(), from.get_number(), from.get_depth())
        else:
            if to.is_lexical():
                self._compile_r2l(to.get_number(), to.get_depth(), from.get_number())
            else:
                self._compile_r2r(to.get_number(), from.get_number())

    def compile_send(self, arg_cnt, key):
        self._compile_send(arg_cnt, key)
    
    def compile_assign(self, key):
        self._compile_assign(key)
    
    def compile_return(self, depth):
        self._compile_return(depth)
    
    def compile_make_closure(self, reg, code):
        assert reg.is_register() and not reg.is_lexical()
        self._compile_make_closure(reg.get_number(), code)

    def finish(self):
        return None

    def __init__(self):
        self._bytes = []
        self._constants = []
        self._closures = []
