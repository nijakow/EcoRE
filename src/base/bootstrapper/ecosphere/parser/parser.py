import ecosphere.objects.misc
import ecosphere.parser.tokenizer
from ecosphere.parser.tokenizer import TokenType

from ecosphere.parser.ast import ASTExpression, ASTNumber, ASTCharacter, ASTSelf, ASTGroup, ASTObject, ASTPlainObject, ASTSlot, ASTInterface, ASTInterfaceEntry, ASTNumber, ASTKey, ASTString, ASTArray, ASTBuiltin, ASTSend, ASTVarAccess, ASTSeq, ASTCompound, ASTBlock, ASTVar, ASTAssignment, ASTReturn, ASTLabelDef, ASTProxy, ASTAs


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
        return not (c.isalnum() or c == '_' or c == '?' or c == '!')

    def parse_compound(self, env, terminator=TokenType.RPAREN):
        return ASTCompound(self.parse_expressions(env.subenvironment(), terminator))
    
    def parse_block(self, env, terminator=TokenType.RBRACK):
        parameters = []
        varargs = False
        while True:
            if self.check(TokenType.COLON):
                the_type = self.parse_optional_type(env)
                key = ecosphere.objects.misc.EcoKey.Get(self.expect(TokenType.IDENTIFIER).get_text())
                env.bind(key)
                parameters.append((key, the_type))
                if not self.check(TokenType.RARROW) and not self.check(TokenType.BAR):
                    self.expect(TokenType.SEPARATOR)
                    continue
            elif self.check(TokenType.ELLIPSIS):
                varargs = True
                if not self.check(TokenType.RARROW):
                    self.expect(TokenType.BAR)
            break
        return ASTBlock(None, parameters, varargs, self.parse_compound(env, terminator)) # TODO: Parse type
    
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
            elif self.check(TokenType.INHERITED):
                flags.add('inherited')
                flags.add('nodelegate')
            elif self.check(TokenType.STATIC):
                flags.add('static')
            elif self.check(TokenType.PROTECTED):
                flags.add('private')
            elif self.check(TokenType.PRIVATE):
                flags.add('private')
                flags.add('static')
            elif self.check(TokenType.FINAL):
                flags.add('final')
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
        ups = []
        while not self.check(TokenType.RCURLY):
            if self.check(TokenType.CARET):
                ups.append(self.parse_expression(env))
            else:
                slots.append(self.parse_object_slot(env))
            if self.check(TokenType.RCURLY): break
            self.expect(TokenType.SEPARATOR)
        return ASTPlainObject(ups, slots)
    
    def parse_array(self, env):
        return ASTArray(self.parse_expressions(env, TokenType.RPAREN))
    
    def parse_argdef(self, env):
        the_type = self.parse_optional_type(env)
        # the_expr = self.parse_expression(env)
        the_expr = self.expect(TokenType.IDENTIFIER).get_key()
        return (the_type, the_expr)

    def parse_argdefs(self, env):
        args = []
        has_varargs = False
        if not self.check(TokenType.RPAREN):
            while True:
                if self.check(TokenType.ELLIPSIS):
                    has_varargs = True
                    self.expect(TokenType.RPAREN)
                    break
                args.append(self.parse_argdef(env))
                if self.check(TokenType.RPAREN): break
                self.expect(TokenType.SEPARATOR)
        return args, has_varargs

    def parse_optional_argdefs(self, env):
        if self.check(TokenType.LPAREN):
            return self.parse_argdefs(env)
        return [], False

    def parse_slot_decl(self, env):
        return_type = self.parse_optional_type(env)
        has_varargs = False
        t = self.check_unary_ident()
        if t:
            args, has_varargs = self.parse_optional_argdefs(env)
            return return_type, t.get_key(), args, has_varargs
        t = self.check_binary_ident()
        if t:
            return return_type, t.get_key(), [self.parse_argdef(env)], False
        t = self.check_nary_ident()
        if t:
            args = []
            name = ''
            while t:
                name += t.get_text()
                args.append(self.parse_argdef(env))
                t = self.check_nary_ident()
            return return_type, ecosphere.objects.misc.EcoKey.Get(name), args, False
        name = self.expect(TokenType.KEY).get_key()
        args, has_varargs = self.parse_optional_argdefs(env)
        return return_type, name, args, has_varargs

    def parse_interface(self, env):
        parents = []
        slots = []
        while not self.check(TokenType.RBRACK):
            if self.check(TokenType.WITH):
                parents.append(self.parse_expression(env))
            else:
                return_type, name, args, has_varargs = self.parse_slot_decl(env)
                slots.append(ASTInterfaceEntry(name, return_type, args, has_varargs))
            self.check(TokenType.SEPARATOR)
        return ASTInterface(parents, slots)

    def parse_simple_expression(self, env, allow_followups=2) -> ASTExpression:
        if self.check(TokenType.SELF):
            return ASTSelf()
        elif self.check(TokenType.LPAREN):
            return self.parse_compound(env)
        elif self.check(TokenType.LBRACK):
            return self.parse_block(env)
        elif self.check(TokenType.LCURLY):
            return self.parse_object(env)
        elif self.check(TokenType.HASHLPAREN):
            return self.parse_array(env)
        elif self.check(TokenType.PERCLBRACK):
            return self.parse_interface(env)
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
    
    def parse_send(self, env, ast: ASTExpression, allow_followups) -> ASTExpression:
        t = self.check_unary_ident()
        if not t:
            t = self.check(TokenType.KEY)
        if t:
            args, varargs = self.parse_optional_arglist(env)
            return ASTSend(ast, t.get_key(), args, varargs)
        if allow_followups <= 0:
            return ast
        t = self.check_binary_ident()
        if t:
            while t:
                arg = self.parse_expression(env, 0)
                ast = ASTSend(ast, t.get_key(), [arg], False)
                t = self.check_binary_ident()
            return ast
        if allow_followups <= 1:
            return ast
        t = self.check_nary_ident()
        if t:
            name = ''
            args = []
            while t:
                name += t.get_text()
                args.append(self.parse_expression(env, 1))
                t = self.check_nary_ident()
            return ASTSend(ast, ecosphere.objects.misc.EcoKey.Get(name), args, False)
        return ast

    def parse_bar(self, env, terminator=TokenType.BAR):
        the_type = self.parse_optional_type(env)
        varname = self.expect(TokenType.IDENTIFIER).get_key()
        env.bind(varname)  # Don't bind the type
        if self.check(TokenType.ASSIGNMENT):
            value = self.parse_expression(env)
        else:
            value = ASTSelf()
        self.check(TokenType.SEPARATOR)
        if self.check(terminator):
            return ASTVar(the_type, varname, value, self.parse_expression(env))
        else:
            return ASTVar(the_type, varname, value, self.parse_bar(env))

    def parse_expression(self, env, allow_followups=2) -> ASTExpression:
        if (allow_followups >= 2) and self.check(TokenType.CARET):
            return ASTReturn(self.parse_expression(env, allow_followups))
        elif (allow_followups >= 2) and self.check(TokenType.BAR):
            return self.parse_bar(env)
        ast = self.parse_simple_expression(env, allow_followups)
        next = None
        while True:
            next = ast
            ast = self.parse_send(env, ast, allow_followups)
            if self.check(TokenType.LPAREN):
                args, varargs = self.parse_arglist(env)
                ast = ASTSend(ast, ecosphere.objects.misc.EcoKey.Get('value'), args, varargs)
            elif self.check(TokenType.AS):
                ast = ASTAs(ast, self.parse_expression(env, False))
            elif self.check(TokenType.ASSIGNMENT):
                ast = ASTAssignment(ast, self.parse_expression(env, allow_followups))
            elif (allow_followups >= 2) and self.check(TokenType.TILDE):
                pass
            elif ast == next:
                break
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
    
    def parse_optional_arglist(self, env) -> list:
        if self.check(TokenType.LPAREN):
            return self.parse_arglist(env)
        return [], False

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
            raise ParseException('Expected a token type \'' + str(token_type) + '\', got \'' + str(t._type) + '\'!', t)
    
    def check_unary_ident(self):
        t = self.check(TokenType.IDENTIFIER)
        if not t: return False
        name = t.get_text()
        if not self._is_bin(name[-1]):
            return t
        else:
            t.fail()
            return False
    
    def check_binary_ident(self):
        t = self.check(TokenType.IDENTIFIER)
        if not t: return False
        name = t.get_text()
        if self._is_bin(name[-1]) and name[-1] != ':':
            return t
        else:
            t.fail()
            return False
    
    def check_nary_ident(self):
        t = self.check(TokenType.IDENTIFIER)
        if not t: return False
        name = t.get_text()
        if name[-1] == ':':
            return t
        else:
            t.fail()
            return False

    def parse(self, terminator=TokenType.EOF):
        env = ParseEnv()
        return self.parse_expressions(env, terminator)

    def __init__(self, tokenizer: ecosphere.parser.tokenizer.Tokenizer):
        self._t = tokenizer
