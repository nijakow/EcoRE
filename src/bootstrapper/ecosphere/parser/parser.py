import ecosphere.objects.misc
import ecosphere.parser.tokenizer
from ecosphere.parser.tokenizer import TokenType

from ecosphere.parser.ast import ASTExpression, ASTNumber, ASTCharacter, ASTSelf, ASTGroup, ASTObject, ASTPlainObject, ASTSlot, ASTNumber, ASTKey, ASTString, ASTVector, ASTBuiltin, ASTSend, ASTVarAccess, ASTCompound, ASTBlock, ASTVar, ASTAssignment, ASTReturn, ASTLabelDef, ASTProxy


class ParseException(Exception):

    def __init__(self, msg: str, token: ecosphere.parser.tokenizer.Token):
        super().__init__(msg)


class ParseEnv:

    def subenvironment(self):
        return ParseEnv(self)

    def object_subenvironment(self):
        return ParseEnv()

    def bind(self, key):
        self._keys.add(key)
    
    def is_bound(self, key):
        if key in self._keys:
            return True
        elif self._parent is not None:
            return self._parent.is_bound(key)
        else:
            return False

    def __init__(self, parent=None):
        self._parent = parent
        self._keys = set()


class Parser:

    def _is_bin(self, c):
        return not (c.isalnum() or c == '_')

    def parse_compound(self, env, terminator=TokenType.RPAREN):
        return ASTCompound(self.parse_expressions(env.subenvironment(), terminator))
    
    def parse_block(self, env):
        parameters = []
        varargs = False
        while True:
            if self.check(TokenType.COLON):
                key = ecosphere.objects.misc.EcoKey.Get(self.expect(TokenType.IDENTIFIER).get_text())
                env.bind(key)
                parameters.append((key,))
                if not self.check(TokenType.RARROW) and not self.check(TokenType.BAR):
                    self.expect(TokenType.SEPARATOR)
            elif self.check(TokenType.ELLIPSIS):
                varargs = True
                if not self.check(TokenType.RARROW):
                    self.expect(TokenType.BAR)
            break
        return ASTBlock(parameters, varargs, self.parse_compound(env, TokenType.RBRACK))
    
    def parse_group(self, env):
        return ASTGroup(self.parse_expressions(env, TokenType.RCURLY))
    
    def parse_optional_type(self, env):
        the_type = None
        if self.check(TokenType.LBRACK):
            the_type = self.parse_expression(env)
            self.expect(TokenType.RBRACK)
        return the_type

    def parse_object_slot(self, env):
        flags = set()
        while True:
            if self.check(TokenType.WITH):
                flags.add('inherited')
            elif self.check(TokenType.OWN):
                flags.add('part')
            elif self.check(TokenType.WITHSTAR):
                flags.add('inherited')
                flags.add('part')
            else:
                break
        name = ''
        args = []
        the_type = self.parse_optional_type(env)
        kw = self.expect(TokenType.IDENTIFIER)
        while kw:
            name += kw.get_text()
            if self._is_bin(name[-1]):
                arg_type = self.parse_optional_type(env)
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
                    arg_type = self.parse_optional_type(env)
                    args.append((ecosphere.objects.misc.EcoKey.Get(self.expect(TokenType.IDENTIFIER).get_text()), arg_type))
                    if self.check(TokenType.RPAREN):
                        break
                    self.expect(TokenType.SEPARATOR)
        if self.check(TokenType.RARROW):
            flags.add('code')
            subenv = env.object_subenvironment()
            for p in args:
                subenv.bind(p[0])
            value = self.parse_expression(subenv)
        elif self.check(TokenType.ASSIGNMENT):
            value = self.parse_expression(env.object_subenvironment())
        else:
            value = None
        return ASTSlot(ecosphere.objects.misc.EcoKey.Get(name), the_type, args, flags, value)

    def parse_object(self, env):
        slots = []
        while not self.check(TokenType.RCURLY):
            slots.append(self.parse_object_slot(env))
            if self.check(TokenType.RCURLY): break
            self.expect(TokenType.SEPARATOR)
        return ASTPlainObject(slots)
    
    def parse_vector(self, env):
        return ASTVector(self.parse_expressions(env, TokenType.RPAREN))

    def parse_simple_expression(self, env, allow_followups=True) -> ASTExpression:
        if self.check(TokenType.SELF):
            return ASTSelf()
        elif self.check(TokenType.LPAREN):
            return self.parse_compound(env)
        elif self.check(TokenType.LBRACK):
            return self.parse_block(env)
        elif self.check(TokenType.LCURLY):
            return self.parse_object(env)
        elif self.check(TokenType.HASHLPAREN):
            return self.parse_vector(env)
        kw = self._t.read()
        if kw.is_a(TokenType.KEY):
            if self.check(TokenType.LPAREN):
                args, varargs = self.parse_arglist(env)
                return ASTBuiltin(kw.get_key(), args, varargs)
            else:
                return ASTKey(kw.get_key())
        elif kw.is_a(TokenType.STRING):
            return ASTString(kw.get_string())
        elif kw.is_a(TokenType.NUMBER):
            return ASTNumber(kw.get_value())
        elif kw.is_a(TokenType.CHARACTER):
            return ASTCharacter(kw.get_value())
        elif kw.is_a(TokenType.LABEL):
            return ASTLabelDef(kw.get_address(), self.parse_simple_expression(env))
        elif kw.is_a(TokenType.PROXY):
            return ASTProxy(kw.get_proxy_type(), kw.get_proxy_target())
        else:
            if kw.is_a(TokenType.IDENTIFIER):
                name = kw.get_text()
                key = ecosphere.objects.misc.EcoKey.Get(name)
                if not self._is_bin(name[-1]) and env.is_bound(key):
                    return ASTVarAccess(key)
            kw.fail()
            return ASTSelf()
    
    def parse_send(self, env, ast: ASTExpression, allow_followups: bool) -> ASTExpression:
        name = ''
        args = []
        varargs = False
        kw = self.check(TokenType.IDENTIFIER)
        bin = False
        if not kw:
            return ast
        while kw:
            name += kw.get_text()
            if self._is_bin(name[-1]):
                bin = True
                if not allow_followups:
                    kw.fail()
                    return ast
                else:
                    args.append(self.parse_expression(env, False))
            if name[-1] != ':':
                break
            kw = self.check(TokenType.IDENTIFIER)
        if (not bin) and self.check(TokenType.LPAREN):
            par_args, varargs = self.parse_arglist(env)
            for e in par_args:
                args.append(e)
        return ASTSend(ast, ecosphere.objects.misc.EcoKey.Get(name), args, varargs)

    def parse_var(self, env):
        varname = self.expect(TokenType.IDENTIFIER).get_key()
        env.bind(varname)
        if self.check(TokenType.ASSIGNMENT):
            value = self.parse_expression(env)
        else:
            value = ASTSelf()
        self.check(TokenType.SEPARATOR)
        if self.check(TokenType.BAR):
            return ASTVar(varname, value, self.parse_expression(env))
        else:
            return ASTVar(varname, value, self.parse_var(env))

    def parse_expression(self, env, allow_followups:bool=True) -> ASTExpression:
        if allow_followups and self.check(TokenType.CARET):
            return ASTReturn(self.parse_expression(env, allow_followups))
        elif allow_followups and self.check(TokenType.BAR):
            return self.parse_var(env)
        ast = self.parse_simple_expression(env, allow_followups)
        next = None
        while ast != next:
            next = ast
            ast = self.parse_send(env, ast, allow_followups)
            if self.check(TokenType.LPAREN):
                args, varargs = self.parse_arglist(env)
                ast = ASTSend(ast, ecosphere.objects.misc.EcoKey.Get('value'), args, varargs)
            elif self.check(TokenType.ASSIGNMENT):
                ast = ASTAssignment(ast, self.parse_expression(env, allow_followups))
        return ast

    def parse_expressions(self, env, terminator: TokenType) -> list:
        exprs = []
        while not self.check(terminator):
            exprs.append(self.parse_expression(env))
            if self.check(terminator): break
            self.expect(TokenType.SEPARATOR)
        return exprs
    
    def parse_arglist(self, env) -> list:
        exprs = []
        while not self.check(TokenType.RPAREN):
            if self.check(TokenType.ELLIPSIS):
                self.expect(TokenType.RPAREN)
                return exprs, True
            exprs.append(self.parse_expression(env))
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

    def parse(self, terminator=TokenType.EOF):
        env = ParseEnv()
        return self.parse_expressions(env, terminator)

    def __init__(self, tokenizer: ecosphere.parser.tokenizer.Tokenizer):
        self._t = tokenizer
