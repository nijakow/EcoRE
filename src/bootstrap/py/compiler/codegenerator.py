import datatypes


class CodeGenerator:

    def finish(self):
        return datatypes.Code(0,
                              0,
                              self._constants,
                              self._subcodes,
                              self._instructions)
    
    def __init__(self):
        self._instructions = []
        self._constants = []
        self._subcodes = []
