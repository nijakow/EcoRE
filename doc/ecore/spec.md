# The Standard Library

## Mirrors

Every object possesses exactly one _mirror_, a special kind of object by which the structure and properties of the mirrored object can be modified.

A mirror is created by calling the `Mirror:` standard function:

`| mirror <- Mirror: object |`

When a mirror is initialized, it gets _bound_ to the object it was created for.

### Methods available for mirrors

`reflectee`

Get the object the mirror is referencing.

`slots`

Get an immutable vector with references to all object slots.

`slots_do: [[slot] => ]`

Iterate over all object _slots_. Same as `slots do: function`.

`slot: [key]`

Look up a slot by the key specified as the parameter.

`slot: [key] put: []`

The same as `(slot: key) put: value`.

`add_slot: [key]`

Add a new slot. Its name is specified as a parameter. The slot is _invisible_ by default, no other flags are set.

`remove_slot: [key]`

The same as `(slot: key) remove`.
