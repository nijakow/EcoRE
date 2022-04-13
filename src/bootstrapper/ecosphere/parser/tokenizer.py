import enum

import ecosphere.objects.misc


class TokenType(enum.Enum):
    EOF = enum.auto()
    IDENTIFIER = enum.auto()
    STRING = enum.auto()
    KEY = enum.auto()
    NUMBER = enum.auto()
    CHARACTER = enum.auto()
    LABEL = enum.auto()
    PROXY = enum.auto()
    LPAREN = enum.auto()
    RPAREN = enum.auto()
    LBRACK = enum.auto()
    RBRACK = enum.auto()
    LCURLY = enum.auto()
    RCURLY = enum.auto()
    HASHLPAREN = enum.auto()
    PERCLBRACK = enum.auto()
    SEPARATOR = enum.auto()
    COLON = enum.auto()
    TILDE = enum.auto()
    CARET = enum.auto()
    BAR = enum.auto()
    ASSIGNMENT = enum.auto()
    RARROW = enum.auto()
    ELLIPSIS = enum.auto()
    SELF = enum.auto()
    IS = enum.auto()
    AS = enum.auto()
    WITH = enum.auto()
    INHERITED = enum.auto()
    PRIVATE = enum.auto()
    FINAL = enum.auto()
    OWN = enum.auto()


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
    
    def get_key(self):
        return ecosphere.objects.misc.EcoKey.Get(self.get_text())

    def __init__(self, tokenizer: 'Tokenizer', name: str):
        super().__init__(tokenizer, TokenType.IDENTIFIER)
        self._text = name

class StringToken(Token):

    def get_string(self) -> ecosphere.objects.misc.EcoString:
        return ecosphere.objects.misc.EcoString(self._str)

    def __init__(self, tokenizer: 'Tokenizer', the_str: str):
        super().__init__(tokenizer, TokenType.STRING)
        self._str = the_str

class KeyToken(Token):

    def get_key(self) -> ecosphere.objects.misc.EcoKey:
        return self._key

    def __init__(self, tokenizer: 'Tokenizer', key: ecosphere.objects.misc.EcoKey):
        super().__init__(tokenizer, TokenType.KEY)
        self._key = key

class StringToken(Token):

    def get_string(self) -> ecosphere.objects.misc.EcoString:
        return ecosphere.objects.misc.EcoString(self._string)
    
    def __init__(self, tokenizer: 'Tokenizer', string: ecosphere.objects.misc.EcoString):
        super().__init__(tokenizer, TokenType.STRING)
        self._string = string

class NumberToken(Token):

    def get_value(self):
        return self._value
    
    def __init__(self, tokenizer: 'Tokenizer', value):
        super().__init__(tokenizer, TokenType.NUMBER)
        self._value = value

class CharacterToken(Token):

    def get_value(self):
        return self._value
    
    def __init__(self, tokenizer: 'Tokenizer', value):
        super().__init__(tokenizer, TokenType.CHARACTER)
        self._value = value

class LabelToken(Token):

    def get_address(self):
        return self._target

    def __init__(self, tokenizer: 'Tokenizer', target: str):
        super().__init__(tokenizer, TokenType.LABEL)
        self._target = target

class ProxyToken(Token):

    def get_proxy_type(self):
        return self._proxy_type
    
    def get_proxy_target(self):
        return self._target

    def __init__(self, tokenizer: 'Tokenizer', target: str):
        super().__init__(tokenizer, TokenType.PROXY)
        index = target.find(':')
        if index >= 0:
            self._proxy_type = target[:index]
            self._target = target[index+1:]
        else:
            self._proxy_type = ''
            self._target = target


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
                elif e == 'r': c = '\r'
                elif e == 'e': c = '\033'
                # TODO: More
                elif e == end: c = end
                else: c += e
            s += c
        self._s.read()
        return s
    
    def parse_nested_comment(self):
        nesting = 1
        while nesting:
            if not self._s.has():
                break
            elif self._s.peeks('/\"'):
                nesting += 1
            elif self._s.peeks("\"/"):
                nesting -= 1
            else:
                self._s.read()

    def read(self) -> Token:
        if self._pushbacks:
            return self._pushbacks.pop()
        self._slurp_whitespace()
        
        if self._s.peeks('...'): return Token(self, TokenType.ELLIPSIS)
        elif self._s.peeks('<-'): return Token(self, TokenType.ASSIGNMENT)
        elif self._s.peeks('=>'): return Token(self, TokenType.RARROW)
        elif self._s.peeks('('): return Token(self, TokenType.LPAREN)
        elif self._s.peeks(')'): return Token(self, TokenType.RPAREN)
        elif self._s.peeks('['): return Token(self, TokenType.LBRACK)
        elif self._s.peeks(']'): return Token(self, TokenType.RBRACK)
        elif self._s.peeks('{'): return Token(self, TokenType.LCURLY)
        elif self._s.peeks('}'): return Token(self, TokenType.RCURLY)
        elif self._s.peeks('#('): return Token(self, TokenType.HASHLPAREN)
        elif self._s.peeks('%['): return Token(self, TokenType.PERCLBRACK)
        elif self._s.peeks('.'): return Token(self, TokenType.SEPARATOR)
        elif self._s.peeks(','): return Token(self, TokenType.SEPARATOR)
        elif self._s.peeks(';'): return Token(self, TokenType.SEPARATOR)
        elif self._s.peeks(':'): return Token(self, TokenType.COLON)
        elif self._s.peeks('|'): return Token(self, TokenType.BAR)
        elif self._s.peeks('^'): return Token(self, TokenType.CARET)
        elif self._s.peeks('\''): return StringToken(self, self.parse_string('\''))
        elif self._s.peeks('#\''): return KeyToken(self, ecosphere.objects.misc.EcoKey.Get(self.parse_string('\'')))
        elif self._s.peeks('#='): return LabelToken(self, self.parse_string(':'))
        elif self._s.peeks('#<'): return ProxyToken(self, self.parse_string('>'))
        elif self._s.peeks('#\\'):
            c = self._s.read()
            return CharacterToken(self, ord(c))
        elif self._s.peeks('\"'): self.parse_string('\"'); return self.read()  # Comment
        elif self._s.peeks("/\""): self.parse_nested_comment(); return self.read() # Nested comment

        c = ''

        while self._s.has() and not self._isspecial(self._s.peek()):
            c += self._s.read()

        try:
            if c.startswith('0x'):
                return NumberToken(self, int(c, 16))
            elif c.startswith('0b'):
                return NumberToken(self, int(c, 2))
            else:
                return NumberToken(self, int(c))
        except:
            pass

        if c == '': return Token(self, TokenType.EOF)
        elif c == 'self': return Token(self, TokenType.SELF)
        elif c == 'is': return Token(self, TokenType.IS)
        elif c == 'as': return Token(self, TokenType.AS)
        elif c == 'with': return Token(self, TokenType.WITH)
        elif c == 'inherited': return Token(self, TokenType.INHERITED)
        elif c == 'private': return Token(self, TokenType.PRIVATE)
        elif c == 'final': return Token(self, TokenType.FINAL)
        elif c == 'own': return Token(self, TokenType.OWN)
        elif c == '=': return Token(self, TokenType.ASSIGNMENT)
        elif c == '~': return Token(self, TokenType.TILDE)
        else: return IdentifierToken(self, c)
    
    def unread(self, token: Token):
        self._pushbacks.append(token)

    def __init__(self, stream):
        self._s = stream
        self._pushbacks = list()
