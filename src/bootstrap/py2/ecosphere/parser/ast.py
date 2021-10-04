
class ASTExpression:

    def accept(self, visitor):
        visitor.visit_unknown(self)

    def __init__(self):
        pass


class ASTSelf(ASTExpression):

    def accept(self, visitor):
        visitor.visit_self(self)

    def __init__(self):
        super().__init__()

class ASTObject(ASTExpression):

    def accept(self, visitor):
        visitor.visit_object(self)

    def __init__(self):
        super().__init__()

class ASTGroup(ASTObject):

    def __init__(self, objects):
        super().__init__()
        self._objects = objects

class ASTPlainObject(ASTObject):

    def __init__(self, slots):
        super().__init__()
        self._slots = slots

class ASTSlot:

    def __init__(self, name, the_type, args, flags, value):
        self._name = name
        self._type = the_type
        self._args = args
        self._flags = flags
        self._value = value

class ASTKey(ASTObject):

    def __init__(self, key):
        super().__init__()
        self._key = key

class ASTBuiltin(ASTExpression):

    def accept(self, visitor):
        visitor.visit_builtin(self)

    def __init__(self, key, args, varargs):
        super().__init__()
        self._key = key
        self._args = args
        self._varargs = varargs

class ASTSend(ASTExpression):
    
    def accept(self, visitor):
        visitor.visit_send(self)

    def get_subject(self):
        return self._subject

    def get_key(self):
        return self._key

    def get_args(self):
        return self._args

    def get_arg_count(self):
        return len(self.get_args())

    def has_varargs(self):
        return self._varargs

    def __init__(self, subject, key, args, varargs):
        super().__init__()
        self._subject = subject
        self._key = key
        self._args = args
        self._varargs = varargs

class ASTVar(ASTExpression):

    def accept(self, visitor):
        visitor.visit_var(self)

    def __init__(self, variables):
        super().__init__()
        self._vars = variables

class ASTAssignment(ASTExpression):

    def accept(self, visitor):
        visitor.visit_assignment(self)

    def __init__(self, lhs, rhs):
        super().__init__()
        self._lhs = lhs
        self._rhs = rhs

class ASTReturn(ASTExpression):

    def accept(self, visitor):
        visitor.visit_return(self)

    def __init__(self, value):
        super().__init__()
        self._value = value

class ASTCompound(ASTExpression):

    def accept(self, visitor):
        visitor.visit_compound(self)

    def get_instructions(self):
        return self._instructions

    def __init__(self, instructions):
        super().__init__()
        self._instructions = instructions

class ASTBlock(ASTExpression):

    def accept(self, visitor):
        visitor.visit_block(self)

    def get_parameters(self):
        return self._parameters
    
    def has_varargs(self):
        return self._parameters
    
    def get_body(self):
        return self._body

    def __init__(self, varargs, parameters, body):
        super().__init__()
        self._parameters = parameters
        self._varargs = varargs
        self._body = body

class ASTLabelDef(ASTExpression):

    def accept(self, visitor):
        visitor.visit_labeldef(self)

    def __init__(self, address, expr):
        super().__init__()
        self._address = address
        self._expr = expr

class ASTLabelRef(ASTExpression):

    def accept(self, visitor):
        visitor.visit_labelref(self)

    def __init__(self, address):
        super().__init__()
        self._address = address

