
class TextStream:

    def peek(self):
        try:
            return self._string[self._index]
        except:
            return ''

    def read(self):
        c = self.peek()
        if c:
            self._index += 1
        return c

    def __init__(self, string):
        self._string = string
        self._index = 0
