
class AST:
    pass

class ASTExpression(AST):
    pass

class ASTValue(ASTExpression):
    pass

class ASTSelf(ASTValue):
    pass

class ASTObject(ASTValue):
    pass

class ASTSend(ASTExpression):
    pass

class ASTCompound(ASTExpression):
    pass

class ASTSequence(ASTCompound):
    pass

class ASTBlock(ASTCompound):
    pass


