
class StorageLocation:

    def __init__(self):
        pass

class StackLocation(StorageLocation):

    def __init__(self):
        pass

class Register(StorageLocation):

    def __init__(self, allocator, number):
        self._allocator = allocator
        self._number = number


class RegisterAllocator:

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

