import ecosphere.compiler.codegen

class ASTVisitor:

    def visit_unknown(self, ast):
        raise Exception('Visiting unknown AST: ' + str(type(ast)))
    
    def __init__(self):
        pass


class ASTAssignmentVisitor(ASTVisitor):

    def visit_send(self, ast):
        assert ast.get_arg_count() == 0
        if not self._code_generator.store_var(ast.get_key()):
            self._code_generator.push()
            self._code_generator.op_assign(ast.get_key())  # TODO, FIXME, XXX: What's with the subject?

    def __init__(self, code_generator):
        self._code_generator = code_generator


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
            self._code_generator.op_sendv(arg_count, ast.get_key()) # TODO: Add one because of the subject?
        else:
            self._code_generator.op_send(arg_count, ast.get_key())  # TODO: Add one because of the subject?

    def visit_assignment(self, ast):
        ast.get_rhs().accept(self)
        assignment_visitor = ASTAssignmentVisitor(self._code_generator)
        ast.get_lhs().accept(assignment_visitor)

    def visit_var(self, ast):
        varname = ast.get_var()
        value = ast.get_var_value()
        value.accept(self)
        self._environment.bind(varname)
        self._code_generator.store_var(varname)
        ast.get_followup_expression().accept(self)

    def visit_return(self, ast):
        ast.get_expr().accept(self)
        self._code_generator.op_return(self._environment.get_depth())

    def visit_compound(self, ast):
        self._code_generator.load_self()
        for expr in ast.get_instructions():
            expr.accept(self)

    def visit_block(self, ast):
        code = ecosphere.compiler.compile_ast(ast.get_body(), parameters=ast.get_parameters(), has_varargs=ast.has_varargs(), parent_env=self._environment)
        self._code_generator.op_closure(code)
    
    def visit_object(self, ast):
        d = dict()
        def callback(value):
            d['value'] = value
        ast.evaluate(None, self._loader, callback)
        self._code_generator.load_constant(d['value'])
    
    def visit_proxy(self, ast):
        self.visit_object(ast)
    
    def visit_labeldef(self, ast):
        self.visit_object(ast)

    def __init__(self, code_generator, environment, loader):
        self._code_generator = code_generator
        self._environment = environment
        self._loader = loader
