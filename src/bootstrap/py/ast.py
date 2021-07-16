
class AST:
    pass

class ASTExpression(AST):
    
    def compile(self, compiler):
        raise Exception('Can\'t compile this AST!')

class ASTValue(ASTExpression):
    pass

class ASTSelf(ASTValue):
    
    def __repr__(self):
        return 'self'

class ASTObject(ASTValue):
    pass

class ASTSend(ASTExpression):
    
    def __repr__(self):
        return str(self._subj) + '.' + str(self._msg) + str(self._args)

    def __init__(self, subj, msg, args):
        self._subj = subj
        self._msg = msg
        self._args = args

class ASTCompound(ASTExpression):
    
    def __init__(self, instructions):
        self._instrs = instructions

class ASTSequence(ASTCompound):
    
    def __init__(self, instructions):
        super().__init__(instructions)

class ASTBlock(ASTCompound):

    def __init__(self, params, instructions):
        super().__init__(instructions)
        self._params = params


