
class StorageLocation:

    def is_register(self):
        return False
    
    def is_arg(self):
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

class RegisterLike(StorageLocation):

    def is_register(self):
        return True
    
    def get_register_number(self):
        return self._number
    
    def get_bc_register_number(self):
        if self.is_arg():
            return self._number
        else:
            return self._number + 128

    def get_depth(self):
        return self._depth
    
    def free(self):
        self._allocator.free(self)

    def __init__(self, allocator, number, depth=0):
        self._allocator = allocator
        self._number = number
        self._depth = depth

class Register(RegisterLike):

    def increase_depth(self):
        return Register(None, self._number, self._depth + 1)

    def __init__(self, allocator, number, depth=0):
        super().__init__(allocator, number, depth)

class Arg(RegisterLike):

    def is_arg(self):
        return True
    
    def increase_depth(self):
        return Arg(self._number, self._depth + 1)

    def __init__(self, number, depth=0):
        super().__init__(None, number, depth)

class Constant(StorageLocation):

    def is_constant(self):
        return True

    def with_value(self, callback):
        if self._value is not None:
            callback(self._value)
        else:
            self._callbacks.append(callback)
    
    def __call__(self, value):
        self._value = value
        cpy = list(self._callbacks)
        self._callbacks.clear()
        for cb in cpy:
            cb(value)

    def __init__(self):
        self._value = None
        self._callbacks = list()


class StorageManager:
    _SELF  = Arg(0)
    _STACK = StackLocation()

    def get_self(self):
        return StorageManager._SELF
    
    def get_self_with_depth(self, depth):
        return Arg(0, depth)

    def get_register(self, i):
        return self._registers[i]  # TODO: Allocate it if it does not exist

    def get_constant(self):
        return Constant()
    
    def get_stack(self):
        return StorageManager._STACK

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
        self._max_register_count = max(self._max_register_count, len(self._registers))
        return r

    def free(self, r):
        i = 0
        m = len(self._registers)
        while i < m:
            if self._registers[i] == r:
                self._registers[i] = None
            i += 1

    def get_max_register_count(self):
        return self._max_register_count

    def __init__(self):
        self._registers = list()
        self._bindings = dict()
        self._max_register_count = 0
