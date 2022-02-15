ifeq ($(PYTHON),)
PYTHON := python3
endif

NAME = master.ebf
SOURCES = \
init.eco \
std/std.eco \
std/io.eco \
std/util.eco \
std/reflect.eco \
interfaces.eco \
lang/lang.eco \
lang/ast/ast.eco \
lang/ast/construction.eco \
lang/compiler/bytecodes.eco \
lang/compiler/codewriter.eco \
lang/compiler/compiler.eco \
lang/parser/parts/objects/interface.eco \
lang/parser/parts/objects/molecule.eco \
lang/parser/parts/objects/simple.eco \
lang/parser/parts/args.eco \
lang/parser/parts/bars.eco \
lang/parser/parts/charchecks.eco \
lang/parser/parts/checks.eco \
lang/parser/parts/compounds.eco \
lang/parser/parts/expressions.eco \
lang/parser/parts/optionals.eco \
lang/parser/parts/send.eco \
lang/parser/parser.eco \
lang/parser/shared.eco \
lang/parser/tokenizer.eco \
master.eco


$(NAME): $(SOURCES)
	PYTHONPATH=../bootstrapper $(PYTHON) -m ecosphere.bootstrapper $(NAME) $(SOURCES)

all: $(NAME)


clean:
	- $(RM) $(NAME)

re: clean all
