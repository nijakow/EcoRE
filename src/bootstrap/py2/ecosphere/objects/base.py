
class EcoObject:

    def lookup_key(self, key):
        return None

    def __init__(self):
        pass

class EcoNumber(EcoObject):

    def __init__(self, value):
        self._value = value
