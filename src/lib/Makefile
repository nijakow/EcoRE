ifeq ($(PYTHON),)
PYTHON := python3
endif

NAME = master.ebf
SOURCES = \
init.eco \
std/_.eco \
std/bool.eco \
std/clonable.eco \
std/defaultbehavior.eco \
std/globals.eco \
std/ioutil.eco \
std/maybe.eco \
std/util.eco \
std/reflect.eco \
interfaces.eco \
lang/lang.eco \
lang/ast/ast.eco \
lang/ast/construction.eco \
lang/ast/visitor.eco \
lang/compiler/_.eco \
lang/compiler/bytecodes.eco \
lang/compiler/codewriter.eco \
lang/compiler/compiler.eco \
lang/compiler/regalloc.eco \
lang/compiler/scope.eco \
lang/compiler/scoped_compiler.eco \
lang/compiler/value_type.eco \
lang/compiler/visitor.eco \
lang/parser/Parser/objects/interface.eco \
lang/parser/Parser/objects/molecule.eco \
lang/parser/Parser/objects/simple.eco \
lang/parser/Parser/args.eco \
lang/parser/Parser/bars.eco \
lang/parser/Parser/charchecks.eco \
lang/parser/Parser/checks.eco \
lang/parser/Parser/compounds.eco \
lang/parser/Parser/expressions.eco \
lang/parser/Parser/optionals.eco \
lang/parser/Parser/send.eco \
lang/parser/_.eco \
lang/parser/reader.eco \
lang/parser/shared.eco \
lang/parser/tokenizer.eco \
master.eco


$(NAME): $(SOURCES)
	PYTHONPATH=../bootstrapper $(PYTHON) -m ecosphere.bootstrapper --compressed $(NAME) $(SOURCES)

all: $(NAME)


clean:
	- $(RM) $(NAME)

re: clean all
