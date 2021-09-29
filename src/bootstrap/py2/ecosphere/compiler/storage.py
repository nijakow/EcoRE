
class StorageLocation:

    def is_register(self):
        return False

    def is_stack(self):
        return False

    def is_constant(self):
        return False

    def __init__(self):
        pass

class StackLocation(StorageLocation):

    def is_stack(self):
        return True

    def __init__(self):
        pass

class Register(StorageLocation):

    def is_register(self):
        return True
    
    def get_register_number(self):
        return self._number

    def get_depth(self):
        return self._depth

    def increase_depth(self):
        return Register(None, self._number, self._depth + 1)
    
    def free(self):
        self._allocator.free(self)

    def __init__(self, allocator, number, depth=0):
        self._allocator = allocator
        self._number = number
        self._depth = depth

class Constant(StorageLocation):

    def is_constant(self):
        return True

    def get_value(self):
        return self._value

    def __init__(self, value):
        self._value = value


class StorageManager:

    def get_register(self, i):
        return self._registers(i)  # TODO: Allocate it if it does not exist

    def get_constant(self, c):
        return Constant(c)

    def allocate(self):
        i = 0
        m = len(self._registers)
        while i < m:
            if self._registers[i] is None:
                r = Register(self, i)
                self._registers[i] = r
                return r
            i += 1
        r = Register(self, m)
        self._registers.append(r)
        return r

    def free(self, r):
        i = 0
        m = len(self._registers)
        while i < m:
            if self._registers[i] == r:
                self._registers[i] = None

    def __init__(self):
        self._registers = list()
        self._bindings = dict()

