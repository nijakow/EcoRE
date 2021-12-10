import ecosphere.objects.misc

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
    AS = 0x0f
    RETURN = 0x10
    CLOSURE = 0x11


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

    def _add_code_object(self, c):
        self._u16(len(self._code_objects))
        self._code_objects.append(c)

    def write_noop(self):
        self._u8(Bytecodes.NOOP)

    def write_const(self, r, c):
        self._u8(Bytecodes.CONST)
        self._u8(r)
        self._add_constant(c)
    
    def write_const_cb(self, r):
        self._u8(Bytecodes.CONST)
        self._u8(r)
        return self._add_cb_constant()

    def write_pushc(self, c):
        self._u8(Bytecodes.PUSHC)
        self._add_constant(c)
    
    def write_pushc_cb(self):
        self._u8(Bytecodes.PUSHC)
        return self._add_cb_constant()

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
        self._u8(depth)

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
    
    def write_as(self):
        self._u8(Bytecodes.AS)

    def write_return(self, depth):
        self._u8(Bytecodes.RETURN)
        self._u8(depth)

    def write_closure(self, dest, code_object):
        self._u8(Bytecodes.CLOSURE)
        self._u8(dest)
        self._add_code_object(code_object)

    def finish(self):
        return bytes(self._instructions), self._constants, list(self._code_objects)

    def __init__(self):
        self._instructions = bytearray()
        self._constants = list()
        self._code_objects = list()

class CodeGenerator:

    def _drop_last_value(self):
        self._transfer_last_value(None)
    
    def _transfer_value(self, src, dst):
        if src is None:
            src = self._scope.get_storage_manager().get_self()
        if dst is None:
            if src.is_stack():
                self._writer.write_drop()
            else:
                pass
        elif dst.is_stack():
            if src.is_stack():
                pass
            elif src.is_register():
                if src.get_depth() > 0:
                    v = self._scope.get_storage_manager().allocate()
                    self._transfer_value(src, v)
                    self._transfer_value(v, dst)
                    v.free()
                else:
                    self._writer.write_push(src.get_register_number())
            elif src.is_constant():
                cb = self._writer.write_pushc_cb()
                src.with_value(cb)
            else:
                pass # TODO: Error
        elif dst.is_register():
            if src.is_stack():
                if dst.get_depth() > 0:
                    v = self._scope.get_storage_manager().allocate()
                    self._transfer_value(src, v)
                    self._transfer_value(v, dst)
                    v.free()
                else:
                    self._writer.write_pop(dst.get_register_number())
            elif src.is_register():
                if src.get_depth() == 0 and dst.get_depth() == 0:
                    self._writer.write_r2r(dst.get_register_number(), src.get_register_number())
                elif src.get_depth() > 0 and dst.get_depth() == 0:
                    self._writer.write_l2r(dst.get_register_number(), src.get_register_number(), src.get_depth())
                elif src.get_depth() == 0 and dst.get_depth() > 0:
                    self._writer.write_r2l(dst.get_register_number(), dst.get_depth(), src.get_register_number())
                else:
                    v = self._scope.get_storage_manager().allocate()
                    self._transfer_value(src, v)
                    self._transfer_value(v, dst)
                    v.free()
            elif src.is_constant():
                if dst.get_depth() > 0:
                    v = self._scope.get_storage_manager().allocate()
                    self._transfer_value(src, v)
                    self._transfer_value(v, dst)
                    v.free()
                else:
                    cb = self._writer.write_const_cb(dst.get_register_number())
                    src.with_value(cb)
            elif src.is_closure():
                if dst.get_depth() > 0:
                    v = self._scope.get_storage_manager().allocate()
                    self._transfer_value(src, v)
                    self._transfer_value(v, dst)
                    v.free()
                else:
                    self._writer.write_closure(dst.get_register_number(), src.get_code())
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
    
    def _set_last_value_to_stack(self):
        self._set_last_value(self._scope.get_storage_manager().get_stack())

    def load_self(self):
        self._drop_last_value()
        self._set_last_value(self._scope.get_storage_manager().get_self())

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

    def store_var(self, name, loader):
        the_type, storage_location = self._scope.get_binding(name)
        if storage_location is not None:
            # If the_type is not None, do a type check before assigning
            if the_type is not None:
                self.push()
                callback = self.load_constant_cb()
                the_type.evaluate(None, loader, callback)
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
        self._set_last_value_to_stack()

    def op_builtinv(self, args, key):
        self._drop_last_value()
        self._writer.write_builtinv(args, key)
        self._set_last_value_to_stack()

    def op_send(self, args, key):
        self._writer.write_send(args, key)
        self._set_last_value_to_stack()

    def op_sendv(self, args, key):
        self._writer.write_sendv(args, key)
        self._set_last_value_to_stack()
    
    def op_assign(self, key):
        self._writer.write_assign(key)

    def op_return(self, depth):
        self.push()
        self._writer.write_return(depth)

    def op_closure(self, code):
        self._drop_last_value()
        def writer(target):
            if target is not None:
                if target.is_register() and target.get_depth() == 0:
                    self._writer.write_closure(target.get_register_number(), code)
                else:
                    v = self._scope.get_storage_manager().allocate()
                    self._writer.write_closure(v.get_register_number(), code)
                    self._transfer_value(v, target)
                    v.free()
        self._last_value = writer
    
    def op_as(self):
        self.push()
        self._writer.write_as()
        self._set_last_value_to_stack()
    
    def finish(self):
        self.op_return(0)
        instructions, constants, objects = self._writer.finish()
        self._writer = None
        return ecosphere.objects.misc.EcoCode(instructions,
                                              constants,
                                              objects,
                                              self._scope.get_storage_manager().get_max_register_count(),
                                              self._scope.get_parameter_count() + 1,  # One more for "self"
                                              self._scope.has_varargs())

    def __init__(self, writer: CodeWriter, scope):
        self._writer = writer
        self._scope = scope
        self._last_value = None
