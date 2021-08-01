
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



class Register(StorageLocation):

    def is_register(self):
        return True

    def is_local_register(self, scope):
        return self.get_depth() == 0

    def get_allocator(self):
        return self._allocator
    
    def get_id(self):
        return self._id

    def get_depth(self):
        return self._depth

    def free(self):
        self.get_allocator().free_register(self)

    def increment_depth(self):
        return Register(self.get_allocator(), self.get_id(), self.get_depth() + 1)
    
    def __init__(self, allocator, r_id, depth=0):
        self._allocator = allocator
        self._id = r_id
        self._depth = 0


class RegisterAllocator:

    def get_register_count(self):
        return self._max_register_count

    def allocate_register(self):
        i = 0
        while i < len(self._registers):
            if self._registers[i] is None:
                r = Register(self, i)
                self._registers[i] = r
                return r
            i = i + 1
        r = Register(self, i)
        self._registers.append(r)
        self._max_register_count = max(self._max_register_count, len(self._registers))
        return r

    def allocate_temporary_register(self):
        return self.allocate_register()

    def free_register(self, r):
        assert r.get_allocator() is self
        self._registers[r.get_id()] = None
    
    def __init__(self):
        self._registers = []
        self._max_register_count = 0
