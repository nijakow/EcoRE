from datatypes import PlainObject
import compiler.base


class AST:
    pass

class ASTExpression(AST):

    def is_self(self):
        return False
    
    def compile_as_code(self, params=[], has_varargs=False):
        piler = compiler.base.Compiler()
        for p in params:
            piler.add_parameter(p)
        if has_varargs:
            piler.enable_varargs()
        visitor = piler.gen_visitor()
        self.visit(visitor)
        return piler.finish()
    
    def visit(self, visitor):
        visitor.visit_invalid(self)

    def evaluate(self, subj):
        raise Exception('Can\'t evaluate this AST!')
    
    def __init__(self):
        super().__init__()

class ASTProxy(ASTExpression):

    def get_value(self):
        return self._value
    
    def set_value(self, val):
        self._value = val

    def visit(self, visitor):
        self._value.visit(visitor)
    
    def evaluate(self, subj):
        return self._value.evaluate(subj)
    
    def __init__(self):
        super().__init__()
        self._value = None

class ASTValue(ASTExpression):

    def __init__(self):
        super().__init__()

class ASTSelf(ASTValue):
    
    def __repr__(self):
        return 'self'

    def is_self(self):
        return True
    
    def visit(self, visitor):
        visitor.visit_self(self)

    def evaluate(self, subj):
        return subj
        
    def __init__(self):
        super().__init__()

class ASTNil(ASTValue):
    
    def __repr__(self):
        return 'nil'

    def visit(self, visitor):
        visitor.visit_nil(self)
    
    def evaluate(self, subj):
        return PlainObject()  # TODO: Use a reference to NIL

    def __init__(self):
        super().__init__()

class ASTConstant(ASTValue):

    def __repr__(self):
        return '<Constant:' + str(self._value) + '>'

    def get_value(self):
        return self._value
    
    def visit(self, visitor):
        visitor.visit_constant(self)

    def evaluate(self, subj):
        return self.get_value()
        
    def __init__(self, value):
        super().__init__()
        self._value = value

class ASTKey(ASTConstant):

    def __init__(self, value):
        super().__init__(value)

class ASTBuiltin(ASTExpression):

    def get_key(self):
        return self._key
    
    def get_args(self):
        return self._args
    
    def has_varargs(self):
        return self._has_varargs
    
    def visit(self, visitor):
        visitor.visit_builtin(self)

    def __init__(self, key, args, has_varargs=False):
        self._key = key
        self._args = args
        self._has_varargs = has_varargs

class ASTSend(ASTExpression):
    
    def __repr__(self):
        return str(self._subj) + '.' + str(self._msg) + str(self._args)

    def get_subj(self):
        return self._subj

    def get_message(self):
        return self._msg

    def get_args(self):
        return self._args
    
    def has_varargs(self):
        return self._has_varargs
    
    def visit(self, visitor):
        visitor.visit_send(self)

    def evaluate(self, subj):
        assert len(self.get_args()) == 0
        subj = self.get_subj().evaluate(subj)
        if subj is None:
            return None
        else:
            slot = subj.lookup_slot(self.get_message())
            return slot.get_value()
    
    def __init__(self, subj, msg, args, has_varargs=False):
        super().__init__()
        self._subj = subj
        self._msg = msg
        self._args = args
        self._has_varargs = has_varargs


class ASTVarDecl(ASTExpression):

    def get_declaration(self):
        return self._decl
    
    def get_next_expression(self):
        return self._next

    def visit(self, visitor):
        visitor.visit_var_decl(self)

    def __init__(self, decl, next_expr):
        self._decl = decl
        self._next = next_expr

class ASTAssignment(ASTExpression):

    def get_lhs(self):
        return self._lhs
    
    def get_rhs(self):
        return self._rhs

    def visit(self, visitor):
        visitor.visit_assignment(self)

    def __init__(self, lhs, rhs):
        self._lhs = lhs
        self._rhs = rhs

class ASTReturn(ASTExpression):

    def __repr__(self):
        return '^ ' + str(self._retval)

    def get_return_value(self):
        return self._retval

    def visit(self, visitor):
        visitor.visit_return(self)
    
    def __init__(self, retval):
        super().__init__()
        self._retval = retval

class ASTCompound(ASTExpression):

    def get_instructions(self):
        return self._instrs
    
    def visit(self, visitor):
        visitor.visit_compound(self)

    def __init__(self, instructions):
        super().__init__()
        self._instrs = instructions

class ASTBlock(ASTValue):

    def get_parameters(self):
        return self._params
    
    def has_varargs(self):
        return self._has_varargs

    def get_body(self):
        return self._body
    
    def visit(self, visitor):
        visitor.visit_block(self)

    def __init__(self, params, instructions, has_varargs=False):
        super().__init__()
        self._body = ASTCompound(instructions)
        self._params = params
        self._has_varargs = has_varargs
