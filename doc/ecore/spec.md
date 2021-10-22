# Naming Conventions

Most names should consist of only one word. In cases where multiple words are needed, these rules apply:

 - Slot names and methods are written using `snake_case` or `lowerCamelCase`: `mirror remove_slot: slot_name`. When continuing split method names, lower case is used: `vector at: 3 put: 'hi'`. Messages consisting of very short subwords are written using `camelCase`: `5 isEven`, `result ifTrue: b1 ifFalse: b2`.
 - Local variables and parameters are written using `snake_case` or `lowerCamelCase`: `| param_count <- 42 |`, `load: configFilePath`.
 - Function names are written using `UpperCamelCase`: `SquareRoot(x)`, `SquareRoot: x`, `Display: entity On: screen`.

# The Standard Library

## Objects (std Object)

The base for every object.

### Methods available for objects

`init`

Initializes the object.

`clone`

Creates a deep copy of the object.

`mirror`

Returns the mirror of the object.

`[bool] asBool`

Converts the object into either `true` or `false`.

## Booleans (std bool true, std bool false)

Values representing either true or false. Booleans are marked as _atomic_.

### Methods available for booleans

`if: t else: f`

If `self` is `true`, call the first parameter on `self`. If `self` is `false`, call the second parameter on `self`. Return the value of the call.

`if: b`

Same as `if: b else: [ false ]`.

`unless: b`

Same as `if: [ true ] else: b`.

`true: t false: f`

Same as `if: t else: f`.

`true: b`

Same as `if: b`.

`false: b`

Same as `unless: b`.

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
