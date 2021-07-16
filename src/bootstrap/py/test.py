import datatypes
import econnect
import parser

d = datatypes.PlainObject()
d.add_slot(datatypes.ValueSlot('meta', datatypes.PlainObject()))
d.add_slot(datatypes.ValueSlot('x', datatypes.Key.get('xval')))
d.add_slot(datatypes.ValueSlot('y', datatypes.Key.get('yval')))
d.add_slot(datatypes.ValueSlot('z', datatypes.Key.get('zval')))

c = datatypes.Code(4, 2, 1, [d], [], [122, 243, 18, 0, 232])

s = econnect.Serializer()

c.serialize(s)
print(s.finish())


the_parser = parser.TextStream('''
foo: System out println(text) * 2 bar: [x, y, z => 3](1, 2, 3)
''').get_parser()

print(the_parser.parse_expression())
