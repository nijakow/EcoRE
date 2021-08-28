import ecosphere.compiler.base
import ecosphere.parser.ast


class ASTVisitor:

    def visit_invalid(self, elem):
        raise Exception('Visitor visiting invalid element!')

    def visit_unimplemented(self, elem):
        raise Exception('Visitor visiting unimplemented element!')
    
    def visit_self(self, elem):
        self._compiler.compile_load_self()

    def visit_nil(self, elem):
        self._compiler.compile_load_self()  # TODO

    def visit_constant(self, elem):
        self._compiler.compile_load_constant(elem.get_value())
    
    def visit_var_decl(self, elem):
        for decl in elem.get_declarations():
            decl[1].visit(self)
            self._compiler.compile_var_declaration(decl[0])
        elem.get_next_expression().visit(self)

    def visit_builtin(self, elem):
        args = elem.get_args()
        self._compiler._drop_passed_value()
        for arg in args:
            arg.visit(self)
            self._compiler.push_that()
        self._compiler.compile_builtin(len(args), elem.get_key().get_value(), has_varargs=elem.has_varargs())

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
        self._compiler.compile_send(len(args) + 1, key, has_varargs=elem.has_varargs())

    def visit_assignment(self, elem):
        lhand = elem.get_lhs()
        rhand = elem.get_rhs()
        assert isinstance(lhand, ecosphere.parser.ast.ASTSend) and len(lhand.get_args()) == 0
        if lhand.get_subj().is_self():
            rhand.visit(self)
            self._compiler.send_that_to_var(lhand.get_message())
        else:
            lhand.get_subj().visit(self)
            self._compiler.push_that()
            rhand.visit(self)
            self._compiler.push_that()
            self._compiler.compile_slot_assignment(lhand.get_message())

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
        if elem.has_varargs():
            subcompiler.enable_varargs()
        visitor = subcompiler.gen_visitor()
        elem.get_body().visit(visitor)
        self._compiler.compile_make_closure(subcompiler.finish())

    def visit_object(self, elem):
        self._compiler.compile_load_constant(elem.evaluate(elem, self._meta))

    def __init__(self, compiler, meta):
        self._compiler = compiler
        self._meta = meta
