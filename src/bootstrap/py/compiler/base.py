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
                self._codegen.add_popr(loc.get_id())
                return loc
            else:
                reg = self._regalloc.allocate_temporary_register()
                self._codegen.add_popr(loc.get_id())
                self._compile_transfer(loc, reg)
                reg.free()
                return loc
        self._set_passed_value_callback(drop_compiler)
    
    def compile_send(self, args, key):
        self._drop_passed_value()
        def send_compiler(loc):
            self._codegen.add_send(args, key)
            self._compile_transfer(loc, compiler.storage.STACK)
            return loc
        self._set_passed_value_callback(send_compiler)

    def compile_assign(self, key):
        self._drop_passed_value()
        def assign_compiler(loc):
            self._codegen.add_assign(key)
            self._compile_transfer(loc, compiler.storage.STACK)
            return loc
        self._set_passed_value_callback(assign_compiler)

    def compile_nonlocal_return(self):
        self.push_that()
        depth = 0  # TODO: Calculate
        self._codegen.add_return(depth)
    
    def push_that(self):
        self._pull_passed_value(compiler.storage.STACK)

    def grab_var_contents(self, varname):
        # TODO: Find variable, return True or False depending on success
        return False
    
    def send_that_to_var(self, varname):
        # TODO: Find variable, return True or False depending on success
        return False

    def finish(self):
        self.push_that()  # TODO: Is this correct in every case?
        return self._codegen.finish()
    
    def gen_visitor(self):
        return compiler.visitor.ASTVisitor(self)
    
    def __init__(self):
        self._passed_value_callback = None
        self._codegen = compiler.codegenerator.CodeGenerator()
        self._regalloc = compiler.storage.RegisterAllocator()
