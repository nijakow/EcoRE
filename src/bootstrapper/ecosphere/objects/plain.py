import ecosphere.objects.base


class EcoSlot:

    def serialize(self, serializer):
        raise Exception('Can\'t serialize this slot!')
    
    def get_name(self):
        return self._name
    
    def add_value_callback(self, cb):
        raise Exception('Can\'t put a callback on this slot!')

    def __init__(self, name):
        self._name = name


class EcoValueSlot(EcoSlot):

    def serialize(self, serializer):
        flags = 0x00
        if self._value is not None: flags |= 0x02
        if self._is_inherited:      flags |= 0x04
        if self._is_no_delegate:    flags |= 0x08
        if self._is_part:           flags |= 0x10
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
        self.add_value_callback(the_callback)

    def add_value_callback(self, cb):
        if self._value is not None:
            cb(self._value)
        else:
            self._callbacks.append(cb)

    def __init__(self, name, is_inherited, is_no_delegate, is_part):
        super().__init__(name)
        self._is_inherited = is_inherited
        self._is_part = is_part
        self._is_no_delegate = is_no_delegate
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
    
    def add_up(self, value):
        self._ups.append(value)

    def add_slot(self, slot):
        self._slots.append(slot)
        name = slot.get_name()
        if name in self._callbacks:
            elems = self._callbacks[name]
            del self._callbacks[name]
            for e in elems:
                slot.add_value_callback(e)
    
    def when_defined(self, name, callback):
        slot = self.find_slot_by_name(name)
        if slot is not None:
            slot.add_value_callback(callback)
        else:
            if name not in self._callbacks:
                self._callbacks[name] = []
            self._callbacks[name].append(callback)
    
    def serialize(self, serializer):
        if not serializer.try_serialize_known_object(self):
            serializer.write_message('ecosphere.object.molecule')
            serializer.write_vlq(serializer.add_object(self))
            serializer.write_vlq(len(self._ups))
            for up in self._ups:
                serializer.write_object(up)
            serializer.write_vlq(len(self._slots))
            for slot in self._slots:
                slot.serialize(serializer)

    def __init__(self):
        super().__init__()
        self._ups = list()
        self._slots = list()
        self._callbacks = dict()
