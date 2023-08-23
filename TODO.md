# List of TODOs for Eco and EcoRE

### System
- Fix finalization and weak references (soft marking)
- `Fiber`s as first-class objects
  - `synchronized: [ ... ]`
- Files and streams
  - Restructure `Path`
  - `File` objects
  - `Stream` objects
  - Scheduler support for file descriptors, networking, ...

### UI
- Rework `ui builder`:
  - Add a `WindowBuilder`, remove `Window(w, h, ...)` and others
  - Rework resizing operations of windows
- Add a color theme
- Rework morphs (make them more modular)
  - Implement `Listener` properly
  - Refactor morph methods
  - Add support for
- Decouple `eve`, implement `fonts`
- Implement a presentation mechanism (similar to Lisp Machines)
- Add object representations (as an addition to outliners, --> [presentations]())
- Implement a mechanism for Drag&Drop
  - `HandMorph grab: [Morph]` should be converted into `HandMorph grab: [Morph] meta: [Metadata]` or
    maybe even `HandMorph grab: [DragDropInfo]`
- Add support for dragging and dropping object instances around
- Add support for building UI through drag&drop
- Add support for creating slots in outliners

### Language Infrastructure
 - Rework reflection, improve `Mirror`s
 - Type system
   - Introduce `Concept`s
   - `%[]` interfaces should be reworked
   - "EcoSense": Ask the system about refactoring info
 - Compiler and Optimizer
   - Implement a new, full and flexible AST
   - Update the `Decompiler` to analyze `cast:` calls
   - Implement AST rewriting for jump instructions and typechecks
   - Wire "EcoSense" into the optimizer
   - Set up a backend for code generation (should support live-patching)

### Miscellaneous
 - `WeakSet`s, `WeakMap`s, `NotificationList`s

### Applications and Tools
 - Better Core Sampler
 - Debugger
 - Frontend to "EcoSense"


# Roadmap

## Timeline

 1) Minor UI refactoring and improvements
    - `WindowBuilder` [done]
    - Color theme [done]
    - Morph refactoring [skip]
 2) Get weak data structures right
    - `WeakSet`s, `WeakMap`s, `NotificationList`s
 3) Implement reflection, types, concepts, interfaces and most basic parts of "EcoSense"
 4) Use this for the implementation of drag&drop
 5) Implement draggable objects and tools
 6) Implement files
 7) Set up the AST and language tools (decompiler, optimizer, compiler, file I/O, EConnect, ...)

Side tasks:
 - Python support
 - DWARF support

Later:
 - Add animations
 - Generate image files from within Eco
 - Rewrite EcoRE
