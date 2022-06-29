import ecosphere.compiler.storage

class Environment:

    def get_storage_manager(self):
        return self._storage

    def create_subenv(self):
        return SubEnvironment(self)

    def create_subrootenv(self):
        return RootEnvironment(self)

    def bind(self, var, the_type, storage_location=None):
        if storage_location is None:
            storage_location = self.get_storage_manager().allocate()
        self._bindings[var] = (the_type, storage_location)

    def get_binding(self, var):
        if var in self._bindings:
            return self._bindings[var]
        else:
            return self._lookup(var)

    def _lookup(self, var):
        if self._direct_parent is not None:
            return self._direct_parent.get_binding(var)
        else:
            return (None, None)
    
    def get_depth(self):
        return self._direct_parent.get_depth()

    def __init__(self, storage_manager, parent):
        self._storage = storage_manager
        self._direct_parent = parent
        self._bindings = dict()

class RootEnvironment(Environment):

    def _lookup(self, var):
        if self._indirect_parent is not None:
            the_type, storage_location = self._indirect_parent.get_binding(var)
            if storage_location is not None:
                inc = storage_location.increase_depth()
                self.bind(var, the_type, inc)  # Keep a reference to the object to allow for comparisons
                return (the_type, inc)
        return (None, None)

    def add_parameter(self, key, the_type):
        self._bindings[key] = (the_type, ecosphere.compiler.storage.Arg(self._parameter_count))
        self._parameter_count += 1
    
    def enable_varargs(self):
        self._has_varargs = True
    
    def get_parameter_count(self):
        return self._parameter_count
    
    def has_varargs(self):
        return self._has_varargs
    
    def get_depth(self):
        if self._indirect_parent is None:
            return 0
        else:
            return self._indirect_parent.get_depth() + 1

    def __init__(self, parent=None):
        super().__init__(ecosphere.compiler.storage.StorageManager(), None)
        self._indirect_parent = parent
        self._parameter_count = 0
        self._has_varargs = False

class SubEnvironment(Environment):

    def __init__(self, parent):
        super().__init__(parent.get_storage_manager(), parent)

