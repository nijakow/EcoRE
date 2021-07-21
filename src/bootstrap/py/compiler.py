import enum


class Bytecodes(enum.IntEnum):
    NOOP         = 0
    SELF         = 1
    C2R          = 2
    R2R          = 3
    R2L          = 4
    L2R          = 5
    A2R          = 6
    SEND         = 7
    ASSIGN       = 8
    RETURN       = 9
    MAKE_CLOSURE = 10


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

class ReturnStorageSlot(StorageLocation):

    def is_return_storage_slot(self):
        return True

    def __init__(self):
        super().__init__()

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
    
    def _compile_self2r(self, reg):
        self._add_u8(Bytecodes.SELF)
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
    
    def _compile_a2r(self, to):
        self._add_u8(Bytecodes.A2R)
        self._add_u8(to)
    
    def _compile_send(self, key, arg_loc, arg_cnt):
        self._add_u8(Bytecodes.SEND)
        self._add_constant(key)
        self._add_u8(arg_loc)
        self._add_u8(arg_cnt)
    
    def _compile_assign(self, target_reg, key, value_loc):
        self._add_u8(Bytecodes.ASSIGN)
        self._add_u8(target_reg)
        self._add_constant(key)
        self._add_u8(value_loc)

    def _compile_return(self, reg, depth):
        self._add_u8(Bytecodes.RETURN)
        self._add_u8(reg)
        self._add_u8(depth)

    def _compile_c2r(self, to, value):
        self._add_u8(Bytecodes.C2R)
        self._add_u8(to)
        self._add_constant(value)

    def compile_c2r(self, to, value):
        assert to.is_register() and not to.is_lexical()
        self._compile_c2r(to.get_number(), value)

    def compile_mov(self, to, from):
        if from.is_return_storage_slot():
            assert to.is_register() and not to.is_lexical()
            self._compile_a2r(to.get_number())
        else:
            assert from.is_register() and to.is_register()
            if from.is_lexical():
                assert not to.is_lexical()
                self._compile_l2r(to.get_number(), from.get_number(), from.get_depth())
            else:
                if to.is_lexical():
                    self._compile_r2l(to.get_number(), to.get_depth(), from.get_number())
                else:
                    self._compile_r2r(to.get_number(), from.get_number())

    def finish(self):
        return None

    def __init__(self):
        self._bytes = []
        self._constants = []
