
class Environment:

    def get_var_manager(self):
        return self._vars

    def __init__(self, var_manager, parent):
        self._vars = var_manager
        self._direct_parent = parent

class RootEnvironment(Environment):

    def __init__(self, parent=None):
        super().__init__(VarManager(), None)
        self._indirect_parent = parent

class SubEnvironment(Environment):

    def __init__(self, parent):
        super().__init__(parent.get_var_manager(), parent)

