
class EcoObject:

    def lookup_key(self, key):
        return None
    
    def serialize(self, serializer):
        raise Exception('Can\'t serialize this object!')

    def __init__(self):
        pass

class EcoNumber(EcoObject):

    def serialize(self, serializer):
        serializer.write_message('ecosphere.object.int')
        serializer.write_vlq(self._value)

    def __init__(self, value):
        self._value = value

class EcoCharacter(EcoObject):

    def serialize(self, serializer):
        serializer.write_message('ecosphere.object.character')
        serializer.write_vlq(self._value)

    def __init__(self, value):
        self._value = value
