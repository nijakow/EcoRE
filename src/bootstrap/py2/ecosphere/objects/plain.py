import ecosphere.objects.base


class EcoSlot:

    def __init__(self, name):
        self._name = name


class EcoValueSlot(EcoSlot):

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
        if slot in self._slot_def_callbacks:
            for f in self._slot_def_callbacks[slot]:
                f(slot)

    def when_slot_defined(self, key, func):
        s = self.find_slot_by_name(key)
        if s is not None:
            func(s)
        else:
            if key not in self._slot_def_callbacks:
                self._slot_def_callbacks[key] = list()
            self._slot_def_callbacks[key].append(func)

    def __init__(self):
        super().__init__
        self._slots = list()
