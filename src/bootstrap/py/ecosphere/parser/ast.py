import ecosphere.datatypes
import ecosphere.compiler.base

class AST:
    pass

class ASTExpression(AST):

    def is_self(self):
        return False
    
    def compile_as_code(self, meta, params=[], has_varargs=False):
        piler = ecosphere.compiler.base.Compiler(meta)
        for p in params:
            piler.add_parameter(p)
        if has_varargs:
            piler.enable_varargs()
        visitor = piler.gen_visitor()
        self.visit(visitor)
        return piler.finish()
    
    def visit(self, visitor):
        visitor.visit_invalid(self)

    def evaluate(self, subj, meta):
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

    def evaluate(self, subj, meta):
        return subj
        
    def __init__(self):
        super().__init__()

class ASTNil(ASTValue):
    
    def __repr__(self):
        return 'nil'

    def visit(self, visitor):
        visitor.visit_nil(self)
    
    def evaluate(self, subj, meta):
        return ecosphere.datatypes.PlainObject()  # TODO: Use a reference to NIL, maybe ask Meta for a specific label?

    def __init__(self):
        super().__init__()

class ASTConstant(ASTValue):

    def __repr__(self):
        return '<Constant:' + str(self._value) + '>'

    def get_value(self):
        return self._value
    
    def visit(self, visitor):
        visitor.visit_constant(self)

    def evaluate(self, subj, meta):
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

    def evaluate(self, subj, meta):
        assert len(self.get_args()) == 0
        subj = self.get_subj().evaluate(subj, meta)
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

    def get_declarations(self):
        return self._decls
    
    def get_next_expression(self):
        return self._next

    def visit(self, visitor):
        visitor.visit_var_decl(self)

    def __init__(self, decls, next_expr):
        self._decls = decls
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


class ASTObject(ASTValue):

    def visit(self, visitor):
        visitor.visit_object(self)
    
    def evaluate(self, subj, meta):
        if self._value is None:
            self._value = self.do_evaluate(subj, meta)
        return self._value

    def __init__(self):
        super().__init__()
        self._value = None

class ASTGroupObject(ASTObject):

    def do_evaluate(self, subj, meta):
        group = ecosphere.datatypes.Group()
        for elem in self._elements:
            group.add_object(elem.evaluate(subj, meta))
        return group

    def __init__(self, elements):
        super().__init__()
        self._elements = elements

class ASTSlot:

    def evaluate(self, obj, subj, meta):
        raise Exception('Error while evaluating an ASTSlot: No evaluation method was implemented!')

    def __init__(self, is_private, name):
        self._is_private = is_private
        self._name = name

class ASTCodeSlot(ASTSlot):

    def evaluate(self, obj, subj, meta):
        code = self._body.compile_as_code(meta, self._params, self._has_varargs)
        obj.add_slot(ecosphere.datatypes.CodeSlot(self._name, code, self._is_private))

    def __init__(self, is_private, name, params, has_varargs, body):
        super().__init__(is_private, name)
        self._params = params
        self._has_varargs = has_varargs
        self._body = body

class ASTValueSlot(ASTSlot):

    def evaluate(self, obj, subj, meta):
        obj.add_slot(ecosphere.datatypes.ValueSlot(self._name, self._value.evaluate(subj, meta), self._is_inherited, self._is_private))

    def __init__(self, is_private, is_inherited, name, value):
        super().__init__(is_private, name)
        self._is_inherited = is_inherited
        self._value = value

class ASTPlainObject(ASTObject):

    def add_slot(self, slot):
        self._slots.append(slot)
    
    def do_evaluate(self, subj, meta):
        obj = ecosphere.datatypes.PlainObject()
        for slot in self._slots:
            slot.evaluate(obj, subj, meta)
        return obj

    def __init__(self):
        super().__init__()
        self._slots = list()

class ASTProxy(ASTObject):
    
    def do_evaluate(self, subj, meta):
        value = meta.get_label_value(self._label)
        return value.evaluate(subj, meta)
    
    def __init__(self, label):
        super().__init__()
        self._label = label
