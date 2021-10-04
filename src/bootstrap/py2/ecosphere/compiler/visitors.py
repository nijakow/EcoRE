import ecosphere.compiler.codegen

class ASTVisitor:

    def visit_unknown(self, ast):
        raise Exception('Visiting unknown AST: ' + str(type(ast)))
    
    def __init__(self):
        pass
    

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
            self._code_generator.op_builtinv(len(args), arg.get_key())
        else:
            self._code_generator.op_builtin(len(args), arg.get_key())

    def visit_send(self, ast):
        arg_count = ast.get_arg_count()
        if (arg_count == 0) and (self._code_generator.load_var(ast.get_key())):
            return
        ast.get_subject().accept(self)
        for arg in ast.get_args():
            arg.accept(self)
            self._code_generator.push()
        if ast.has_varargs():
            self._code_generator.op_sendv(arg_count, ast.get_key()) # TODO: Add one because of the subject?
        else:
            self._code_generator.op_send(arg_count, ast.get_key())  # TODO: Add one because of the subject?

    def visit_assignment(self, ast):
        return self.visit_unknown(ast)  # TODO

    def visit_var(self, ast):
        return self.visit_unknown(ast)  # TODO

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

    def finish(self):
        pass  # TODO: Load the last value as the return value, add a return bytecode

    def __init__(self, code_generator, environment):
        self._code_generator = code_generator
        self._environment = environment

