import ecosphere.compiler.codegen

class ASTVisitor:

    def visit_unknown(self, ast):
        raise Exception('Visiting unknown AST: ' + str(type(ast)))
    
    def __init__(self):
        pass


class ASTAssignmentVisitor(ASTVisitor):

    def visit_send(self, ast):
        assert not self._code_generator.is_var(ast.get_key())
        assert ast.get_arg_count() == 0
        ast.get_subject().accept(self._parent_visitor)
        self._code_generator.push()
        self._rhs.accept(self._parent_visitor)
        self._code_generator.push()
        self._code_generator.op_assign(ast.get_key())
    
    def visit_var_access(self, ast):
        assert self._code_generator.is_var(ast.get_key())
        self._rhs.accept(self._parent_visitor)
        assert self._code_generator.store_var(ast.get_key())

    def __init__(self, code_generator, parent_visitor, rhs_ast):
        self._code_generator = code_generator
        self._parent_visitor = parent_visitor
        self._rhs = rhs_ast


class ASTCompilerVisitor(ASTVisitor):

    def visit_self(self, ast):
        self._code_generator.load_self()

    def visit_constant(self, ast):
        self._code_generator.load_constant(ast.get_value())

    def visit_builtin(self, ast):
        args = ast.get_args()
        for arg in args:
            arg.accept(self)
            self._code_generator.push()
        if ast.has_varargs():
            self._code_generator.op_builtinv(len(args), ast.get_key())
        else:
            self._code_generator.op_builtin(len(args), ast.get_key())

    def visit_send(self, ast):
        arg_count = ast.get_arg_count()
        if (arg_count == 0) and (self._code_generator.load_var(ast.get_key())):
            return
        ast.get_subject().accept(self)
        self._code_generator.push()
        for arg in ast.get_args():
            arg.accept(self)
            self._code_generator.push()
        if ast.has_varargs():
            self._code_generator.op_sendv(arg_count + 1, ast.get_key())
        else:
            self._code_generator.op_send(arg_count + 1, ast.get_key())

    def visit_var_access(self, ast):
        if self._code_generator.load_var(ast.get_key()):
            return
        else:
            self._code_generator.load_self()
            self._code_generator.push()
            self._code_generator.op_send(1, ast.get_key())

    def visit_assignment(self, ast):
        assignment_visitor = ASTAssignmentVisitor(self._code_generator, self, ast.get_rhs())
        ast.get_lhs().accept(assignment_visitor)

    def visit_var(self, ast):
        varname = ast.get_var()
        value = ast.get_var_value()
        value.accept(self)
        self._environment.bind(varname, ast.get_type())
        self._code_generator.store_var(varname)
        ast.get_followup_expression().accept(self)

    def visit_return(self, ast):
        ast.get_expr().accept(self)
        self._code_generator.op_return(self._environment.get_depth())

    def visit_seq(self, ast):
        self._code_generator.load_self()
        for expr in ast.get_instructions():
            expr.accept(self)

    def visit_compound(self, ast):
        self._code_generator.load_self()
        for expr in ast.get_instructions():
            expr.accept(self)

    def visit_block(self, ast):
        code = ecosphere.compiler.compile_ast(ast.get_body(),
                                              self._loader,
                                              parameters=ast.get_parameters(),
                                              has_varargs=ast.has_varargs(),
                                              parent_env=self._environment)
        self._code_generator.op_closure(code)
    
    def visit_object(self, ast):
        callback = self._code_generator.load_constant_cb()
        ast.evaluate(None, self._loader, callback)
    
    def visit_proxy(self, ast):
        self.visit_object(ast)
    
    def visit_labeldef(self, ast):
        self.visit_object(ast)
    
    def visit_as(self, ast):
        ast.get_left().accept(self)
        self._code_generator.push()
        ast.get_right().accept(self)
        self._code_generator.op_as()


    def __init__(self, code_generator, environment, loader):
        self._code_generator = code_generator
        self._environment = environment
        self._loader = loader
