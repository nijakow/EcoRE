
OBJECTS = set()
KEYS = None

class EObject:

    def __getitem__(self, i):
        return self._slots[i]
    
    def __setitem__(self, i, v):
        self._slots[i] = v
    
    def __str__(self):
        return str(type(self)) + ' ' + str(len(self._slots))

    def __init__(self, tp, size):
        self._slots = [None] * size
        self[0] = tp
        OBJECTS.add(self)

class AObject(EObject):
    def __init__(self, tp, size):
        super().__init__(tp, size)

class BObject(EObject):

    def __str__(self):
        return str(self._bytes)

    def __init__(self, tp, bts):
        super().__init__(tp, 2)
        self._bytes = bts

class String(AObject):

    def __init__(self, pystr):
        super().__init__(None, 4)
        self[2] = len(pystr)
        self[3] = BObject(None, bytes(pystr, 'UTF-8'))

class Key(AObject):

    def __init__(self, next, name):
        super().__init__(None, 4)
        self[2] = next
        self[3] = name

class Array(AObject):

    def __init__(self, count):
        super().__init__(None, count + 2)

class Code(AObject):

    def set_bytecodes(self, b):
        self[2] = b
    
    def set_constants(self, c):
        self[3] = c
    
    def set_closures(self, c):
        self[4] = c
    
    def set_register_count(self, r):
        self[5] = r
    
    def set_parameters(self, p):
        self[6] = p
    
    def set_has_varargs(self, v):
        self[7] = v

    def __init__(self):
        super().__init__(None, 8)

class Type(AObject):

    def set_slot(self, i, v):
        self[i + 2] = v

    def __init__(self, count):
        super().__init__(None, count + 2)

class Slot(AObject):
    # Layout:
    #  - <parent>
    #  - name
    #  - flags
    #  - offset / method
    #  - referenced type

    def set_name(self, name):
        self[2] = name
    
    def set_flags(self, flags):
        self[3] = flags
    
    def set_offset(self, offset):
        self[4] = offset
    
    def set_method(self, method):
        self[4] = method
    
    def set_referenced_type(self, tp):
        self[5] = tp

    def __init__(self):
        super().__init__(None, 6)
        self[3] = 0

class Transformer:

    def get(self, obj):
        if obj not in self._objs:
            if isinstance(obj, bytes):
                self.put(obj, BObject(None, bytes))
            elif isinstance(obj, str):
                self.put(obj, String(obj))
            else:
                obj.transform(self)
        return self._objs[obj]
    
    def put(self, obj, val):
        assert isinstance(val, EObject)
        self._objs[obj] = val
    
    def make_array(self, elements):
        a = Array(len(elements))
        for i in range(0, len(elements)):
            a[i + 2] = self.get(elements[i])
        return a
    
    def make_key(self, name):
        global KEYS
        k = Key(KEYS, name)
        KEYS = k
        return k

    def __init__(self):
        self._objs = dict()
