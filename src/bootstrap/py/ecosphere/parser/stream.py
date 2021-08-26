import ecosphere.parser.tokens
import ecosphere.parser.parsers


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
    
    def get_parser(self, parse_manager=None):
        return ecosphere.parser.parsers.EcoParser(ecosphere.parser.tokens.Tokenizer(self), parse_manager)

    def __init__(self, string):
        self._string = string
        self._index = 0
