import datatypes
import econnect
import parser

d = datatypes.PlainObject()
d.add_slot(datatypes.ValueSlot('meta', datatypes.PlainObject()))
d.add_slot(datatypes.ValueSlot('x', datatypes.Key.get('xval')))
d.add_slot(datatypes.ValueSlot('y', datatypes.Key.get('yval')))
d.add_slot(datatypes.ValueSlot('z', datatypes.Key.get('zval')))

c = datatypes.Code(4, 1, [d], [], [122, 243, 18, 0, 232])

s = econnect.Serializer()

c.serialize(s)
print(s.finish())


the_parser = parser.TextStream('''
foo: {x, y, with z = {}, with code => x}
''').get_parser()

print(the_parser.parse_expression())
