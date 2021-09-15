import enum

import ecosphere.objects.misc


class TokenType(enum.Enum):
    EOF = enum.auto()
    IDENTIFIER = enum.auto()
    STRING = enum.auto()
    KEY = enum.auto()
    LPAREN = enum.auto()
    RPAREN = enum.auto()
    LBRACK = enum.auto()
    RBRACK = enum.auto()
    LCURLY = enum.auto()
    RCURLY = enum.auto()
    SEPARATOR = enum.auto()
    CARET = enum.auto()
    BAR = enum.auto()
    ASSIGNMENT = enum.auto()
    RARROW = enum.auto()
    ELLIPSIS = enum.auto()
    SELF = enum.auto()
    WITH = enum.auto()
    WITHSTAR = enum.auto()


class Token:

    def is_a(self, token_type: TokenType) -> bool:
        return self._type == token_type

    def fail(self):
        self._t.unread(self)

    def __init__(self, tokenizer: 'Tokenizer', token_type: TokenType):
        self._t = tokenizer
        self._type = token_type

class IdentifierToken(Token):

    def get_text(self) -> str:
        return self._text

    def __init__(self, tokenizer: 'Tokenizer', name: str):
        super().__init__(tokenizer, TokenType.IDENTIFIER)
        self._text = name

class StringToken(Token):

    def get_string(self) -> str:
        return self._str

    def __init__(self, tokenizer: 'Tokenizer', the_str: str):
        super().__init__(tokenizer, TokenType.STRING)
        self._str = the_str

class KeyToken(Token):

    def get_key(self) -> ecosphere.objects.misc.EcoKey:
        return self._key

    def __init__(self, tokenizer: 'Tokenizer', key: ecosphere.objects.misc.EcoKey):
        super().__init__(tokenizer, TokenType.KEY)
        self._key = key


class Tokenizer:

    def _isspecial(self, c):
        return (c is None) or (c in '()[]\{\}.,;|^') or (c.isspace())

    def _slurp_whitespace(self):
        while self._s.has() and self._s.peek().isspace():
            self._s.read()
    
    def parse_string(self, end) -> str:
        s = ''
        while (self._s.has() and self._s.peek() != end):
            c = self._s.read()
            if c == '\\':
                e = self._s.read()
                if e == '\\': c = '\\'
                elif e == 'n': c = '\n'
                elif e == 't': c = '\t'
                # TODO: More
                elif e == end: c = end
                else: c += e
            s += c
        self._s.read()
        return s

    def read(self) -> Token:
        if self._pushbacks:
            return self._pushbacks.pop()
        self._slurp_whitespace()
        
        if self._s.peeks('...'): return Token(self, TokenType.ELLIPSIS)
        elif self._s.peeks('<-'): return Token(self, TokenType.ASSIGNMENT)
        elif self._s.peeks('=>'): return Token(self, TokenType.RARROW)
        elif self._s.peeks('('): return Token(self, TokenType.LPAREN)
        elif self._s.peeks('('): return Token(self, TokenType.LPAREN)
        elif self._s.peeks(')'): return Token(self, TokenType.RPAREN)
        elif self._s.peeks('['): return Token(self, TokenType.LBRACK)
        elif self._s.peeks(']'): return Token(self, TokenType.RBRACK)
        elif self._s.peeks('\{'): return Token(self, TokenType.LCURLY)
        elif self._s.peeks('\}'): return Token(self, TokenType.RCURLY)
        elif self._s.peeks('.'): return Token(self, TokenType.SEPARATOR)
        elif self._s.peeks(','): return Token(self, TokenType.SEPARATOR)
        elif self._s.peeks(';'): return Token(self, TokenType.SEPARATOR)
        elif self._s.peeks('|'): return Token(self, TokenType.BAR)
        elif self._s.peeks('^'): return Token(self, TokenType.CARET)
        elif self._s.peeks('\''): return StringToken(self, self.parse_string('\''))
        elif self._s.peeks('#\''): return KeyToken(self, ecosphere.objects.misc.EcoKey.Get(self.parse_string('\'')))
        elif self._s.peeks('\"'): self.parse_string('\"'); return self.read()  # Comment

        c = ''

        while self._s.has() and not self._isspecial(self._s.peek()):
            c += self._s.read()
        
        if c == '': return Token(self, TokenType.EOF)
        elif c == 'self': return Token(self, TokenType.SELF)
        elif c == 'with': return Token(self, TokenType.WITH)
        elif c == 'with*': return Token(self, TokenType.WITHSTAR)
        elif c == '=': return Token(self, TokenType.ASSIGNMENT)
        else: return IdentifierToken(self, c)
    
    def unread(self, token: Token):
        self._pushbacks.append(token)

    def __init__(self, stream):
        self._s = stream
        self._pushbacks = list()
