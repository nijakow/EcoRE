import ecosphere.compiler.scopes

class Compiler:

    def __init__(self, ast, parent_env=None):
        self._environment = ecosphere.compiler.scopes.Environment(parent_env)

