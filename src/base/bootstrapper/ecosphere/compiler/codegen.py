import ecosphere.objects.misc

class Bytecodes:
    NOOP = 0x00
    CONST = 0x01
    PUSH = 0x02
    POP = 0x03
    LOAD_LOCAL = 0x04
    STORE_LOCAL = 0x05
    LOAD_LEXICAL = 0x06
    STORE_LEXICAL = 0x07
    BUILTIN = 0x08
    BUILTINV = 0x09
    SEND = 0x0a
    SENDV = 0x0b
    RESEND = 0x0c
    RESENDV = 0x0d
    ASSIGN = 0x0e
    RETURN = 0x0f
    CLOSURE = 0x10

    JUMP = 0x40


class CodeWriter:

    def _u8(self, b):
        self._instructions.append(b)

    def _u16(self, v):
        self._u8(v & 0xff)
        self._u8((v >> 8) & 0xff)

    def _add_constant(self, c):
        self._u16(len(self._constants))
        self._constants.append(c)
    
    def _add_cb_constant(self):
        index = len(self._constants)
        self._u16(index)
        self._constants.append(None)
        def callback(value):
            self._constants[index] = value
        return callback

    def write_noop(self):
        self._u8(Bytecodes.NOOP)

    def write_const(self, c):
        self._u8(Bytecodes.CONST)
        self._add_constant(c)
    
    def write_const_cb(self):
        self._u8(Bytecodes.CONST)
        return self._add_cb_constant()

    def write_pushc(self, c):
        self.write_const(c)
        self.write_push()
    
    def write_pushc_cb(self):
        c = self.write_const_cb()
        self.write_push()
        return c

    def write_push(self):
        self._u8(Bytecodes.PUSH)

    def write_pop(self):
        self._u8(Bytecodes.POP)
    
    def write_drop(self):
        self.write_pop()
    
    def write_dup(self):
        self.write_pop()
        self.write_push()
        self.write_push()

    def write_load_arg(self, i):
        self.write_load_local(i)
    
    def write_store_arg(self, i):
        self.write_load_local(i)

    def write_load_local(self, i):
        self._u8(Bytecodes.LOAD_LOCAL)
        self._u8(i)
    
    def write_store_local(self, i):
        self._u8(Bytecodes.STORE_LOCAL)
        self._u8(i)

    def write_load_lexical(self, d, i):
        self._u8(Bytecodes.LOAD_LEXICAL)
        self._u8(d)
        self._u8(i)
    
    def write_store_lexical(self, d, i):
        self._u8(Bytecodes.STORE_LEXICAL)
        self._u8(d)
        self._u8(i)
    
    def write_load(self, d, i):
        if d == 0:
            self.write_load_local(i)
        else:
            self.write_load_lexical(d, i)
    
    def write_store(self, d, i):
        if d == 0:
            self.write_store_local(i)
        else:
            self.write_store_lexical(d, i)

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

    def write_closure(self, code_object):
        self._u8(Bytecodes.CLOSURE)
        self._add_constant(code_object)
    
    def write_jump_offset_depth(self, offset, depth):
        self._u8(Bytecodes.JUMP)
        self._u8(depth)
        self._u16(offset)
    
    def write_jump_offset(self, offset):
        self.write_jump_offset_depth(offset, 0)

    def finish(self):
        return bytes(self._instructions), self._constants

    def __init__(self):
        self._instructions = bytearray()
        self._constants = list()

class CodeGenerator:

    def _drop_last_value(self):
        self._transfer_last_value(None)

    def _get_self(self):
        return self._scope.get_storage_manager().get_self_with_depth(self._scope.get_depth())
    
    def _transfer_value(self, src, dst):
        if src is None:
            src = self._get_self()
        if dst is None:
            if src.is_stack():
                self._writer.write_drop()
            else:
                pass
        elif dst.is_stack():
            if src.is_stack():
                pass
            elif src.is_register():
                self._writer.write_load(src.get_depth(), src.get_bc_register_number())
                self._writer.write_push()
            elif src.is_constant():
                cb = self._writer.write_const_cb()
                src.with_value(cb)
                self._writer.write_push()
            elif src.is_closure():
                self._writer.write_closure(src.get_code())
                self._writer.write_push()
            else:
                pass # TODO: Error
        elif dst.is_register():
            if src.is_stack():
                self._writer.write_pop()
                self._writer.write_store(dst.get_depth(), dst.get_bc_register_number())
            elif src.is_register():
                self._writer.write_load(src.get_depth(), src.get_bc_register_number())
                self._writer.write_store(dst.get_depth(), dst.get_bc_register_number())
            elif src.is_constant():
                cb = self._writer.write_const_cb()
                src.with_value(cb)
                self._writer.write_store(dst.get_depth(), dst.get_bc_register_number())
            elif src.is_closure():
                self._writer.write_closure(src.get_code())
                self._writer.write_store(dst.get_depth(), dst.get_bc_register_number())
            else:
                pass # TODO: Error
        else:
            pass # TODO: Error
    
    def _transfer_last_value(self, target):
        if not self._last_value:
            if target is None:
                # We're transferring None to None -> do nothing
                return
            self.load_self()
        f = self._last_value
        self._last_value = None
        f(target)
    
    def _set_last_value(self, v):
        self._last_value = lambda target: self._transfer_value(v, target)
    
    def _set_last_value_to_accu(self):
        self._set_last_value(self._scope.get_storage_manager().get_accu())

    def _set_last_value_to_stack(self):
        self._set_last_value(self._scope.get_storage_manager().get_stack())

    def load_self(self):
        self._drop_last_value()
        self._set_last_value(self._get_self())

    def load_constant_cb(self):
        constant = self._scope.get_storage_manager().get_constant()
        self._set_last_value(constant)
        return constant

    def load_constant(self, c):
        constant = self._scope.get_storage_manager().get_constant()
        constant(c)
        self._set_last_value(constant)

    def load_var(self, name):
        self._drop_last_value()
        the_type, storage_location = self._scope.get_binding(name)
        if storage_location is not None:
            self._set_last_value(storage_location)
            return True
        else:
            return False
    
    def is_var(self, name):
        the_type, storage_location = self._scope.get_binding(name)
        return storage_location is not None

    def store_var(self, name, visitor):
        the_type, storage_location = self._scope.get_binding(name)
        if storage_location is not None:
            # If the_type is not None, do a type check before assigning
            if the_type is not None:
                self.push()
                the_type.accept(visitor)
                self.op_as()
            self._transfer_last_value(storage_location)
            return True
        else:
            return False

    def push(self):
        self._transfer_last_value(self._scope.get_storage_manager().get_stack())

    def op_builtin(self, args, key):
        self._drop_last_value()
        self._writer.write_builtin(args, key)
        # self._set_last_value_to_stack()

    def op_builtinv(self, args, key):
        self._drop_last_value()
        self._writer.write_builtinv(args, key)
        # self._set_last_value_to_stack()

    def op_send(self, args, key):
        self._writer.write_send(args, key)
        # self._set_last_value_to_stack()

    def op_sendv(self, args, key):
        self._writer.write_sendv(args, key)
        # self._set_last_value_to_stack()
    
    def op_assign(self, key):
        self._writer.write_assign(key)
        # self._set_last_value_to_stack()

    def op_return(self, depth, the_type, visitor):
        if the_type is not None:
            self.push()
            the_type.accept(visitor)
            self.op_as()
        # self.push()
        self._writer.write_return(depth)

    def op_closure(self, code):
        self._drop_last_value()
        def writer(target):
            if target is not None:
                self._writer.write_closure(code)
                if target.is_register():
                    self._writer.write_store(target.get_depth(), target.get_bc_register_number())
                elif target.is_stack():
                    self._writer.write_push()
                else:
                    raise Exception('Ow')
        self._last_value = writer
    
    def finish(self, the_type, visitor):
        self.op_return(0, the_type, visitor)
        instructions, constants = self._writer.finish()
        self._writer = None
        return ecosphere.objects.misc.EcoCode(instructions,
                                              constants,
                                              self._scope.get_storage_manager().get_max_register_count(),
                                              self._scope.get_parameter_count() + 1,  # One more for "self"
                                              self._scope.has_varargs())

    def __init__(self, writer: CodeWriter, scope):
        self._writer = writer
        self._scope = scope
        self._last_value = None
