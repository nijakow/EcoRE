import datatypes
import econnect

d = datatypes.PlainObject()
d.add_slot(datatypes.ValueSlot('hello', datatypes.Key.get('hello')))
d.add_slot(datatypes.ValueSlot('hey', datatypes.Key.get('hey')))
d.add_slot(datatypes.ValueSlot('hoy', datatypes.Key.get('hello')))

c = datatypes.Code(4, 2, 1, [d], [], [122, 243, 18, 0, 232])

s = econnect.Serializer()

c.serialize(s)
print(s.finish())
