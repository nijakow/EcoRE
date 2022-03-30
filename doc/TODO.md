# TODOs for EcoRE

## Things to implement

### Byte-related stuff
 - std util ByteVector (based on std util Vector -> shared trait?)
 - std io ByteBufferWriter (implements IByteWriter, returns a blob)
 - std lang eco code CodeWriter (implements %[addByte: b, addConst: c, addCode: c, finish])
 - std lang eco code BasicCodeWriter (extends CodeWriter, but implements all basic bytecodes)

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
