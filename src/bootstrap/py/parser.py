
import enum

import datatypes
import ast


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


class TokenType(enum.Enum):
    EOF        = enum.auto()
    IDENTIFIER = enum.auto()
    LPAREN     = enum.auto()
    RPAREN     = enum.auto()
    LBRACK     = enum.auto()
    RBRACK     = enum.auto()
    LCURLY     = enum.auto()
    RCURLY     = enum.auto()
    SEPARATOR  = enum.auto()
    EQUALS     = enum.auto()
    CARET      = enum.auto()
    BAR        = enum.auto()
    SELF       = enum.auto()
    WITH       = enum.auto()
    METHOD     = enum.auto()


class Token:

    def __repr__(self):
        return '<Token:' + str(self._token_type) + '>'

    def fail(self):
        self._tokenizer.unread(self)

    def is_type(self, token_type):
        return self._token_type == token_type

    def check(self, token_type):
        if self.is_type(token_type):
            return self
        else:
            self.fail()
            return False

    def expect(self, token_type):
        if not self.is_type(token_type):
            raise Exception('Oof, expected different type')

    def __init__(self, tokenizer, token_type):
        self._tokenizer = tokenizer
        self._token_type = token_type


class IdentifierToken(Token):

    def __repr__(self):
        return '<IdentifierToken:' + str(self._name) + '>'

    def get_name(self):
        return self._name

    def __init__(self, tokenizer, name):
        super().__init__(tokenizer, TokenType.IDENTIFIER)
        self._name = name


class Tokenizer:

    def isspecial(self, c):
        return str.isspace(c) or (c in '()[]{}.,;|=')

    def slurp_whitespace(self):
        while str.isspace(self._s.peek()):
            self._s.read()

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
        elif c == '.': return Token(self, TokenType.SEPARATOR)
        elif c == ',': return Token(self, TokenType.SEPARATOR)
        elif c == ';': return Token(self, TokenType.SEPARATOR)
        elif c == '=': return Token(self, TokenType.EQUALS)
        elif c == '^': return Token(self, TokenType.CARET)
        elif c == '|': return Token(self, TokenType.BAR)

        while not self.isspecial(self._s.peek()):
            c += self._s.read()

        if c == 'self': return Token(self, TokenType.SELF)
        elif c == 'with': return Token(self, TokenType.WITH)
        elif c == 'method': return Token(self, TokenType.METHOD)
        else: return IdentifierToken(self, c)

    def unread(self, token):
        self._pushbacks.append(token)

    def check(self, token_type):
        return self.read().check(token_type)

    def check_identifier(self):
        return self.check(TokenType.IDENTIFIER)

    def expect(self, token_type):
        self.read().expect(token_type)

    def __init__(self, stream):
        self._s = stream
        self._pushbacks = []


class Parser:

    def parse_simple_expression(self, allow_followups=True):
        if self._t.check(TokenType.SELF):
            return ast.ASTSelf()
        elif self._t.check(TokenType.LPAREN):
            return ast.ASTSequence(self.parse_expression_list(TokenType.RPAREN))
        elif self._t.check(TokenType.LBRACK):
            # TODO: Parse argument list
            return ast.ASTBlock([], self.parse_expression_list(TokenType.RBRACK))
        elif self._t.check(TokenType.LCURLY):
            # TODO: Parse object
            return ast.ASTObject()
        elif self._t.check(TokenType.CARET):
            return ast.ASTReturn(self.parse_expression(allow_followups))
        elif self._t.check(TokenType.BAR):
            # TODO: Parse variable list
            self._t.expect(TokenType.BAR)
            return ast.ASTVariable()
        else:
            return ast.ASTSelf()

    def parse_send(self, the_ast, allow_followups=True):
        ident = self._t.check_identifier()
        if ident:
            params = []
            key = ident.get_key()
            if key.takes_parameters():
                if key.has_followup() and not allow_followups:
                    ident.fail()
                else:
                    funcname = key
                    params.append(self.parse_expression(False))
                    while key.has_followup():
                        ident = self._t.check_identifier()
                        if not ident: break
                        key = ident.get_key()
                        funcname = funcname.add_followup(key)
                        params.append(self.parse_expression(False))
            if self._t.check(TokenType.LPAREN):
                for arg in self.parse_expression_list(TokenType.RPAREN):
                    params.append(arg)
            the_ast = ast.ASTSend(the_ast, key, params)
        return the_ast

    def parse_expression(self, allow_followups=True):
        the_ast = self.parse_simple_expression(allow_followups)
        prev = None
        while the_ast != prev:
            prev = the_ast
            the_ast = self.parse_send(the_ast, allow_followups)
            if self._t.check(TokenType.LPAREN):
                the_ast = ast.ASTSend(the_ast, datatypes.Key.get('value'), self.parse_expression_list(TokenType.RPAREN))
            elif self._t.check(TokenType.EQUALS):
                the_ast = ast.ASTAssignment(the_ast, self.parse_expression(allow_followups))
            # TODO: Assignments
        return the_ast

    def parse_expression_list(self, terminator):
        l = []
        if not self._t.check(terminator):
            while True:
                l.append(self.parse_expression())
                if self._t.check(terminator): break
                else: self._t.expect(TokenType.SEPARATOR)
        return l

    def __init__(self, tokenizer):
        self._t = tokenizer
