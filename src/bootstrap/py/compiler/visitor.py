
class ASTVisitor:

    def visit_invalid(self, elem):
        raise Exception('Visitor visiting invalid element!')
    
    def visit_self(self, elem):
        pass

    def visit_nil(self, elem):
        pass

    def visit_constant(self, elem):
        pass

    def visit_send(self, elem):
        pass

    def visit_assign(self, elem):
        pass

    def visit_compound(self, elem):
        pass

    def visit_block(self, elem):
        pass
    
    def __init__(self):
        pass
