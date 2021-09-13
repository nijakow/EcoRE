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

    def __init__(self):
        super().__init__
        self._slots = list()
