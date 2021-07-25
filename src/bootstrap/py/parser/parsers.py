import parser.core
import parser.ast
import datatypes
from parser.tokens import TokenType


def default_object():
    return parser.ast.ASTNil()


class ExpressionParser(parser.core.SubParser):

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
                the_ast = parser.ast.ASTSend(the_ast,
                                             datatypes.Key.get('value'),
                                             self.parse_expression_list(TokenType.RPAREN))
            elif self.check(TokenType.EQUALS):
                the_ast = parser.ast.ASTAssignment(the_ast,
                                                   self.parse_expression(allow_followups))
        return the_ast

    def gen_simple_expression_parser(self):
        return SimpleExpressionParser(self)

    def gen_send_parser(self):
        return SendParser(self)

    def __init__(self, parent_parser):
        super().__init__(parent_parser)


class SimpleExpressionParser(ExpressionParser):

    def parse_compound(self):
        exprs = self.parse_expression_list(TokenType.RPAREN)
        if len(exprs) == 1:
            return exprs[0]
        else:
            return parser.ast.ASTCompound(exprs)
    
    def parse_closure(self):
        return self.gen_closure_parser().parse_closure()

    def parse_object(self):
        return self.gen_object_parser().parse_object()
    
    def parse_return(self, allow_followups=True):
        return parser.ast.ASTReturn(self.parse_expression(allow_followups))

    def parse_simple_expression(self, allow_followups=True):
        if self.check(TokenType.SELF):
            return parser.ast.ASTSelf()
        elif self.check(TokenType.LPAREN):
            return self.parse_compound()
        elif self.check(TokenType.LBRACK):
            return self.parse_closure()
        elif self.check(TokenType.LCURLY):
            return self.parse_object()
        elif self.check(TokenType.CARET):
            return self.parse_return(allow_followups)
        elif self.check(TokenType.BAR):
            return self.parse_var_decl()
        else:
            kw = self.get_tokenizer().read()
            if kw.isa(TokenType.KEY):
                return parser.ast.ASTConstant(kw.get_key())
            elif kw.is_type(TokenType.LABEL):
                proxy = parser.ast.ASTProxy()
                self.get_label_storage().when_label_defined(kw.get_key(), lambda obj: proxy.set_value(obj))
                return proxy
            else:
                kw.fail()
                return parser.ast.ASTSelf()

    def gen_closure_parser(self):
        return ClosureParser(self)
    
    def gen_object_parser(self):
        return ObjectParser(self)

    def __init__(self, parent_parser):
        super().__init__(parent_parser)


class ClosureParser(ExpressionParser):

    def parse_closure(self):
        params = []
        exprs = []
        if not self.check(TokenType.RBRACK):
            exprs.append(self.parse_expression())
            while True:
                if self.check(TokenType.RARROW):
                    for e in exprs:
                        params.append(e)
                    exprs.clear()
                    exprs.append(self.parse_expression())
                elif self.check(TokenType.SEPARATOR):
                    exprs.append(self.parse_expression())
                else:
                    self.expect(TokenType.RBRACK)
                    break
        return parser.ast.ASTBlock(params, exprs)

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
            for p in self.parse_expression_list(TokenType.RPAREN):
                params.append(p)
        return key, params

    def parse_slot(self):
        self._parse_flags()
        self._key, self._params = self.parse_header()
        if self.check(TokenType.RARROW):
            self._flag_method = True
            self._body = self.parse_expression()
        elif self.check(TokenType.EQUALS):
            self._body = self.parse_expression()
        self._finish()

    def _finish(self):
        if self._flag_method:
            assert not self._flag_inherited
            code = self._body.compile_as_code(self._params)
            self._object.add_slot(datatypes.CodeSlot(self._key,
                                                     code,
                                                     is_private=self._flag_private))
        else:
            assert len(self._params) == 0
            self._object.add_slot(datatypes.ValueSlot(self._key,
                                                      self._body.evaluate(self._object),
                                                      is_inherited=self._flag_inherited,
                                                      is_private=self._flag_private))

    def __init__(self, parent_parser, the_object):
        super().__init__(parent_parser)
        self._object = the_object
        self._key = None
        self._params = []
        self._body = default_object()
        self._flag_inherited = False
        self._flag_private = False
        self._flag_method = False


class ObjectParser(ExpressionParser):

    def parse_object(self):
        kw = self.check_key()
        if kw:
            self.get_label_storage().define_label(kw.get_key(), self._ast)
        while not self.check(TokenType.RCURLY):
            self.gen_object_slot_parser().parse_slot()
            self.check(TokenType.SEPARATOR)
        return self._ast

    def gen_object_slot_parser(self):
        return ObjectSlotParser(self, self._object)

    def __init__(self, parent_parser):
        super().__init__(parent_parser)
        self._object = datatypes.PlainObject()
        self._ast = parser.ast.ASTConstant(self._object)


class SendParser(ExpressionParser):

    def _parse_binary_op(self, allow_followups):
        self._params.append(self.parse_expression(allow_followups))
    
    def _parse_paren_arglist(self):
        for arg in self.parse_expression_list(TokenType.RPAREN):
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
            return parser.ast.ASTSend(the_ast, key, self._params)
        else:
            return the_ast
    
    def __init__(self, parent_parser):
        super().__init__(parent_parser)
        self._params = []


class EcoParser(parser.core.Parser):

    def parse(self):
        return ExpressionParser(self).parse_expression()

    def __init__(self, tokenizer):
        super().__init__(tokenizer)
