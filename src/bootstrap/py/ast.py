
class AST:
    pass

class ASTExpression(AST):
    
    def compile(self, compiler):
        raise Exception('Can\'t compile this AST!')
    
    def __init__(self):
        super().__init__()

class ASTValue(ASTExpression):

    def __init__(self):
        super().__init__()

class ASTSelf(ASTValue):
    
    def __repr__(self):
        return 'self'
    
    def __init__(self):
        super().__init__()

class ASTNil(ASTValue):
    
    def __repr__(self):
        return 'nil'
    
    def __init__(self):
        super().__init__()

class ASTConstant(ASTValue):

    def __repr__(self):
        return '<Constant:' + str(self._value) + '>'

    def __init__(self, value):
        super().__init__()
        self._value = value

class ASTSend(ASTExpression):
    
    def __repr__(self):
        return str(self._subj) + '.' + str(self._msg) + str(self._args)

    def __init__(self, subj, msg, args):
        super().__init__()
        self._subj = subj
        self._msg = msg
        self._args = args

class ASTCompound(ASTExpression):
    
    def __init__(self, instructions):
        super().__init__()
        self._instrs = instructions

class ASTSequence(ASTCompound):
    
    def __init__(self, instructions):
        super().__init__(instructions)

class ASTBlock(ASTCompound):

    def __init__(self, params, instructions):
        super().__init__(instructions)
        self._params = params
