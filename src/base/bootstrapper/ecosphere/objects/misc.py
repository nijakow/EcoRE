import ecosphere.objects.base
import ecosphere.vm

from ecosphere.compiler.codegen import Bytecodes

class EcoKey(ecosphere.objects.base.EcoObject):
    KEYS = dict()

    def __repr__(self):
        return '#\'' + self._name + '\''

    def dot_extend(self, extension):
        return EcoKey.Get(self._name + ':' + extension)
    
    def serialize(self, serializer):
        if not serializer.try_serialize_known_object(self):
            serializer.write_message('ecosphere.object.key')
            serializer.write_vlq(serializer.add_object(self))
            serializer.write_string(self._name)

    def Get(name):
        if name not in EcoKey.KEYS:
            k = EcoKey(name)
            EcoKey.KEYS[name] = k
        return EcoKey.KEYS[name]
    
    def transform(self, transformer):
        k = transformer.make_key(transformer.get(self._name))
        transformer.put(self, k)
        return k

    def __init__(self, name):
        self._name = name

class EcoString(ecosphere.objects.base.EcoObject):

    def serialize(self, serializer):
        if not serializer.try_serialize_known_object(self):
            serializer.write_message('ecosphere.object.string')
            serializer.write_vlq(serializer.add_object(self))
            serializer.write_string(self._string)
    
    def transform(self, transformer):
        s = transformer.get(self._name)
        transformer.put(self, s)
        return s

    def __init__(self, string):
        self._string = string

class EcoArray(ecosphere.objects.base.EcoObject):

    def get_setter(self, index):
        def func(value):
            self._elements[index] = value
        return func

    def serialize(self, serializer):
        if not serializer.try_serialize_known_object(self):
            serializer.write_message('ecosphere.object.array')
            serializer.write_vlq(serializer.add_object(self))
            serializer.write_objects(self._elements)
    
    def transform(self, transformer):
        e = len(self._elements)
        a = ecosphere.vm.Array(e)
        transformer.put(self, a)
        i = 0
        while i < e:
            a.put(i, transformer.get(self._elements[i]))
        return a

    def __init__(self, length):
        self._elements = [None] * length

class EcoCode(ecosphere.objects.base.EcoObject):
    class Disassembler:

        def _u8(self):
            b = self._code._instructions[self._i]
            self._i += 1
            return b
        
        def _u16(self):
            b0 = self._u8()
            b1 = self._u8()
            return (b1 << 8) | b0
        
        def _const(self):
            return self._code._constants[self._u16()]

        def _op(self, *text):
            for t in text:
                self._text += str(t)
                self._text += ' '
            self._text += '\n'

        def disassemble(self):
            while self._i < len(self._code._instructions):
                op = self._u8()
                if op == Bytecodes.NOOP:
                    self._op('NOOP')
                elif op == Bytecodes.CONST:
                    self._op('CONST', self._const())
                elif op == Bytecodes.PUSH:
                    self._op('PUSH')
                elif op == Bytecodes.POP:
                    self._op('POP')
                elif op == Bytecodes.LOAD_LOCAL:
                    i = self._u8()
                    self._op('LOAD_LOCAL', i)
                elif op == Bytecodes.STORE_LOCAL:
                    i = self._u8()
                    self._op('STORE_LOCAL', i)
                elif op == Bytecodes.LOAD_LEXICAL:
                    d = self._u8()
                    i = self._u8()
                    self._op('LOAD_LEXICAL', d, i)
                elif op == Bytecodes.STORE_LEXICAL:
                    d = self._u8()
                    i = self._u8()
                    self._op('LEXICAL', d, i)
                elif op == Bytecodes.BUILTIN:
                    args = self._u8()
                    self._op('BLT ', args, self._const())
                elif op == Bytecodes.BUILTINV:
                    args = self._u8()
                    self._op('BLTV', args, self._const())
                elif op == Bytecodes.SEND:
                    args = self._u8()
                    self._op('SEND', args, self._const())
                elif op == Bytecodes.SENDV:
                    args = self._u8()
                    self._op('SNDV', args, self._const())
                elif op == Bytecodes.ASSIGN:
                    self._op('ASGN', self._const())
                elif op == Bytecodes.RETURN:
                    self._op('RET ', self._u8())
                elif op == Bytecodes.CLOSURE:
                    self._op('CLOSURE', self._const())
                else:
                    self._op('???')
                    raise Exception('Huh? Unknown bytecode!')
            return self._text

        def __init__(self, code):
            self._code = code
            self._text = ''
            self._i = 0

    def serialize(self, serializer, id=0):
        if not serializer.try_serialize_known_object(self):
            serializer.write_message('ecosphere.object.code')
            serializer.write_vlq(serializer.add_object(self))
            serializer.write_vlq(self._register_count)
            serializer.write_vlq(self._parameters)
            if self._has_varargs:
                serializer.write_vlq(0x01)
            else:
                serializer.write_vlq(0x00)
            serializer.write_objects(self._constants)
            serializer.write_bytes(self._instructions)
    
    def transform(self, transformer):
        c = ecosphere.vm.Code()
        transformer.put(self, c)
        c.set_bytecodes(transformer.get(self._instructions))
        c.set_constants(transformer.make_array(self._constants))
        c.set_register_count(self._register_count)
        c.set_parameters(self._parameters)
        c.set_has_varargs(self._has_varargs)
        return c

    def __init__(self, instructions, constants, register_count, parameter_count, has_varargs):
        super().__init__()
        self._instructions = instructions
        self._constants = constants
        self._register_count = register_count
        self._parameters = parameter_count
        self._has_varargs = has_varargs
