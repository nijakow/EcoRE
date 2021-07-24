
import enum

import datatypes
import parser.ast


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
    
    def get_parser(self):
        return Parser(Tokenizer(self))

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
            raise Exception('Oof, expected ' + str(token_type) + ', got ' + str(self))
        return self

    def __init__(self, tokenizer, token_type):
        self._tokenizer = tokenizer
        self._token_type = token_type


class IdentifierToken(Token):

    def __repr__(self):
        return '<IdentifierToken:' + str(self._name) + '>'

    def get_name(self):
        return self._name
    
    def get_key(self):
        return datatypes.Key.get(self.get_name())

    def __init__(self, tokenizer, name):
        super().__init__(tokenizer, TokenType.IDENTIFIER)
        self._name = name


class Tokenizer:

    def isspecial(self, c):
        return str.isspace(c) or (c in '()[]{}.,:;|=')

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

        while not self.isspecial(self._s.peek()):
            c += self._s.read()

        if c == 'self': return Token(self, TokenType.SELF)
        elif c == 'with': return Token(self, TokenType.WITH)
        elif c == 'private': return Token(self, TokenType.PRIVATE)
        else: return IdentifierToken(self, c)

    def unread(self, token):
        self._pushbacks.append(token)

    def check(self, token_type):
        return self.read().check(token_type)

    def check_identifier(self):
        return self.check(TokenType.IDENTIFIER)

    def expect(self, token_type):
        return self.read().expect(token_type)
    
    def expect_identifier(self):
        return self.expect(TokenType.IDENTIFIER)

    def __init__(self, stream):
        self._s = stream
        self._pushbacks = []


class Parser:

    def parse_closure(self):
        params = []
        exprs = []
        if not self._t.check(TokenType.RBRACK):
            exprs.append(self.parse_expression())
            while True:
                if self._t.check(TokenType.RARROW):
                    for e in exprs:
                        params.append(e)
                    exprs.clear()
                    exprs.append(self.parse_expression())
                elif self._t.check(TokenType.SEPARATOR):
                    exprs.append(self.parse_expression())
                else:
                    self._t.expect(TokenType.RBRACK)
                    break
        return parser.ast.ASTBlock(params, exprs)

    def parse_object_slot_header(self):
        key = self._t.expect_identifier().get_key()
        params = []
        if self._t.check(TokenType.COLON):
            key = key.colonize()
            params.append(self._t.expect_identifier().get_key())
            t = self._t.read()
            while t.check(TokenType.IDENTIFIER):
                self._t.expect(TokenType.COLON)
                key = key.extend_name(t.get_key().colonize())
                params.append(self._t.expect_identifier().get_key())
                t = self._t.read()
        elif self._t.check(TokenType.LPAREN):
            if not self._t.check(TokenType.RPAREN):
                self.params.append(self._t.expect_identifier())
                while not self._t.check(TokenType.RPAREN):
                    self._t.expect(TokenType.SEPARATOR)
                    self.params.append(self._t.expect_identifier())
        return key, params

    def parse_object(self):
        object = datatypes.PlainObject()
        while not self._t.check(TokenType.RCURLY):
            if self._t.check(TokenType.WITH):
                flag_inherited = True
            else:
                flag_inherited = False
            if self._t.check(TokenType.PRIVATE):
                flag_private = True
            else:
                flag_private = False
            key, params = self.parse_object_slot_header()
            if self._t.check(TokenType.SEPARATOR):
                object.add_slot(datatypes.ValueSlot(key, parser.ast.ASTNil(), is_inherited=flag_inherited, is_private=flag_private))
                continue
            elif self._t.check(TokenType.RCURLY):
                object.add_slot(datatypes.ValueSlot(key, parser.ast.ASTNil(), is_inherited=flag_inherited, is_private=flag_private))
                break
            elif self._t.check(TokenType.RARROW):
                flag_method = True
                # TODO: Check against flag_inherited (must be false)
            else:
                self._t.expect(TokenType.EQUALS)
                flag_method = False
                # TODO: len(params) must be 0!
            value = self.parse_expression()
            if flag_method:
                code = self.parse_expression().compile_as_code()  # TODO
                object.add_slot(datatypes.CodeSlot(key, code, is_private=flag_private))
            else:
                object.add_slot(datatypes.ValueSlot(key, self.parse_expression(), is_inherited=flag_inherited, is_private=flag_private))
            self._t.check(TokenType.SEPARATOR)
        return parser.ast.ASTConstant(object)

    def parse_simple_expression(self, allow_followups=True):
        if self._t.check(TokenType.SELF):
            return parser.ast.ASTSelf()
        elif self._t.check(TokenType.LPAREN):
            exprs = self.parse_expression_list(TokenType.RPAREN)
            if len(exprs) == 1:
                return exprs[0]
            else:
                return parser.ast.ASTCompound(exprs)
        elif self._t.check(TokenType.LBRACK):
            return self.parse_closure()
        elif self._t.check(TokenType.LCURLY):
            return self.parse_object()
        elif self._t.check(TokenType.CARET):
            return parser.ast.ASTReturn(self.parse_expression(allow_followups))
        elif self._t.check(TokenType.BAR):
            # TODO: Parse variable list
            self._t.expect(TokenType.BAR)
            return parser.ast.ASTVariable()
        else:
            return parser.ast.ASTSelf()

    def parse_send(self, the_ast, allow_followups=True):
        ident = self._t.check_identifier()
        if ident:
            params = []
            key = ident.get_key()
            if key.is_binary_op():
                params.append(self.parse_expression(allow_followups))
            elif self._t.check(TokenType.LPAREN):
                for arg in self.parse_expression_list(TokenType.RPAREN):
                    params.append(arg)
            else:
                colon = self._t.read()
                if colon.check(TokenType.COLON):
                    if not allow_followups:
                        colon.fail()
                        ident.fail()
                        return the_ast
                    key = key.colonize()
                    params.append(self.parse_expression(False))
                    while True:
                        ident = self._t.check_identifier()
                        if not ident:
                            break
                        colon = self._t.read()
                        if not colon.check(TokenType.COLON):
                            ident.fail()
                            colon.fail()
                            break
                        key = key.extend_name(ident.get_key().colonize())
                        params.append(self.parse_expression(False))
            the_ast = parser.ast.ASTSend(the_ast, key, params)
        return the_ast

    def parse_expression(self, allow_followups=True):
        the_ast = self.parse_simple_expression(allow_followups)
        prev = None
        while the_ast != prev:
            prev = the_ast
            the_ast = self.parse_send(the_ast, allow_followups)
            if self._t.check(TokenType.LPAREN):
                the_ast = parser.ast.ASTSend(the_ast, datatypes.Key.get('value'), self.parse_expression_list(TokenType.RPAREN))
            elif self._t.check(TokenType.EQUALS):
                the_ast = parser.ast.ASTAssignment(the_ast, self.parse_expression(allow_followups))
        return the_ast

    def parse_expression_list(self, terminator):
        l = []
        if not self._t.check(terminator):
            while True:
                l.append(self.parse_expression())
                if self._t.check(terminator): break
                else:
                    self._t.expect(TokenType.SEPARATOR)
                    if self._t.check(terminator):
                        break
        return l

    def __init__(self, tokenizer):
        self._t = tokenizer
