from ecosphere.objects.misc import EcoKey

class EConnectWriter:

    def _encode_vlq(self, num):
        b = bytearray()
        if num < 0:
            negative = True
            num = -num
        else:
            negative = False
        while num > 0:
            if b:
                b.append((num & 0x7f) | 0x80)
            else:
                b.append(num & 0x7f)
            num >>= 7
        if negative:
            b.append(0x80)
        b.reverse()
        return bytes(b)
    
    def write_byte(self, byte):
        self._bytes.append(byte)
    
    def write_vlq(self, num):
        for b in self._encode_vlq(num):
            self.write_byte(b)
    
    def write_bytes(self, bytes):
        self.write_vlq(len(bytes))
        for b in bytes:
            self.write_byte(b)
    
    def write_string(self, string):
        self.write_bytes(bytes(string, 'UTF-8'))

    def finish(self):
        return bytes(self._encode_vlq(len(self._bytes)) + self._bytes)

    def __init__(self):
        self._bytes = bytearray()


class Serializer(EConnectWriter):

    def add_object(self, object):
        obj_id = len(self._table)
        self._table[object] = obj_id
        return obj_id

    def write_message(self, name):
        key = ecosphere.objects.misc.EcoKey.Get(name)
        if key in self._table:
            self.write_vlq(self._table[key])
        else:
            obj_id = self._add_object(key)
            self.write_vlq(obj_id)
            self.write_string(name)

    def try_serialize_known_object(self, object):
        if object in self._table:
            self.write_message('ecosphere.econnect.id')
            self.write_vlq(self._table[object])
            return True
        else:
            return False

    def write_object(self, object):
        object.serialize(self)
    
    def write_objects(self, objects):
        self.write_vlq(len(objects))
        for object in objects:
            self.write_object(object)

    def __init__(self):
        self._table = dict()
