import enum
import ecosphere.objects.misc.EcoKey


class TokenType(enum.Enum):
    EOF = enum.auto()
    IDENTIFIER = enum.auto()
    LPAREN = enum.auto()
    RPAREN = enum.auto()
    LBRACK = enum.auto()
    RBRACK = enum.auto()
    LCURLY = enum.auto()
    RCURLY = enum.auto()
    SEPARATOR = enum.auto()
    COLON = enum.auto()
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

    def get_key(self) -> ecosphere.objects.misc.EcoKey:
        return self._key

    def __init__(self, tokenizer: 'Tokenizer', name: str):
        super().__init__(tokenizer, TokenType.IDENTIFIER)
        self._key = ecosphere.objects.misc.EcoKey.Get(name)


class Tokenizer:

    def _isspecial(self, c):
        return (c is None) or (c in '()[]\{\}.,;:|')

    def _slurp_whitespace(self):
        while self._s.peek().isspace():
            self._s.read()

    def unread(self, token: Token):
        self._pushbacks.append(token)
    
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
        elif self._s.peeks('{'): return Token(self, TokenType.LCURLY)
        elif self._s.peeks('}'): return Token(self, TokenType.RCURLY)
        elif self._s.peeks('.'): return Token(self, TokenType.SEPARATOR)
        elif self._s.peeks(','): return Token(self, TokenType.SEPARATOR)
        elif self._s.peeks(';'): return Token(self, TokenType.SEPARATOR)
        elif self._s.peeks(':'): return Token(self, TokenType.COLON)
        elif self._s.peeks('|'): return Token(self, TokenType.BAR)

        c = ''

        while not self.isspecial(self._s.peek()):
            c += self._s.read()
        
        if c == 'self': return Token(self, TokenType.SELF)
        elif c == 'with': return Token(self, TokenType.WITH)
        elif c == 'with*': return Token(self, TokenType.WITHSTAR)
        else: return IdentifierToken(self, c)

    def __init__(self, stream):
        self._s = stream
        self._pushbacks = list()
