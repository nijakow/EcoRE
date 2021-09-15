
class ASTExpression:

    def __init__(self):
        pass


class ASTSelf(ASTExpression):

    def __init__(self):
        super().__init__()

class ASTGroup(ASTExpression):

    def __init__(self):
        super().__init__()

class ASTSend(ASTExpression):
    
    def __init__(self, subject, key, args):
        super().__init__()
        self._subject = subject
        self._key = key
        self._args = args

class ASTVar(ASTExpression):

    def __init__(self, variables):
        super().__init__()
        self._vars = variables

class ASTReturn(ASTExpression):

    def __init__(self, value):
        super().__init__()
        self._value = value

class ASTCompound(ASTExpression):

    def __init__(self, instructions):
        super().__init__()
        self._instructions = instructions

class ASTBlock(ASTExpression):

    def __init__(self, parameters, body):
        super().__init__()
        self._parameters = parameters
        self._body = body
