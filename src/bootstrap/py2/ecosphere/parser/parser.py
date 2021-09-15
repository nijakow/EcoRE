import ecosphere.objects.misc
import ecosphere.parser.tokenizer
from ecosphere.parser.tokenizer import TokenType

from ecosphere.parser.ast import ASTExpression, ASTSelf, ASTGroup, ASTObject, ASTSlot, ASTSend, ASTCompound, ASTBlock, ASTVar, ASTAssignment, ASTReturn


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
        if not self.check(TokenType.RBRACK):
            while True:
                instructions.append(self.parse_expression())
                if self.check(TokenType.RARROW):
                    parameters = instructions
                    instructions = self.parse_expressions(TokenType.RBRACK)
                    break
                elif self.check(TokenType.RBRACK):
                    break
                self.expect(TokenType.SEPARATOR)
        return ASTBlock(parameters, ASTCompound(instructions))
    
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
            elif self.check(TokenType.WITHSTAR):
                flags.add('inherited')
                flags.add('subpart')
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
        return ASTObject(slots)

    def parse_simple_expression(self) -> ASTExpression:
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
        else:
            return ASTSelf()
    
    def parse_send(self, ast: ASTExpression, allow_followups: bool) -> ASTExpression:
        name = ''
        args = []
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
        if self.check(TokenType.LPAREN):
            for e in self.parse_expressions(TokenType.RPAREN):
                args.append(e)
        return ASTSend(ast, ecosphere.objects.misc.EcoKey.Get(name), args)

    def parse_expression(self, allow_followups:bool=True) -> ASTExpression:
        if allow_followups and self.check(TokenType.CARET):
            return ASTReturn(self.parse_expression(allow_followups))
        elif allow_followups and self.check(TokenType.BAR):
            return ASTVar(self.parse_expressions(TokenType.BAR), self.parse_expression())
        ast = self.parse_simple_expression()
        next = None
        while ast != next:
            next = ast
            ast = self.parse_send(ast, allow_followups)
            if self.check(TokenType.LPAREN):
                ast = ASTSend(ast, ecosphere.objects.misc.EcoKey.Get('value'), self.parse_expressions(TokenType.RPAREN))
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
