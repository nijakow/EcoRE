import datatypes
import econnect
import parser.parser


the_parser = parser.parser.TextStream('''
( foo bar: x baz: y; self; self; self; {} clone; quux )
''').get_parser()

expr = the_parser.parse_expression()
code = expr.compile_as_code()

print(code._instructions)
for c in code._constants:
    print('    ', c)

s = econnect.Serializer()
code.serialize(s)
print(s.finish())
