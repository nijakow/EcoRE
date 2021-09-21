import ecosphere.compiler.codegen

class ASTVisitor:

    def visit_unknown(self, ast):
        raise Exception('Visiting unknown AST!')

    def visit_self(self, ast):
        self._code_generator.load_self()

    def visit_constant(self, ast):
        self._code_generator.load_constant(ast.get_value())

    def visit_builtin(self, ast):
        args = ast.get_args()
        for arg in args:
            arg.accept_visitor(self)
            self._code_generator.push()
        if ast.has_varargs():
            self._code_generator.op_builtinv(len(args), arg.get_key())
        else:
            self._code_generator.op_builtin(len(args), arg.get_key())

    def visit_send(self, ast):
        ast.get_subject().accept_visitor(self)
        args = ast.get_args()
        for arg in args:
            arg.accept_visitor(self)
            self._code_generator.push()
        if ast.has_varargs():
            self._code_generator.op_sendv(len(args), arg.get_key())
        else:
            self._code_generator.op_send(len(args), arg.get_key())

    def visit_assignment(self, ast):
        return self.visit_unknown(ast)  # TODO

    def visit_var(self, ast):
        return self.visit_unknown(ast)  # TODO

    def visit_return(self, ast):
        ast.get_expr().accept_visitor(self)
        self._code_generator.op_return(self._environment.get_depth())

    def visit_compound(self, ast):
        return self.visit_unknown(ast)  # TODO

    def visit_block(self, ast):
        return self.visit_unknown(ast)  # TODO

    def finish(self):
        pass  # TODO: Load the last value as the return value, add a return bytecode

    def __init__(self, code_generator: ecosphere.compiler.codegen.CodeGenerator, environment):
        self._code_generator = code_generator
        self._environment = environment

