import ecosphere.compiler.storage
import ecosphere.compiler.scope
import ecosphere.compiler.codegenerator
import ecosphere.compiler.visitor


class Compiler:

    def _pull_passed_value(self, storage_loc):
        if self._passed_value_callback is None:
            self.compile_load_self()
        self._passed_value_callback(storage_loc)
        def next(loc):
            return None
        self._passed_value_callback = next

    def _drop_passed_value(self):
        if self._passed_value_callback is not None:
            self._pull_passed_value(None)

    def _set_passed_value_callback(self, cb):
        self._passed_value_callback = cb

    def _compile_transfer(self, dest, src):
        if dest is src:
            return
        elif dest is None:
            if src.is_stack():
                self._codegen.add_drop()
            else:
                pass # Do nothing, that's okay
        elif dest.is_stack():
            if src.is_register():
                if src.is_local_register(self._current_scope):
                    self._codegen.add_pushr(src.get_id())
                else:
                    reg = self._regalloc.allocate_temporary_register()
                    self._compile_transfer(reg, src)
                    self._compile_transfer(dest, reg)
                    reg.free()
            else:
                raise Exception('Unimplemented!')
        elif dest.is_register():
            if dest.is_local_register(self._current_scope):
                if src.is_stack():
                    self._codegen.add_popr(dest.get_id())
                elif src.is_register():
                    if src.is_local_register(self._current_scope):
                        self._codegen.add_r2r(dest.get_id(), src.get_id())
                    else:
                        self._codegen.add_l2r(dest.get_id(), src.get_id(), src.get_depth())
                else:
                    raise Exception('Unimplemented!')
            else:
                reg = self._regalloc.allocate_temporary_register()
                self._compile_transfer(reg, src)
                self._compile_transfer(dest, reg)
                reg.free()
        else:
            raise Exception('Unimplemented!')
    
    def compile_load_self(self):
        self._drop_passed_value()
        def self_compiler(loc):
            if loc is not None:
                self._compile_transfer(loc, self._self_register)
            return loc
        self._set_passed_value_callback(self_compiler)

    def compile_load_constant(self, value):
        self._drop_passed_value()
        def constant_compiler(loc):
            if loc is None:
                return None
            elif loc.is_local_register(self._current_scope):
                self._codegen.add_const2r(loc.get_id(), value)
                return loc
            elif loc.is_stack():
                self._codegen.add_pushc(value)
            else:
                reg = self._regalloc.allocate_temporary_register()
                self._codegen.add_const2r(reg.get_id(), value)
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
            elif loc.is_local_register(self._current_scope):
                self._codegen.add_popr(loc.get_id())
                return loc
            else:
                reg = self._regalloc.allocate_temporary_register()
                self._codegen.add_popr(reg.get_id())
                self._compile_transfer(loc, reg)
                reg.free()
                return loc
        self._set_passed_value_callback(drop_compiler)
    
    def compile_var_declaration(self, name):
        self.add_var(name)
        self.send_that_to_var(name)
    
    def compile_builtin(self, args, key, has_varargs=False):
        self._codegen.add_builtin(args, key, has_varargs=has_varargs)
        def builtin_compiler(loc):
            self._compile_transfer(loc, ecosphere.compiler.storage.STACK)
            return loc
        self._set_passed_value_callback(builtin_compiler)

    def compile_send(self, args, key, has_varargs=False):
        self._codegen.add_send(args, key, has_varargs=has_varargs)
        def send_compiler(loc):
            self._compile_transfer(loc, ecosphere.compiler.storage.STACK)
            return loc
        self._set_passed_value_callback(send_compiler)

    def compile_slot_assignment(self, key):
        self._codegen.add_assign(key)
        def assign_compiler(loc):
            self._compile_transfer(loc, ecosphere.compiler.storage.STACK)
            return loc
        self._set_passed_value_callback(assign_compiler)

    def compile_assign(self, key):
        self.push_that()
        def assign_compiler(loc):
            self._codegen.add_assign(key)
            self._compile_transfer(loc, ecosphere.compiler.storage.STACK)
            return loc
        self._set_passed_value_callback(assign_compiler)

    def compile_nonlocal_return(self):
        self.push_that()
        depth = self._root_scope.get_lexical_depth()
        self._codegen.add_return(depth)
    
    def compile_local_return(self):
        self.push_that()
        self._codegen.add_return(0)

    def compile_make_closure(self, code):
        self._drop_passed_value()
        def closure_compiler(loc):
            if loc is None:
                return None
            elif loc.is_local_register(self._current_scope):
                self._codegen.add_make_closure(loc.get_id(), code)
            else:
                reg = self._regalloc.allocate_temporary_register()
                self._codegen.add_make_closure(reg.get_id(), code)
                self._compile_transfer(loc, reg)
                reg.free()
                return loc
        self._set_passed_value_callback(closure_compiler)
    
    def push_that(self):
        self._pull_passed_value(ecosphere.compiler.storage.STACK)

    def grab_var_contents(self, varname):
        reg = self._current_scope.get_var(varname)
        if reg is not None:
            self._drop_passed_value()
            def var_compiler(loc):
                self._compile_transfer(loc, reg)
                return loc
            self._set_passed_value_callback(var_compiler)
            return True
        else:
            return False
    
    def send_that_to_var(self, varname):
        reg = self._current_scope.get_var(varname)
        if reg is not None:
            self._pull_passed_value(reg)
            def cb(loc):
                self._compile_transfer(loc, reg)
            self._set_passed_value_callback(cb)
            return True
        else:
            return False

    def push_scope(self):
        self._current_scope = ecosphere.compiler.scope.SubScope(self._current_scope)

    def pop_scope(self):
        self._current_scope = self._current_scope.get_parent()

    def add_parameter(self, expr):
        self._root_scope.add_var(expr.get_message())  # Assuming it's a SEND expression
        self._parameter_count += 1
    
    def add_var(self, varname):
        self._root_scope.add_var(varname)
    
    def enable_varargs(self):
        self._has_varargs = True
    
    def finish(self):
        self.compile_local_return() # TODO: Only if the last instruction wasn't a return
        return self._codegen.finish(self._regalloc.get_register_count(),
                                    self._parameter_count,
                                    has_varargs=self._has_varargs)
    
    def gen_visitor(self):
        return ecosphere.compiler.visitor.ASTVisitor(self, self._meta)

    def gen_subcompiler(self):
        return Compiler(self._meta, lexical_parent_scope=self._current_scope)
    
    def __init__(self, meta, lexical_parent_scope=None):
        self._meta = meta
        self._passed_value_callback = None
        self._codegen = ecosphere.compiler.codegenerator.CodeGenerator()
        self._regalloc = ecosphere.compiler.storage.RegisterAllocator()
        self._self_register = self._regalloc.allocate_register()
        self._root_scope = ecosphere.compiler.scope.BaseScope(self._regalloc, lexical=lexical_parent_scope)
        self._current_scope = self._root_scope
        self._parameter_count = 1  # The SELF
        self._has_varargs = False
