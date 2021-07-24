import compiler.storage
import compiler.codegenerator
import compiler.visitor


class Compiler:

    def _pull_passed_value(self, storage_loc):
        self._passed_value_callback(storage_loc)

    def _drop_passed_value(self):
        self._pull_passed_value(None)

    def _set_passed_value_callback(self, cb):
        self._passed_value_callback = cb

    def _compile_transfer(self, dest, src):
        pass
    
    def compile_load_self(self):
        self._drop_passed_value()
        def self_compiler(loc):
            if loc is None:
                return None
            elif loc.is_local_register():
                self._codegen.add_self2r(loc.get_id())
                return loc
            else:
                reg = self._regalloc.allocate_temporary_register()
                self._codegen.add_self2r(loc.get_id())
                self._compile_transfer(loc, reg)
                reg.free()
                return loc
        self._set_passed_value_callback(self_compiler)

    def compile_load_constant(self, value):
        self._drop_passed_value()
        def constant_compiler(loc):
            if loc is None:
                return None
            elif loc.is_local_register():
                self._codegen.add_const2r(loc.get_id(), value)
                return loc
            else:
                reg = self._regalloc.allocate_temporary_register()
                self._codegen.add_const2r(loc.get_id(), value)
                self._compile_transfer(loc, reg)
                reg.free()
                return loc
        self._set_passed_value_callback(constant_compiler)

    def compile_pop(self):
        def drop_compiler(loc):
            if loc is None:
                self._codegen.add_drop()
                return None
            elif loc.is_stack():
                return loc  # Nothing to do, already on stack
            elif loc.is_local_register():
                self._codegen.add_pop2r(loc.get_id())
                return loc
            else:
                reg = self._regalloc.allocate_temporary_register()
                self._codegen.add_pop2r(loc.get_id())
                self._compile_transfer(loc, reg)
                reg.free()
                return loc
    
    def compile_send(self, args, key):
        self._drop_passed_value()
        self._codegen.add_send(args, key)
    
    def push_that(self):
        self._pull_passed_value(compiler.storage.STACK)

    def grab_var_contents(self, varname):
        # TODO: Find variable, return True or False depending on success
        return False
    
    def send_that_to_var(self, varname):
        # TODO: Find variable, return True or False depending on success
        return False

    def finish(self):
        return self._codegen.finish()
    
    def gen_visitor(self):
        return compiler.visitor.ASTVisitor(self)
    
    def __init__(self):
        self._passed_value_callback = None
        self._codegen = compiler.codegenerator.CodeGenerator()
        self._regalloc = compiler.storage.RegisterAllocator()
