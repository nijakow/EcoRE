import datatypes
import econnect
import parser.stream


the_parser = parser.stream.TextStream('''
{
    x,
    hello = x
}
''').get_parser()

expr = the_parser.parse()
code = expr.compile_as_code()

print(code._instructions)
for c in code._constants:
    print('    ', c)

s = econnect.Serializer()
code.serialize(s)
print(s.finish())
