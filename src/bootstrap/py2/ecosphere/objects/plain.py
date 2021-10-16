import ecosphere.objects.base


class EcoSlot:

    def serialize(self, serializer):
        raise Exception('Can\'t serialize this slot!')

    def __init__(self, name):
        self._name = name


class EcoValueSlot(EcoSlot):

    def serialize(self, serializer):
        flags = 0x00
        if self._value is not None: flags |= 0x02
        if self._is_inherited:      flags |= 0x04
        if self._is_part:           flags |= 0x08
        serializer.write_vlq(flags)
        serializer.write_object(self._name)
        if self._value is not None:
            serializer.write_object(self._value)

    def set_value(self, value):
        self._value = value
        callbacks = self._callbacks
        self._callbacks = None
        for e in callbacks:
            e(self._value)
    
    def evaluate(self, the_subject, the_environment, the_callback, args=None):
        if self._callbacks is None:
            the_callback(self._value) # TODO: Wait for the value, in case it wasn't set yet
        else:
            self._callbacks.append(the_callback)

    def __init__(self, name, is_inherited, is_part):
        super().__init__(name)
        self._is_inherited = is_inherited
        self._is_part = is_part
        self._value = None
        self._callbacks = list()


class EcoCodeSlot(EcoSlot):

    def serialize(self, serializer):
        flags = 0x01
        serializer.write_vlq(flags)
        serializer.write_object(self._name)
        serializer.write_object(self._code)

    def evaluate(self, the_subject, the_environment, the_callback, args=list()):
        # TODO, XXX: Handle args, create a new environment
        self._code.get_ast().evaluate(the_subject, the_environment, the_callback)

    def __init__(self, name, code):
        super().__init__(name)
        self._code = code


class EcoPlainObject(ecosphere.objects.base.EcoObject):

    def find_slot_by_name(self, key):
        for s in self._slots:
            if s.get_name() == key:
                return s
        return None
    
    def lookup_key(self, key):
        return self.find_slot_by_name(key)
    
    def add_slot(self, slot):
        self._slots.append(slot)
    
    def serialize(self, serializer):
        if not serializer.try_serialize_known_object(self):
            serializer.write_message('ecosphere.object.object')
            serializer.write_vlq(serializer.add_object(self))
            serializer.write_vlq(len(self._slots))
            for slot in self._slots:
                slot.serialize(serializer)

    def __init__(self):
        super().__init__
        self._slots = list()
