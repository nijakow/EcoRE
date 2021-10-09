import ecosphere.objects.misc
import ecosphere.parser.tokenizer
from ecosphere.parser.tokenizer import TokenType

from ecosphere.parser.ast import ASTExpression, ASTNumber, ASTSelf, ASTGroup, ASTObject, ASTPlainObject, ASTSlot, ASTNumber, ASTKey, ASTBuiltin, ASTSend, ASTCompound, ASTBlock, ASTVar, ASTAssignment, ASTReturn, ASTLabelDef, ASTLabelRef


class ParseException(Exception):

    def __init__(self, msg: str, token: ecosphere.parser.tokenizer.Token):
        super().__init__(msg)


class Parser:

    def _is_bin(self, c):
        return not (c.isalnum() or c == '_')

    def parse_compound(self):
        return ASTCompound(self.parse_expressions(TokenType.RPAREN))
    
    def parse_block(self):
        parameters = []
        instructions = []
        varargs = False
        while True:
            if self.check(TokenType.COLON):
                parameters.append(ecosphere.objects.misc.EcoKey.Get(self.expect(TokenType.IDENTIFIER).get_text()))
                if not self.check(TokenType.RARROW):
                    self.expect(TokenType.SEPARATOR)
                continue
            elif self.check(TokenType.ELLIPSIS):
                varargs = True
                self.expect(TokenType.RARROW)
            break
        return ASTBlock(parameters, varargs, ASTCompound(self.parse_expressions(TokenType.RBRACK)))
    
    def parse_group(self):
        return ASTGroup(self.parse_expressions(TokenType.RCURLY))
    
    def parse_optional_type(self):
        the_type = None
        if self.check(TokenType.LBRACK):
            the_type = self.parse_expression()
            self.expect(TokenType.RBRACK)
        return the_type

    def parse_object_slot(self):
        flags = set()
        while True:
            if self.check(TokenType.WITH):
                flags.add('inherited')
            elif self.check(TokenType.OWN):
                flags.add('own')
            else:
                break
        name = ''
        args = []
        the_type = self.parse_optional_type()
        kw = self.expect(TokenType.IDENTIFIER)
        while kw:
            name += kw.get_text()
            if self._is_bin(name[-1]):
                arg_type = self.parse_optional_type()
                args.append((ecosphere.objects.misc.EcoKey.Get(self.expect(TokenType.IDENTIFIER).get_text()), arg_type))
            if name[-1] != ':':
                break
            kw = self.check(TokenType.IDENTIFIER)
        if self.check(TokenType.LPAREN):
            if not self.check(TokenType.RPAREN):
                while True:
                    if self.check(TokenType.ELLIPSIS):
                        flags.add('varargs')
                        self.expect(TokenType.RPAREN)
                        break
                    arg_type = self.parse_optional_type()
                    args.append((ecosphere.objects.misc.EcoKey.Get(self.expect(TokenType.IDENTIFIER).get_text()), arg_type))
                    if self.check(TokenType.RPAREN):
                        break
                    self.expect(TokenType.SEPARATOR)
        if self.check(TokenType.RARROW):
            flags.add('code')
        else:
            self.expect(TokenType.ASSIGNMENT)
        value = self.parse_expression()
        return ASTSlot(ecosphere.objects.misc.EcoKey.Get(name), the_type, args, flags, value)

    def parse_object(self):
        slots = []
        while not self.check(TokenType.RCURLY):
            slots.append(self.parse_object_slot())
            if self.check(TokenType.RCURLY): break
            self.expect(TokenType.SEPARATOR)
        return ASTPlainObject(slots)

    def parse_simple_expression(self, allow_followups=True) -> ASTExpression:
        if self.check(TokenType.SELF):
            return ASTSelf()
        elif self.check(TokenType.LPAREN):
            return self.parse_compound()
        elif self.check(TokenType.LBRACK):
            return self.parse_block()
        elif self.check(TokenType.LCURLY):
            if self.check(TokenType.RARROW):
                return self.parse_group()
            else:
                return self.parse_object()
        kw = self._t.read()
        if kw.is_a(TokenType.KEY):
            if allow_followups and self.check(TokenType.LPAREN):
                args, varargs = self.parse_arglist()
                return ASTBuiltin(kw.get_key(), args, varargs)
            else:
                return ASTKey(kw.get_key())
        elif kw.is_a(TokenType.NUMBER):
            return ASTNumber(kw.get_value())
        elif kw.is_a(TokenType.LABEL):
            if kw.is_definition():
                return ASTLabelDef(kw.get_address(), self.parse_simple_expression())
            else:
                return ASTLabelRef(kw.get_address())
        # TODO: Strings
        else:
            kw.fail()
            return ASTSelf()
    
    def parse_send(self, ast: ASTExpression, allow_followups: bool) -> ASTExpression:
        name = ''
        args = []
        varargs = False
        kw = self.check(TokenType.IDENTIFIER)
        if not kw:
            return ast
        while kw:
            name += kw.get_text()
            if self._is_bin(name[-1]):
                if not allow_followups:
                    kw.fail()
                    return ast
                else:
                    args.append(self.parse_expression(False))
            if name[-1] != ':':
                break
            kw = self.check(TokenType.IDENTIFIER)
        if allow_followups and self.check(TokenType.LPAREN):
            par_args, varargs = self.parse_arglist()
            for e in par_args:
                args.append(e)
        return ASTSend(ast, ecosphere.objects.misc.EcoKey.Get(name), args, varargs)

    def parse_var(self):
        varname = self.expect(TokenType.IDENTIFIER).get_key()
        if self.check(TokenType.ASSIGNMENT):
            value = self.parse_expression()
        else:
            value = ASTSelf()
        self.expect(TokenType.BAR)
        return ASTVar(varname, value, self.parse_expression())

    def parse_expression(self, allow_followups:bool=True) -> ASTExpression:
        if allow_followups and self.check(TokenType.CARET):
            return ASTReturn(self.parse_expression(allow_followups))
        elif allow_followups and self.check(TokenType.BAR):
            return self.parse_var()
        ast = self.parse_simple_expression(allow_followups)
        next = None
        while ast != next:
            next = ast
            ast = self.parse_send(ast, allow_followups)
            if allow_followups and self.check(TokenType.LPAREN):
                args, varargs = self.parse_arglist()
                ast = ASTSend(ast, ecosphere.objects.misc.EcoKey.Get('value'), args, varargs)
            elif self.check(TokenType.ASSIGNMENT):
                ast = ASTAssignment(ast, self.parse_expression(allow_followups))
        return ast

    def parse_expressions(self, terminator: TokenType) -> list:
        exprs = []
        while not self.check(terminator):
            exprs.append(self.parse_expression())
            if self.check(terminator): break
            self.expect(TokenType.SEPARATOR)
        return exprs
    
    def parse_arglist(self) -> list:
        exprs = []
        while not self.check(TokenType.RPAREN):
            if self.check(TokenType.ELLIPSIS):
                self.expect(TokenType.RPAREN)
                return exprs, True
            exprs.append(self.parse_expression())
            if self.check(TokenType.RPAREN): break
            self.expect(TokenType.SEPARATOR)
        return exprs, False

    def check(self, token_type: TokenType):
        t = self._t.read()
        if t.is_a(token_type):
            return t
        else:
            t.fail()
            return False
    
    def expect(self, token_type: TokenType):
        t = self._t.read()
        if t.is_a(token_type):
            return t
        else:
            t.fail()
            raise ParseException('Expected a token type \'' + str(token_type) + '\'!', t)

    def __init__(self, tokenizer: ecosphere.parser.tokenizer.Tokenizer):
        self._t = tokenizer
