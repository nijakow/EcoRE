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
        if subj.is_self() and len(args) == 0:
            if self._compiler.grab_var_contents(key):
                return
        subj.visit(self)
        self._compiler.push_that()
        for arg in args:
            arg.visit(self)
            self._compiler.push_that()
        self._compiler.compile_send(len(args), key)

    def visit_assign(self, elem):
        self.visit_unimplemented(elem)

    def visit_return(self, elem):
        retval = elem.get_return_value()
        retval.visit(self)
        self._compiler.compile_nonlocal_return()
    
    def visit_compound(self, elem):
        self._compiler.push_scope()
        for instruction in elem.get_instructions():
            instruction.visit(self)
        self._compiler.pop_scope()

    def visit_block(self, elem):
        subcompiler = self._compiler.gen_subcompiler()
        for p in elem.get_parameters():
            subcompiler.add_parameter(p)
        visitor = subcompiler.gen_visitor()
        elem.get_body().visit(visitor)
        self._compiler.compile_make_closure(subcompiler.finish())
    
    def __init__(self, compiler):
        self._compiler = compiler
