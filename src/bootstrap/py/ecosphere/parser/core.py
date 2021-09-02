from ecosphere.parser.tokens import TokenType as TokenType
import ecosphere.parser.stream
import ecosphere.parser.ast


def default_object():
    return ecosphere.parser.ast.ASTNil()


class ParseException(Exception):

    def __init__(self, ast, message):
        super().__init__(message)
        self._ast = ast


class LabelStorage:

    def define_label(self, key, value):
        self._labels[key] = value
        if key in self._label_callbacks:
            callbacks = self._label_callbacks[key]
            del self._label_callbacks[key]
            for cb in callbacks:
                cb(value)
    
    def get_label_value(self, key):
        return self._labels[key]

    def __init__(self):
        self._labels = dict()
        self._label_callbacks = dict()


class ParseManager:
    
    def define_label(self, key, value):
        self._label_storage.define_label(key, value)
    
    def get_label_value(self, key):
        return self._label_storage.get_label_value(key)
    
    def parse_file(self, path):
        if path not in self._files:
            # TODO: Insert sentinel value to avoid endless recursions
            print('Loading file', path, '...')
            with open(path, 'r') as f:
                text_stream = ecosphere.parser.stream.TextStream(f.read())
                the_parser = EcoParser(ecosphere.parser.tokens.Tokenizer(text_stream), self)
                self._files[path] = the_parser.parse()
        return self._files[path]

    def __init__(self):
        self._label_storage = LabelStorage()
        self._files = dict()


class Parser:

    def check(self, token_type):
        t = self._t.read()
        if t.isa(token_type):
            return t
        else:
            t.fail()
            return False
    
    def check_identifier(self):
        return self.check(TokenType.IDENTIFIER)
    
    def check_key(self):
        return self.check(TokenType.KEY)
    
    def expect(self, token_type):
        t = self._t.read()
        if t.isa(token_type):
            return t
        else:
            t.fail()
            raise ParseException(t, 'Expected different token type: ' + str(token_type) + ', got: ' + str(t))
    
    def expect_identifier(self):
        return self.expect(TokenType.IDENTIFIER)

    def get_tokenizer(self):
        return self._t
    
    def get_pm(self):
        return self._pm

    def __init__(self, tokenizer, parse_manager=None):
        self._t = tokenizer
        self._pm = parse_manager or ParseManager()


class SubParser(Parser):

    def get_parent(self):
        return self._parent

    def __init__(self, parent):
        super().__init__(parent.get_tokenizer(), parent.get_pm())
        self._parent = parent


class ExpressionParser(SubParser):

    def parse_argument_list(self, terminator=TokenType.RPAREN):
        params = []
        if self.check(terminator):
            return params, False
        while True:
            if self.check(TokenType.ELLIPSIS):
                self.expect(terminator)
                return params, True
            params.append(self.parse_expression())
            if self.check(terminator):
                return params, False
            self.expect(TokenType.SEPARATOR)

    def parse_expression_list(self, terminator):
        l = []
        if not self.check(terminator):
            l.append(self.parse_expression())
            while not self.check(terminator):
                self.expect(TokenType.SEPARATOR)
                if self.check(terminator):
                    break
                l.append(self.parse_expression())
        return l

    def parse_expression(self, allow_followups=True):
        the_ast = self.gen_simple_expression_parser().parse_simple_expression(allow_followups)
        prev = None
        while the_ast != prev:
            prev = the_ast
            the_ast = self.gen_send_parser().parse_send(the_ast, allow_followups)
            if self.check(TokenType.LPAREN):
                args, varargs = self.parse_argument_list(TokenType.RPAREN)
                if isinstance(the_ast, ecosphere.parser.ast.ASTKey):
                    the_ast = ecosphere.parser.ast.ASTBuiltin(the_ast, args, has_varargs=varargs)
                else:
                    the_ast = ecosphere.parser.ast.ASTSend(the_ast,
                                                 ecosphere.datatypes.Key.get('value'),
                                                 args,
                                                 has_varargs=varargs)
            elif self.check(TokenType.ASSIGNMENT):
                the_ast = ecosphere.parser.ast.ASTAssignment(the_ast,
                                                   self.parse_expression(allow_followups))
        return the_ast

    def gen_simple_expression_parser(self):
        return SimpleExpressionParser(self)

    def gen_send_parser(self):
        return SendParser(self)

    def __init__(self, parent_parser):
        super().__init__(parent_parser)


class SimpleExpressionParser(ExpressionParser):

    def parse_compound(self, end=TokenType.RPAREN):
        exprs = self.parse_expression_list(end)
        if len(exprs) == 1:
            return exprs[0]
        else:
            return ecosphere.parser.ast.ASTCompound(exprs)
    
    def parse_closure(self):
        return self.gen_closure_parser().parse_closure()

    def parse_object(self):
        return self.gen_object_parser().parse_object()
    
    def parse_group(self):
        return self.gen_group_parser().parse_group()
    
    def parse_return(self, allow_followups=True):
        return ecosphere.parser.ast.ASTReturn(self.parse_expression(allow_followups))
    
    def parse_var_decl(self, allow_followups=True):
        decls = []
        while True:
            name = self.expect_identifier().get_key()
            if self.check(TokenType.ASSIGNMENT):
                value = self.parse_expression()
            else:
                value = ecosphere.parser.ast.ASTNil()
            decls.append((name, value))
            if self.check(TokenType.BAR):
                break
            self.expect(TokenType.SEPARATOR)
        return ecosphere.parser.ast.ASTVarDecl(decls, self.parse_expression(allow_followups))

    def parse_simple_expression(self, allow_followups=True):
        if self.check(TokenType.SELF):
            return ecosphere.parser.ast.ASTSelf()
        elif self.check(TokenType.LPAREN):
            return self.parse_compound()
        elif self.check(TokenType.LBRACK):
            return self.parse_closure()
        elif self.check(TokenType.LCURLY):
            if self.check(TokenType.RARROW):
                return self.parse_group()
            else:
                return self.parse_object()
        elif self.check(TokenType.CARET):
            return self.parse_return(allow_followups)
        elif self.check(TokenType.BAR):
            return self.parse_var_decl(allow_followups)
        else:
            kw = self.get_tokenizer().read()
            if kw.isa(TokenType.KEY):
                return ecosphere.parser.ast.ASTKey(kw.get_key())
            elif kw.isa(TokenType.CONSTANT):
                return ecosphere.parser.ast.ASTConstant(kw.get_value())
            elif kw.is_type(TokenType.LABEL):
                if kw.is_definition():
                    expr = self.parse_simple_expression()
                    self.get_pm().define_label(kw.get_key(), expr)
                    return expr
                else:
                    return ecosphere.parser.ast.ASTProxy(kw.get_key())
            elif kw.is_type(TokenType.FILE):
                path = kw.get_filename()
                return self._pm.parse_file(path)
            else:
                kw.fail()
                return ecosphere.parser.ast.ASTSelf()

    def gen_closure_parser(self):
        return ClosureParser(self)
    
    def gen_object_parser(self):
        return ObjectParser(self)
    
    def gen_group_parser(self):
        return GroupParser(self)

    def __init__(self, parent_parser):
        super().__init__(parent_parser)


class ClosureParser(ExpressionParser):

    def parse_closure(self):
        params = []
        exprs = []
        varargs = False
        if not self.check(TokenType.RBRACK):
            exprs.append(self.parse_expression())
            while True:
                if self.check(TokenType.RARROW):
                    for e in exprs:
                        params.append(e)
                    exprs.clear()
                    exprs.append(self.parse_expression())
                elif self.check(TokenType.ELLIPSIS):
                    varargs = True
                    self.expect(TokenType.RARROW)
                    for e in exprs:
                        params.append(e)
                    exprs.clear()
                    exprs.append(self.parse_expression())
                elif self.check(TokenType.SEPARATOR):
                    exprs.append(self.parse_expression())
                else:
                    self.expect(TokenType.RBRACK)
                    break
        return ecosphere.parser.ast.ASTBlock(params, exprs, has_varargs=varargs)

    def __init__(self, parent_parser):
        super().__init__(parent_parser)


class ObjectSlotParser(ExpressionParser):

    def _parse_flags(self):
        while True:
            if self.check(TokenType.WITH):
                self._flag_inherited = True
            elif self.check(TokenType.PRIVATE):
                self._flag_private = True
            else:
                break

    def parse_header(self):
        key = self.expect_identifier().get_key()
        params = []
        varargs = False
        if self.check(TokenType.COLON):
            key = key.colonize()
            params.append(self.expect_identifier().get_key())
            t = self.get_tokenizer().read()
            while t.check(TokenType.IDENTIFIER):
                self.expect(TokenType.COLON)
                key = key.extend_name(t.get_key().colonize())
                params.append(self.expect_identifier().get_key())
                t = self.get_tokenizer().read()
        elif self.check(TokenType.LPAREN):
            exprs, varargs = self.parse_argument_list(TokenType.RPAREN)
            for p in exprs:
                params.append(p)
        return key, params, varargs

    def parse_slot(self):
        self._parse_flags()
        self._key, self._params, self._has_varargs = self.parse_header()
        if self.check(TokenType.RARROW):
            self._flag_method = True
            self._body = self.parse_expression()
        elif self.check(TokenType.EQUALS):
            self._body = self.parse_expression()
        self._finish()

    def _finish(self):
        if self._flag_method:
            assert not self._flag_inherited
            # code = self._body.compile_as_code(self._params, has_varargs=self._has_varargs)
            self._object.add_slot(ecosphere.parser.ast.ASTCodeSlot(self._flag_private, self._key, self._params, self._has_varargs, self._body))
        else:
            assert len(self._params) == 0
            self._object.add_slot(ecosphere.parser.ast.ASTValueSlot(self._flag_private, self._flag_inherited, self._key, self._body))

    def __init__(self, parent_parser, the_object):
        super().__init__(parent_parser)
        self._object = the_object
        self._key = None
        self._params = []
        self._has_varargs = False
        self._body = default_object()
        self._flag_inherited = False
        self._flag_private = False
        self._flag_method = False


class ObjectParser(ExpressionParser):

    def parse_object(self):
        kw = self.check_key()
        if kw:
            self.get_pm().define_label(kw.get_key(), self._ast)
        while not self.check(TokenType.RCURLY):
            self.gen_object_slot_parser().parse_slot()
            self.check(TokenType.SEPARATOR)
        return self._object

    def gen_object_slot_parser(self):
        return ObjectSlotParser(self, self._object)

    def __init__(self, parent_parser):
        super().__init__(parent_parser)
        self._object = ecosphere.parser.ast.ASTPlainObject()


class GroupParser(ExpressionParser):

    def parse_group(self):
        return ecosphere.parser.ast.ASTGroupObject(self.parse_expression_list(TokenType.RCURLY))

    def __init__(self, parent_parser):
        super().__init__(parent_parser)


class SendParser(ExpressionParser):

    def _parse_binary_op(self, allow_followups):
        self._params.append(self.parse_expression(allow_followups))
    
    def _parse_paren_arglist(self):
        args, self._has_varargs = self.parse_argument_list(TokenType.RPAREN)
        for arg in args:
            self._params.append(arg)
    
    def _parse_multi(self, key):
        while True:
            self._params.append(self.parse_expression(False))
            ident = self.check_identifier()
            if not ident:
                break
            colon = self.get_tokenizer().read()
            if not colon.check(TokenType.COLON):
                ident.fail()
                colon.fail()
                break
            key = key.extend_name(ident.get_key().colonize())
        return key

    def parse_send(self, the_ast, allow_followups=False):
        ident = self.check_identifier()
        if ident:
            key = ident.get_key()
            if key.is_binary_op():
                self._parse_binary_op(allow_followups)
            elif self.check(TokenType.LPAREN):
                self._parse_paren_arglist()
            else:
                colon = self.get_tokenizer().read()
                if colon.check(TokenType.COLON):
                    if not allow_followups:
                        colon.fail()
                        ident.fail()
                        return the_ast
                    else:
                        key = self._parse_multi(key.colonize())
            return ecosphere.parser.ast.ASTSend(the_ast, key, self._params, has_varargs=self._has_varargs)
        else:
            return the_ast
    
    def __init__(self, parent_parser):
        super().__init__(parent_parser)
        self._params = []
        self._has_varargs = False


class EcoParser(Parser):

    def parse(self):
        return SimpleExpressionParser(self).parse_compound(end=TokenType.EOF)

    def __init__(self, tokenizer, parse_manager):
        super().__init__(tokenizer, parse_manager)
