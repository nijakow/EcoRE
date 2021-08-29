# The EConnect Protocol

The EConnect protocol the binary file format responsible for storing and transmitting objects that live in the Ecosphere. It was designed to be simple, easy to parse and expandable.

## File Endings and Conventions

The default file extension for EConnect binary files is `.ebf`. In its current iteration the EConnect protocol has no magic number or similar mechanisms to provide a way of determining the version of EConnect that's being used. All metadata is optional.

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

Byte sequences are preceeded by an unsigned integer denoting their count. There is no limitation to their length. Null bytes (`'\0'`) may be used inside of byte sequences.

Example:

`0x05 'H' 'e' 'l' 'l' 'o'` 

### Messages

Messages are a mechanism to represent more complex data structures. They start with an unsigned integer denoting a _binding_ to a key, followed by more fields based on the data type defined by the referenced key.

Example:

`0x01 <optional>`

> Note: Since the binding `0x00` is special, using `0x00` as a message ID always references the _Standard Key Message_.
