# TODOs for EcoRE

## Things to implement

### I/O
 - `io SlurpFile:` should be replaced with a wrapper around a "read file as bytes/blob" mechanism

### Byte- and Vector-related stuff
 - Iterators!
 - `std util ByteVector` (based on `std util Vector` -> shared trait?)
 - `std io ByteBufferWriter` (implements `IByteWriter`, returns a blob)
 - `std lang eco code CodeWriter` (implements `%[addByte: b, addConst: c, addCode: c, finish]`)
 - `std lang eco code BasicCodeWriter` (extends `CodeWriter`, but implements all basic bytecodes)

### Interfaces
 - Let interfaces inherit from each other
 - Check if an interface contains a selector
 - Check if an interface extends another interface
 - Get a list of interfaces inheriting an object (explicitly and implicitly)
 - Get the interface an object implements
 - Allow to specify the SELF type in an interface

### FFI
 - Implement a proper FFI system
 - Idea: Create a blob for the call, and insert the raw pointers
   by using a special method similar to at: index putInt32/64: ptr.
   This method takes the object, tries to write it into the buffer
   at a specific offset, and returns the amount of bytes written
   so that the next call can start at the new offset.
   The locations of these bytes are then stored as a list of pointers
   at the beginning of the blob (the amount of arguments is known,
   so the space can be reserved).
   This could also be implemented in C.

## Other improvements
 - Multiple fibers
 - Multiple threads
 - Fibers pause when waiting for a file descriptor
