import ecosphere.objects.base

class EcoKey(ecosphere.objects.base.EcoObject):
    KEYS = dict()

    def __repr__(self):
        return '#\'' + self._name + '\''

    def dot_extend(self, extension):
        return EcoKey.Get(self._name + ':' + extension)

    def Get(name):
        if name not in EcoKey.KEYS:
            k = EcoKey(name)
            EcoKey.KEYS[name] = k
        return EcoKey.KEYS[name]

    def __init__(self, name):
        self._name = name

class EcoCode(ecosphere.objects.base.EcoObject):

    def __init__(self, instructions, constants, closures, parameter_count, has_varargs):
        super().__init__()
        self._instructions = instructions
        self._constants = constants
        self._closures = closures
        self._parameters = parameter_count
        self._has_varargs = has_varargs
        for i in instructions:
            print(i)
