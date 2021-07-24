import compiler.base


class ASTVisitor:

    def visit_invalid(self, elem):
        raise Exception('Visitor visiting invalid element!')

    def visit_unimplemented(self, elem):
        raise Exception('Visitor visiting unimplemented element!')
    
    def visit_self(self, elem):
        self._compiler.compile_load_self()

    def visit_nil(self, elem):
        self._compiler.compile_load_nil()

    def visit_constant(self, elem):
        self._compiler.compile_load_constant(elem.get_value())

    def visit_send(self, elem):
        subj = elem.get_subj()
        key  = elem.get_message()
        args = elem.get_args()
        subj.visit(self)
        self._compiler.push_that()
        for arg in args:
            arg.visit(self)
            self._compiler.push_that()
        self._compiler.compile_send(len(args), key)

    def visit_assign(self, elem):
        self.visit_unimplemented(elem)

    def visit_compound(self, elem):
        # TODO: Create new subscope
        for instruction in elem.get_instructions():
            elem.visit(self)

    def visit_block(self, elem):
        code = elem.compile_as_code() # TODO: Pass current environment
        compiler.compile_make_closure(code)
    
    def __init__(self, compiler):
        self._compiler = compiler
