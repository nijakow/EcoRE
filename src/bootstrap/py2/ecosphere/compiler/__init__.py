import ecosphere.compiler.scopes
import ecosphere.compiler.visitors
import ecosphere.compiler.codegen

class Compiler:

    def __init__(self, ast, parent_env=None):
        self._environment = ecosphere.compiler.scopes.RootEnvironment(parent_env)

def compile_ast(the_ast, loader, parameters=list(), has_varargs=False, parent_env=None):
    environment = ecosphere.compiler.scopes.RootEnvironment(parent_env)
    for param in parameters:
        environment.add_parameter(param)
    writer = ecosphere.compiler.codegen.CodeWriter()
    codegen = ecosphere.compiler.codegen.CodeGenerator(writer, environment)
    visitor = ecosphere.compiler.visitors.ASTCompilerVisitor(codegen, environment, loader)
    the_ast.accept(visitor)
    return codegen.finish()
