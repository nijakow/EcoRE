
class StorageLocation:

    def is_stack(self):
        return False

    def is_register(self):
        return False

    def is_local_register(self, scope):
        return False
    
    def __init__(self):
        pass


class Stack(StorageLocation):

    def is_stack(self):
        return True

    def __init__(self):
        pass

STACK = Stack()



class LocalRegister(StorageLocation):

    def is_register(self):
        return True

    def is_local_register(self, scope):
        return True

    def get_allocator(self):
        return self._allocator
    
    def get_id(self):
        return self._id

    def free(self):
        self.get_allocator().free_register(self)
    
    def __init__(self, allocator, r_id):
        self._allocator = allocator
        self._id = r_id


class RegisterAllocator:

    def allocate_register(self):
        i = 0
        while i < len(self._registers):
            if self._registers[i] is None:
                r = LocalRegister(self, i)
                self._registers[i] = r
                return r
            i = i + 1
        r = LocalRegister(self, i)
        self._registers.append(r)
        return r

    def allocate_temporary_register(self):
        return self.allocate_register()

    def free_register(self, r):
        assert r.get_allocator() is self
        self._registers[r.get_id()] = None
    
    def __init__(self):
        self._registers = []
