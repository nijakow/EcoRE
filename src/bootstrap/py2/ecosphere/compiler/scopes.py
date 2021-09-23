import ecosphere.compiler.storage

class Environment:

    def get_storage_manager(self):
        return self._storage

    def create_subenv(self):
        return SubEnvironment(self)

    def create_subrootenv(self):
        return RootEnvironment(self)

    def bind(self, var, storage_location=None):
        self.get_storage_manager().bind(var, storage_location)

    def get_binding(self, var):
        storage_location = self.get_storage_manager().get_binding(var)
        if storage_location:
            return storage_location
        else:
            return self._lookup(var)

    def _lookup(self, var):
        if self._direct_parent is not None:
            return self._direct_parent.get_binding(var)
        else:
            return None

    def __init__(self, storage_manager, parent):
        self._storage = storage_manager
        self._direct_parent = parent

class RootEnvironment(Environment):

    def _lookup(self, var):
        if self._indirect_parent is not None:
            storage_location = self._indirect_parent.get_binding(var)
            if storage_location is not None:
                inc = storage_location.increment_depth()
                self.bind(var, inc)  # Keep a reference to the object to allow for comparisons
                return inc
        return None

    def __init__(self, parent=None):
        super().__init__(ecosphere.compiler.storage.StorageManager(), None)
        self._indirect_parent = parent

class SubEnvironment(Environment):

    def __init__(self, parent):
        super().__init__(parent.get_storage_manager(), parent)

