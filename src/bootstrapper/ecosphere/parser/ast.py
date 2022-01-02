import ecosphere.objects.plain
import ecosphere.objects.interface
import ecosphere.compiler


class ASTExpression:

    def accept(self, visitor):
        visitor.visit_unknown(self)
    
    def evaluate(self, the_subject, the_environment, the_callback):
        raise Exception('Unable to evaluate this AST: ' + str(type(self)))

    def __init__(self):
        pass


class ASTSelf(ASTExpression):

    def accept(self, visitor):
        visitor.visit_self(self)

    def evaluate(self, the_subject, the_environment, the_callback):
        the_callback(the_subject)

    def __init__(self):
        super().__init__()

class ASTObject(ASTExpression):

    def accept(self, visitor):
        visitor.visit_object(self)
    
    def evaluate(self, the_subject, the_environment, the_callback):
        self._evaluate(the_subject, the_environment, the_callback)
    
    def _evaluate(self, the_subject, the_environment, the_callback):
        raise Exception('Can\'t construct a value for this AST!')

    def __init__(self):
        super().__init__()

class ASTGroup(ASTObject):

    def __init__(self, objects):
        super().__init__()
        self._objects = objects

class ASTPlainObject(ASTObject):
    
    def _evaluate(self, the_subject, the_environment, the_callback):
        plain = ecosphere.objects.plain.EcoPlainObject()
        the_callback(plain)
        for up in self._ups:
            up.evaluate(the_subject, the_environment, lambda value: plain.add_up(value))
        for slot in self._slots:
            slot.create_on(plain, the_environment)

    def __init__(self, ups, slots):
        super().__init__()
        self._ups = ups
        self._slots = slots

class ASTSlot:

    def create_on(self, plain_object, the_environment):
        if 'code' in self._flags:
            code = ecosphere.compiler.compile_ast(self._value, the_environment, self._type, self._args, 'varargs' in self._flags)
            slot = ecosphere.objects.plain.EcoCodeSlot(self._name, code)
        else:
            slot = ecosphere.objects.plain.EcoValueSlot(self._name, 'inherited' in self._flags, 'nodelegate' in self._flags, 'part' in self._flags)
            if self._value is not None:
                self._value.evaluate(plain_object, the_environment, lambda value: slot.set_value(value))
        plain_object.add_slot(slot)

    def __init__(self, name, the_type, args, flags, value):
        self._name = name
        self._type = the_type
        self._args = args
        self._flags = flags
        self._value = value

class ASTInterface(ASTObject):

    def _evaluate(self, the_subject, the_environment, the_callback):
        interf = ecosphere.objects.interface.EcoInterface()
        the_callback(interf)
        for parent in self._parents:
            parent.evaluate(the_subject, the_environment, lambda value: interf.add_parent(value))
        for entry in self._entries:
            entry.create_on(the_subject, interf, the_environment)

    def __init__(self, parents, entries):
        super().__init__()
        self._parents = parents
        self._entries = entries

class ASTInterfaceEntry:

    def create_on(self, the_subject, interf, the_environment):
        entry = ecosphere.objects.interface.EcoInterfaceEntry(self._name, len(self._args), self._has_varargs)
        if self._return_type is not None:
            self._return_type.evaluate(the_subject, the_environment, lambda value: entry.set_return_type(value))
        for i in range(0, len(self._args)):
            if self._args[i][0] is not None:
                our_i = i
                self._args[i][0].evaluate(the_subject, the_environment, lambda value: entry.set_arg_type(our_i, value))
        interf.add_entry(entry)

    def __init__(self, name, return_type, args, has_varargs):
        self._return_type = return_type
        self._name = name
        self._args = args
        self._has_varargs = has_varargs

class ASTNumber(ASTObject):

    def _evaluate(self, the_subject, the_environment, the_callback):
        the_callback(ecosphere.objects.base.EcoNumber(self._value))

    def __init__(self, value):
        super().__init__()
        self._value = value

class ASTCharacter(ASTObject):

    def _evaluate(self, the_subject, the_environment, the_callback):
        the_callback(ecosphere.objects.base.EcoCharacter(self._value))

    def __init__(self, value):
        super().__init__()
        self._value = value

class ASTKey(ASTObject):

    def _evaluate(self, the_subject, the_environment, the_callback):
        the_callback(self._key)

    def __init__(self, key):
        super().__init__()
        self._key = key

class ASTString(ASTObject):

    def _evaluate(self, the_subject, the_environment, the_callback):
        the_callback(self._string)

    def __init__(self, string):
        super().__init__()
        self._string = string

class ASTArray(ASTObject):

    def _evaluate(self, the_subject, the_environment, the_callback):
        v = ecosphere.objects.misc.EcoArray(len(self._elements))
        the_callback(v)
        for i in range(0, len(self._elements)):
            self._elements[i].evaluate(the_subject, the_environment, v.get_setter(i))

    def __init__(self, elements):
        super().__init__()
        self._elements = elements

class ASTBuiltin(ASTExpression):

    def accept(self, visitor):
        visitor.visit_builtin(self)

    def get_key(self):
        return self._key

    def get_args(self):
        return self._args
    
    def has_varargs(self):
        return self._varargs

    def __init__(self, key, args, varargs):
        super().__init__()
        self._key = key
        self._args = args
        self._varargs = varargs

class ASTVarAccess(ASTExpression):

    def accept(self, visitor):
        visitor.visit_var_access(self)
    
    def get_key(self):
        return self._key
    
    def __init__(self, key):
        super().__init__()
        self._key = key

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
    
    def evaluate(self, the_subject, the_environment, the_callback):
        def subject_callback(the_new_subject):
            slot = the_new_subject.lookup_key(self.get_key())
            slot.evaluate(the_new_subject, the_environment, the_callback)
        assert self.get_arg_count() == 0
        self.get_subject().evaluate(the_subject, the_environment, subject_callback)

    def __init__(self, subject, key, args, varargs):
        super().__init__()
        self._subject = subject
        self._key = key
        self._args = args
        self._varargs = varargs

class ASTVar(ASTExpression):

    def accept(self, visitor):
        visitor.visit_var(self)
    
    def get_type(self):
        return self._type

    def get_var(self):
        return self._var
    
    def get_var_value(self):
        return self._value
    
    def get_followup_expression(self):
        return self._next

    def __init__(self, the_type, variable, value, next_expr):
        super().__init__()
        self._type = the_type
        self._var = variable
        self._value = value
        self._next = next_expr

class ASTAssignment(ASTExpression):

    def accept(self, visitor):
        visitor.visit_assignment(self)
    
    def get_lhs(self):
        return self._lhs
    
    def get_rhs(self):
        return self._rhs

    def __init__(self, lhs, rhs):
        super().__init__()
        self._lhs = lhs
        self._rhs = rhs

class ASTReturn(ASTExpression):

    def accept(self, visitor):
        visitor.visit_return(self)

    def get_expr(self):
        return self._value

    def __init__(self, value):
        super().__init__()
        self._value = value

class ASTSeq(ASTExpression):

    def accept(self, visitor):
        visitor.visit_seq(self)
    
    def get_instructions(self):
        return self._instructions

    def __init__(self, *instructions):
        super().__init__()
        self._instructions = instructions

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

    def get_type(self):
        return self._type

    def get_parameters(self):
        return self._parameters
    
    def has_varargs(self):
        return self._parameters
    
    def get_body(self):
        return self._body

    def __init__(self, the_type, parameters, varargs, body):
        super().__init__()
        self._type = the_type
        self._parameters = parameters
        self._varargs = varargs
        self._body = body

class ASTLabelDef(ASTExpression):

    def accept(self, visitor):
        visitor.visit_labeldef(self)
    
    def evaluate(self, subject, environment, the_callback):
        def our_callback(value):
            environment.define_label(self._address, value)
            the_callback(value)
        self._expr.evaluate(subject, environment, our_callback)

    def __init__(self, address, expr):
        super().__init__()
        self._address = address
        self._expr = expr

class ASTProxy(ASTExpression):

    def accept(self, visitor):
        visitor.visit_proxy(self)
    
    def evaluate(self, subject, environment, the_callback):
        if self._proxy_type == '':
            if '@' in self._address:
                raise Exception('The use of the \'@\' sign in proxies is now forbidden!')
            else:
                environment.when_label_defined(self._address, the_callback)
        else:
            raise Exception('Can\'t evaluate ASTProxy: Proxy type not recognized!')

    def __init__(self, proxy_type, address):
        super().__init__()
        self._proxy_type = proxy_type
        self._address = address

class ASTAs(ASTExpression):

    def get_left(self):
        return self._left
    
    def get_right(self):
        return self._right

    def accept(self, visitor):
        visitor.visit_as(self)
    
    def __init__(self, left, right):
        super().__init__()
        self._left = left
        self._right = right
