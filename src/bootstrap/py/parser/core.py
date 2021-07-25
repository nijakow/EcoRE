from parser.tokens import TokenType as TokenType


class ParseException(Exception):

    def __init__(self, ast, message):
        super().__init__(message)
        self._ast = ast


class Parser:

    def check(self, token_type):
        t = self._t.read()
        if t.isa(token_type):
            return t
        else:
            t.fail()
            return False
    
    def check_identifier(self):
        return self.check(TokenType.IDENTIFIER)
    
    def expect(self, token_type):
        t = self._t.read()
        if t.isa(token_type):
            return t
        else:
            t.fail()
            raise ParseException(t, 'Expected different token type: ' + str(token_type) + ', got: ' + str(t))
    
    def expect_identifier(self):
        return self.expect(TokenType.IDENTIFIER)

    def get_tokenizer(self):
        return self._t

    def __init__(self, tokenizer):
        self._t = tokenizer


class SubParser(Parser):

    def get_parent(self):
        return self._parent

    def __init__(self, parent):
        super().__init__(parent.get_tokenizer())
        self._parent = parent
