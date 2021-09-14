import ecosphere.parser.tokenizer
from ecosphere.parser.tokenizer import TokenType


class ParseException(Exception):

    def __init__(self, msg: str, token: ecosphere.parser.tokenizer.Token):
        super().__init__(msg)


class Parser:

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


    def parse_send(self, ast: ASTExpression, allow_followups:bool=True) -> ASTExpression:
        return ast

    def parse_expression(self, allow_followups:bool=True) -> ASTExpression:
        ast = self.parse_simple_expression()
        next = None
        while ast != next:
            ast = self.parse_send(ast, allow_followups)
        return ast

    def parse_expressions(self, terminator: TokenType) -> list:
        exprs = []
        if self.check(terminator):
            return exprs
        while True:
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
            raise ParseException('Expected a different token type!', t)

    def __init__(self, tokenizer: ecosphere.parser.tokenizer.Tokenizer):
        self._t = tokenizer
