import datatypes


class Serializer:

    def write_byte(self, b):
        self._bytes.append(b)

    def write_bytes(self, bts):
        for b in bts:
            self.write_byte(b)
    
    def write_uint(self, i):
        bts = []
        while i != 0:
            bts.append(i & 0x7f)
            i >>= 7
        if not bts:
            self.write_byte(0)
        else:
            bts.reverse()
            for x in range(0, len(bts) - 1):
                self.write_byte(bts[x] | 0x80)
            self.write_byte(bts[-1])

    def write_byte_sequence(self, bts):
        self.write_uint(len(bts))
        self.write_bytes(bts)

    def write_string(self, s):
        self.write_byte_sequence(bytes(s, 'UTF-8'))
    
    def write_object_sequence(self, s):
        self.write_uint(len(s))
        for e in s:
            e.serialize(self)

    def open_message(self, name):
        k = datatypes.Key.get(name)
        id = self.get_id(k)
        if id:
            self.write_uint(id)
        else:
            self.write_uint(0)
            k.serialize_core(self, self.bind(k))

    def serialize_object(self, obj):
        id = self.get_id(obj)
        if id:
            self.open_message('ecosphere.econnect.id')
            self.write_uint(id)
        else:
            obj.do_serialize(self, self.bind(obj))

    def get_id(self, obj):
        if obj in self._objects:
            return self._objects[obj]
        else:
            return None
    
    def bind(self, obj):
        self._id_counter = self._id_counter + 1
        self._objects[obj] = self._id_counter
        return self._id_counter

    def finish(self):
        return bytes(self._bytes)

    def __init__(self):
        self._bytes = bytearray()
        self._objects = dict()
        self._id_counter = 0
