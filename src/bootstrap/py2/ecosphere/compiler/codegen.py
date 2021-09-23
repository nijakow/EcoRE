
class Bytecodes:
    NOOP = 0x00
    CONST = 0x01
    PUSHC = 0x02
    PUSH = 0x03
    POP = 0x04
    DROP = 0x05
    DUP = 0x06
    R2R = 0x07
    R2L = 0x08
    L2R = 0x09
    BUILTIN = 0x0a
    BUILTINV = 0x0b
    SEND = 0x0c
    SENDV = 0x0d
    ASSIGN = 0x0e
    RETURN = 0x0f
    CLOSURE = 0x10


class CodeWriter:

    def _u8(self, b):
        self._instructions.append(b)

    def _u16(self, v):
        self._u8(v & 0xff)
        self._u8((v >> 8) & 0xff)

    def _add_constant(self, c):
        self._u16(len(self._constants))
        self._constants.append(c)

    def _add_code_object(self, c):
        self._u16(len(self._code_objects))
        self._code_objects.append(c)

    def write_noop(self):
        self._u8(Bytecodes.NOOP)

    def write_const(self, c):
        self._u8(Bytecodes.CONST)
        self._add_constant(c)

    def write_pushc(self, c):
        self._u8(Bytecodes.PUSHC)
        self._add_constant(c)

    def write_push(self, r):
        self._u8(Bytecodes.PUSH)
        self._u8(r)

    def write_pop(self, r):
        self._u8(Bytecodes.POP)
        self._u8(r)

    def write_drop(self):
        self._u8(Bytecodes.DROP)

    def write_dup(self):
        self._u8(Bytecodes.DUP)

    def write_r2r(self, dest, src):
        self._u8(Bytecodes.R2R)
        self._u8(dest)
        self._u8(src)

    def write_r2l(self, dest, depth, src):
        self._u8(Bytecodes.R2L)
        self._u8(dest)
        self._u8(depth)
        self._u8(src)

    def write_l2r(self, dest, src, depth):
        self._u8(Bytecodes.L2R)
        self._u8(dest)
        self._u8(src)
        self._u8(src)

    def write_builtin(self, args, key):
        self._u8(Bytecodes.BUILTIN)
        self._u8(args)
        self._add_constant(key)

    def write_builtinv(self, args, key):
        self._u8(Bytecodes.BUILTINV)
        self._u8(args)
        self._add_constant(key)

    def write_send(self, args, key):
        self._u8(Bytecodes.SEND)
        self._u8(args)
        self._add_constant(key)

    def write_sendv(self, args, key):
        self._u8(Bytecodes.SENDV)
        self._u8(args)
        self._add_constant(key)

    def write_assign(self, key):
        self._u8(Bytecodes.ASSIGN)
        self._add_constant(key)

    def write_return(self, depth):
        self._u8(Bytecodes.RETURN)
        self._u8(depth)

    def write_closure(self, dest, code_object):
        self._u8(Bytecodes.CLOSURE)
        self._u8(dest)
        self._add_code_object(code_object)

    def finish(self):
        return bytes(self._instructions), list(self._constants), list(self._code_objects)

    def __init__(self):
        self._instructions = bytearray()
        self._constants = list()
        self._code_objects = list()

class CodeGenerator:

    def _drop_last_value(self):
        if self._last_value is None:
            pass
        elif self._last_value.is_register():
            pass
        elif self._last_value.is_stack():
            self._writer.write_pop()
        elif self._last_value.is_constant():
            pass
        self._last_value = None

    def load_self(self, c):
        self._last_value = self._scope.get_storage_manager().get_self()

    def load_constant(self, c):
        self._last_value = self._scope.get_storage_manager().get_constant(c)

    def load_var(self, name):
        self._drop_last_value()
        storage_location = self._scope.get_binding(name)
        if storage_location is not None:
            self._last_value = storage_location
            return True
        else:
            return False

    def store_var(self, storage_location, depth):
        pass # TODO

    def push(self):
        if self._last_value is None:
            self.load_self()
        if self._last_value.is_register():
            if self._last_value.get_depth() > 0:
                register = self.to_temporary_register()
                self._writer.write_push(register.get_index())
                register.free()
            else:
                self._writer.write_push(self._last_value.get_index())
        elif self._last_value.is_stack():
            pass # TODO: Duplicate?
        else:
            pass

    def op_builtin(self, args, key):
        pass # TODO

    def op_builtinv(self, args, key):
        pass # TODO

    def op_send(self, args, key):
        pass # TODO

    def op_sendv(self, args, key):
        pass # TODO

    def op_return(self, depth):
        pass # TODO

    def __init__(self, writer: CodeWriter, scope):
        self._writer = writer
        self._scope = scope
        self._last_value = None

