# Naming Conventions

Most names should consist of only one word. In cases where multiple words are needed, these rules apply:

 - Slot names and methods are written using `snake_case`: `mirror remove_slot: slot_name`. When continuing split method names, lower case is used: `vector at: 3 put: 'hi'`.
 - Local variables and parameters are written using `snake_case` or `lowerCamelCase`: `| param_count <- 42 |`, `load: configFilePath`.
 - Function names are written using `UpperCamelCase`: `SquareRoot(x)`, `SquareRoot: x`, `Display: entity On: screen`.

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
