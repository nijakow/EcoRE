# EcoRE - The Ecosphere Runtime Environment

This is *EcoRE*, my little programming language playground that I've been working on in my spare time since 2020.

It acts as a virtual machine and runtime environment for my programming language *Eco*, which is a language inspired by [Self](https://selflanguage.org/).


## Building and Running EcoRE

If you are using Debian or Fedora, it should be enough to type

```sh
make install-deps
make run
```

while in the toplevel directory of the repository and the system will take care of the rest.

EcoRE depends on:
 - SDL2
   - SDL2
   - SDL2_ttf
   - SDL2_image
   - SDL2_gfx
 - zlib
 - libffi

You might have to change a few things in order to get it to run on your machine.
