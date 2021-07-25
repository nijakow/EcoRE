import enum

import datatypes


class TokenType(enum.Enum):
    EOF        = enum.auto()
    IDENTIFIER = enum.auto()
    KEY        = enum.auto()
    LABEL      = enum.auto()
    LPAREN     = enum.auto()
    RPAREN     = enum.auto()
    LBRACK     = enum.auto()
    RBRACK     = enum.auto()
    LCURLY     = enum.auto()
    RCURLY     = enum.auto()
    COLON      = enum.auto()
    SEPARATOR  = enum.auto()
    RARROW     = enum.auto()
    EQUALS     = enum.auto()
    CARET      = enum.auto()
    BAR        = enum.auto()
    SELF       = enum.auto()
    WITH       = enum.auto()
    PRIVATE    = enum.auto()


class Token:

    def fail(self):
        self._tokenizer.unread(self)

    def is_type(self, token_type):
        return self._token_type == token_type

    def isa(self, token_type):
        return self.is_type(token_type)

    def check(self, token_type):
        if self.is_type(token_type):
            return self
        else:
            self.fail()
            return False

    def expect(self, token_type):
        if not self.is_type(token_type):
            raise Exception('Oof, expected ' + str(token_type) + ', got ' + str(self))
        return self

    def __init__(self, tokenizer, token_type):
        self._tokenizer = tokenizer
        self._token_type = token_type


class IdentifierToken(Token):

    def get_name(self):
        return self._name
    
    def get_key(self):
        return datatypes.Key.get(self.get_name())

    def __init__(self, tokenizer, name):
        super().__init__(tokenizer, TokenType.IDENTIFIER)
        self._name = name


class KeyToken(Token):

    def get_key(self):
        return self._key

    def __init__(self, tokenizer, key_name):
        super().__init__(tokenizer, TokenType.KEY)
        self._key = datatypes.Key(key_name)


class LabelToken(Token):

    def get_key(self):
        return self._key

    def __init__(self, tokenizer, key_name):
        super().__init__(tokenizer, TokenType.LABEL)
        self._key = datatypes.Key(key_name)


class Tokenizer:

    def isspecial(self, c):
        return str.isspace(c) or (c in '()[]{}.,:;|=\'\"#')

    def slurp_whitespace(self):
        while str.isspace(self._s.peek()):
            self._s.read()

    def parse_quoted_str(self, end):
        the_str = ''
        c = self._s.read()
        while (c != end) and (c != ''):
            if c == '\\':
                pass
            else:
                the_str += c
            c = self._s.read()
        return the_str
    
    def read(self):
        if self._pushbacks:
            return self._pushbacks.pop()

        self.slurp_whitespace()

        c = self._s.read()

        if c == '': return Token(self, TokenType.EOF)
        elif c == '(': return Token(self, TokenType.LPAREN)
        elif c == ')': return Token(self, TokenType.RPAREN)
        elif c == '[': return Token(self, TokenType.LBRACK)
        elif c == ']': return Token(self, TokenType.RBRACK)
        elif c == '{': return Token(self, TokenType.LCURLY)
        elif c == '}': return Token(self, TokenType.RCURLY)
        elif c == ':': return Token(self, TokenType.COLON)
        elif c == '.': return Token(self, TokenType.SEPARATOR)
        elif c == ',': return Token(self, TokenType.SEPARATOR)
        elif c == ';': return Token(self, TokenType.SEPARATOR)
        elif c == '=':
            if self._s.peek() == '>':
                self._s.read()
                return Token(self, TokenType.RARROW)
            else:
                return Token(self, TokenType.EQUALS)
        elif c == '^': return Token(self, TokenType.CARET)
        elif c == '|': return Token(self, TokenType.BAR)
        elif c == '\'': return KeyToken(self, self.parse_quoted_str('\''))
        elif c == '\"':
            self.parse_quoted_str('\"')
            return self.read()
        elif c == '#':
            c = self._s.read()
            if c == '<':
                return LabelToken(self, self.parse_quoted_str('>'))
            else:
                raise Exception('Unknown macro char!')

        while not self.isspecial(self._s.peek()):
            c += self._s.read()

        if c == 'self': return Token(self, TokenType.SELF)
        elif c == 'with': return Token(self, TokenType.WITH)
        elif c == 'private': return Token(self, TokenType.PRIVATE)
        else: return IdentifierToken(self, c)

    def unread(self, token):
        self._pushbacks.append(token)

    #def check(self, token_type):
    #    return self.read().check(token_type)

    #def check_identifier(self):
    #    return self.check(TokenType.IDENTIFIER)

    #def check_key(self):
    #    return self.check(TokenType.KEY)

    #def expect(self, token_type):
    #    return self.read().expect(token_type)
    
    #def expect_identifier(self):
    #    return self.expect(TokenType.IDENTIFIER)

    def __init__(self, stream):
        self._s = stream
        self._pushbacks = []
