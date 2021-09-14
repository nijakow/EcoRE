
class StringStream:

    def peek(self) -> str:
        try:
            return self._s[self._x]
        except:
            return None
    
    def read(self) -> str:
        c = self.peek()
        self._x += 1
        return c
    
    def peeks(self, s: str) -> bool:
        the_x = self._x
        for c in s:
            if self.read() != c:
                self._x = the_x
                return False
        return True

    def __init__(self, s: str):
        self._s = s
        self._x = 0
