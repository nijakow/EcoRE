
class Key:
    KEYS = dict()

    def get(name):
        if name in Key.KEYS:
            return Key.KEYS[name]
        else:
            k = Key(name)
            Key.KEYS[name] = k
            return k

    def __repr__(self):
        return '#\'' + self._name + '\''

    def takes_parameters(self):
        c = self._name[-1]
        return not (str.isalpha(c) or str.isdigit(c))

    def has_followup(self):
        return self._name[-1] == ':'

    def add_followup(self, name):
        return Key.get(self._name + name)

    def __init__(self, name):
        self._name = name
