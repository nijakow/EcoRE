

class EcoSlot:

    def serialize(self, serializer):
        self.write_flags(serializer)
        self._name.serialize(serializer)

    def __init__(self, name):
        if isinstance(name, Key):
            self._name = name
        else:
            self._name = Key.get(name)


class ValueSlot(EcoSlot):

    def write_flags(self, serializer):
        flags = 0x00
        if self._is_inherited:
            flags |= 0x02
        serializer.write_byte(flags)
    
    def serialize(self, serializer):
        super().serialize(serializer)
        self._value.serialize(serializer)

    def __init__(self, name, value, inherited=False):
        super().__init__(name)
        self._value = value
        self._is_inherited = inherited


class EcoObject:

    def serialize(self, serializer):
        serializer.serialize_object(self)
    
    def do_serialize(self, serializer, id=0):
        serializer.write_uint(id)
        serializer.write_uint(len(self._slots))
        for slot in self._slots:
            slot.serialize(serializer)
    
    def add_slot(self, slot):
        self._slots.append(slot)

    def __init__(self):
        self._slots = []


class PlainObject(EcoObject):

    def do_serialize(self, serializer, id=0):
        serializer.open_message('ecosphere.object.object')
        super().do_serialize(serializer, id)

    def __init__(self):
        super().__init__()


class Key(EcoObject):
    KEYS = dict()

    def get(name):
        if name in Key.KEYS:
            return Key.KEYS[name]
        else:
            k = Key(name)
            Key.KEYS[name] = k
            return k

    def __repr__(self):
        return '#\'' + self._name + '\''

    def get_name(self):
        return self._name

    def do_serialize(self, serializer, id=0):
        serializer.open_message('ecosphere.object.key')
        self.serialize_core(serializer, id)
    
    def serialize_core(self, serializer, id):
        serializer.write_uint(id)
        serializer.write_string(self._name)

    def is_binary_op(self):
        c = self._name[-1]
        return not (str.isalpha(c) or str.isdigit(c))

    def colonize(self):
            return Key.get(self._name + ':')

    def extend_name(self, name):
        return Key.get(self._name + name.get_name())

    def __init__(self, name):
        super().__init__()
        self._name = name


class Code(EcoObject):

    def do_serialize(self, serializer, id=0):
        serializer.open_message('ecosphere.object.code')
        super().do_serialize(serializer, id)
        serializer.write_uint(self._register_count)
        serializer.write_uint(self._dynamics_count)
        serializer.write_uint(self._arg_count)
        serializer.write_object_sequence(self._constants)
        serializer.write_object_sequence(self._subcodes)
        serializer.write_byte_sequence(self._instructions)

    def __init__(self, register_count, dynamics_count, arg_count, constants, subcodes, instructions):
        super().__init__()
        self._register_count = register_count
        self._dynamics_count = dynamics_count
        self._arg_count = arg_count
        self._constants = constants
        self._subcodes = subcodes
        self._instructions = instructions
