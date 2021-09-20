
class VarManager:

    def bind(self, var, storage_location):
        self._bindings[var] = storage_location

    def get_binding(self, var):
        return self._bindings.get(var)

    def __init__(self):
        self._bindings = dict()


class Environment:

    def get_var_manager(self):
        return self._vars

    def create_subenv(self):
        return SubEnvironment(self)

    def create_subrootenv(self):
        return RootEnvironment(self)

    def bind(self, var, storage_location):
        self.get_var_manager().bind(var, storage_location)

    def get_binding(self, var):
        storage_location = self.get_var_manager().get_binding(var)
        if storage_location:
            return storage_location, 0
        else:
            return self._lookup(var)

    def _lookup(self, var):
        if self._direct_parent is not None:
            return self._direct_parent.get_binding(var)
        else:
            return None

    def __init__(self, var_manager, parent):
        self._vars = var_manager
        self._direct_parent = parent

class RootEnvironment(Environment):

    def _lookup(self, var):
        if self._indirect_parent is not None:
            storage_location, depth = self._indirect_parent.get_binding(var)
            if storage_location is not None:
                return (storage_location, (depth + 1))
        return None

    def __init__(self, parent=None):
        super().__init__(VarManager(), None)
        self._indirect_parent = parent

class SubEnvironment(Environment):

    def __init__(self, parent):
        super().__init__(parent.get_var_manager(), parent)

