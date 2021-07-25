from datatypes import PlainObject
import compiler.base


class AST:
    pass

class ASTExpression(AST):

    def is_self(self):
        return False
    
    def compile_as_code(self, params=[]):
        piler = compiler.base.Compiler()
        for p in params:
            piler.add_parameter(p)
        visitor = piler.gen_visitor()
        self.visit(visitor)
        return piler.finish()
    
    def visit(self, visitor):
        visitor.visit_invalid(self)

    def evaluate(self, subj):
        raise Exception('Can\'t evaluate this AST!')
    
    def __init__(self):
        super().__init__()

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

class ASTSend(ASTExpression):
    
    def __repr__(self):
        return str(self._subj) + '.' + str(self._msg) + str(self._args)

    def get_subj(self):
        return self._subj

    def get_message(self):
        return self._msg

    def get_args(self):
        return self._args
    
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
    
    def __init__(self, subj, msg, args):
        super().__init__()
        self._subj = subj
        self._msg = msg
        self._args = args

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

    def get_body(self):
        return self._body
    
    def visit(self, visitor):
        visitor.visit_block(self)

    def __init__(self, params, instructions):
        super().__init__()
        self._body = ASTCompound(instructions)
        self._params = params
