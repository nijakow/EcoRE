
class Scope:

    def add_var(self, key, is_param=False):
        self._vars[key] = self._register_allocator.allocate_register()
        self._increment_var_count()

    def get_var(self, key):
        if key in self._vars:
            return self._vars[key]
        else:
            return self._find_var(key)

    def __init__(self):
        self._vars = dict()


class SubScope(Scope):

    def get_parent(self):
        return self._parent

    def _increment_var_count(self):
        self.get_parent()._increment_var_count()

    def _find_var(self, key):
        return self.get_parent().get_var(key)

    def __init__(self, parent):
        super().__init__()
        self._parent = parent


class BaseScope(Scope):

    def _increment_var_count(self):
        self._var_count += 1

    def get_var_count(self):
        return self._var_count
    
    def get_lexical(self):
        return self._lexical
    
    def get_lexical_depth(self):
        depth = 0
        obj = self
        while obj.get_lexical() is not None:
            obj = obj.get_lexical()
            depth += 1
        return depth

    def _find_var(self, key):
        if self.get_lexical() is None:
            return None
        p = self.get_lexical().get_var(key)
        if p is None:
            return None
        else:
            self._vars[key] = p.increment_depth()

    def __init__(self, register_allocator, lexical=None):
        super().__init__()
        self._register_allocator = register_allocator
        self._lexical = lexical
        self._var_count = 0
