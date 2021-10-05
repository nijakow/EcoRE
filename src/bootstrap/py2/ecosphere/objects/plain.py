import ecosphere.objects.base


class EcoSlot:

    def __init__(self, name):
        self._name = name


class EcoValueSlot(EcoSlot):

    def set_value(self, value):
        self._value = value

    def __init__(self, name, is_inherited, is_part):
        super().__init__(name)
        self._is_inherited = is_inherited
        self._is_part = is_part
        self._value = None


class EcoCodeSlot(EcoSlot):

    def __init__(self, name, code):
        super().__init__(name)
        self._code = code


class EcoPlainObject(ecosphere.objects.base.EcoObject):

    def find_slot_by_name(self, key):
        for s in self._slots:
            if s.get_name() == key:
                return s
        return None
    
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
