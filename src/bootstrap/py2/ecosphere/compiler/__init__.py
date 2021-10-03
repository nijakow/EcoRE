import ecosphere.compiler.scopes
import ecosphere.compiler.visitors
import ecosphere.compiler.codegen

class Compiler:

    def __init__(self, ast, parent_env=None):
        self._environment = ecosphere.compiler.scopes.RootEnvironment(parent_env)

def compile_ast(the_ast, parameters=list()):
    environment = ecosphere.compiler.scopes.RootEnvironment()
    writer = ecosphere.compiler.codegen.CodeWriter()
    codegen = ecosphere.compiler.codegen.CodeGenerator(writer)
    visitor = ecosphere.compiler.visitors.ASTVisitor(codegen, environment)
    the_ast.accept(visitor)

