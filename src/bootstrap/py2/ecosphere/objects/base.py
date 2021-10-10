
class EcoObject:

    def lookup_key(self, key):
        return None
    
    def serialize(self, serializer, id=0):
        raise Exception('Can\'t serialize this object!')

    def __init__(self):
        pass

class EcoNumber(EcoObject):

    def serialize(self, serializer, id=0):
        serializer.write_message('ecosphere.object.int')
        serializer.write_vlq(self._value)

    def __init__(self, value):
        self._value = value
