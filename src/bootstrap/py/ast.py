import compiler as piler


class AST:
    pass

class ASTExpression(AST):

    def compile_as_code(self):
        visitor = piler.ASTVisitor()
        self.visit(visitor)
        return visitor.finish()
    
    def visit(self, visitor):
        visitor.invalid_ast(self)
    
    def __init__(self):
        super().__init__()

class ASTValue(ASTExpression):

    def __init__(self):
        super().__init__()

class ASTSelf(ASTValue):
    
    def __repr__(self):
        return 'self'

    def visit(self, visitor):
        visitor.visit_self(self)
    
    def __init__(self):
        super().__init__()

class ASTNil(ASTValue):
    
    def __repr__(self):
        return 'nil'

    def visit(self, visitor):
        visitor.visit_nil(self)
    
    def __init__(self):
        super().__init__()

class ASTConstant(ASTValue):

    def __repr__(self):
        return '<Constant:' + str(self._value) + '>'

    def get_value(self):
        return self._value
    
    def visit(self, visitor):
        visitor.visit_constant(self)
    
    def __init__(self, value):
        super().__init__()
        self._value = value

class ASTSend(ASTExpression):
    
    def __repr__(self):
        return str(self._subj) + '.' + str(self._msg) + str(self._args)

    def get_subj(self):
        return self._subj

    def get_args(self):
        return self._args
    
    def visit(self, visitor):
        visitor.visit_send(self)
    
    def __init__(self, subj, msg, args):
        super().__init__()
        self._subj = subj
        self._msg = msg
        self._args = args

class ASTCompound(ASTExpression):

    def get_instructions(self):
        return self._instrs
    
    def __init__(self, instructions):
        super().__init__()
        self._instrs = instructions

class ASTSequence(ASTCompound):
    
    def visit(self, visitor):
        visitor.visit_sequence(self)
    
    def __init__(self, instructions):
        super().__init__(instructions)

class ASTBlock(ASTValue):

    def get_parameters(self):
        return self._params

    def get_body(self):
        return self._body
    
    def visit(self, visitor):
        visitor.visit_block(self)

    def __init__(self, params, instructions):
        super().__init__()
        self._body = ASTSequence(instructions)
        self._params = params
