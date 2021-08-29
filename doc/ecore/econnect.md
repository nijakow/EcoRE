# The EConnect Protocol

The EConnect protocol the binary file format responsible for storing and transmitting objects that live in the Ecosphere. It was designed to be simple, easy to parse and expandable.

## File Endings and Conventions

The default file extension for EConnect binary files is `.ebf`. In its current iteration the EConnect protocol has no magic number or similar mechanisms to provide a way of determining the version of EConnect that's being used. All metadata is optional.

## The Binding Table

The EConnect protocol is centered around the _Binding Table_, a mapping from unsigned integers to objects. When serializing an object to the EConnect format there is an option to insert it into the Binding Table, giving it a unique ID until the binding is dissolved or overwritten by another object. It is possible to refer to already bound objects from within the file. This is especially important for [Messages](#messages).

Example:

| ID      | Value                                    |
|---------|------------------------------------------|
| 0       | _always reserved_                        |
| 1       | _uninitialized_                          |
| 2       | `<key:'ecosphere.econnect.id'>`          |
| 3       | `<object:...>`                           |
| 4       | _uninitialized_                          |
| 5       | `<object:nil>`                           |
| 6       | _uninitialized_                          |
| ...     | ...                                      |


## Atomic Types and Encodings

The EConnect protocol supports these basic encodings:

### Unsigned Integers

Unsigned integers are represented as a sequence of one or more bytes using a big-endian [VLQ format](https://en.wikipedia.org/wiki/Variable-length_quantity). There is no limitation to their length.

Example:

| N      | Representation                        |
|--------|---------------------------------------|
| 0      | `0x00`                                |
| 1      | `0x01`                                |
| ...    | ...                                   |
| 127    | `0x7f`                                |
| 128    | `0x81 0x00`                           |
| 129    | `0x81 0x01`                           |
| ...    | ...                                   |
| 255    | `0x81 0x7f`                           |
| 256    | `0x82 0x00`                           |
| ...    |                                       |

### Byte sequences

Byte sequences are preceeded by an unsigned integer denoting their length. There is no limitation to their length. Null bytes (`'\0'`) may be used inside of byte sequences.

Example:

`0x05 'H' 'e' 'l' 'l' 'o'` 

### Messages

Messages are a mechanism to represent more complex data structures. They start with an unsigned integer denoting a [binding](#the-binding-table) to a key, followed by more fields based on the data type defined by the referenced key.

Example:

`0x01 <optional>`

> Note: Since the binding `0x00` is special, using `0x00` as a message ID always references the _Standard Key Message_.
