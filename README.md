# EcoRE - The Ecosphere Runtime Environment

This is **EcoRE**, my little programming language playground that I've been working on in my spare time since 2020.

It acts as a virtual machine and runtime environment for my programming language **Eco**, which is a language inspired by [Self](https://selflanguage.org/).


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

In order to create the bootstrap image, you also need Python (version 3).

You might have to change a few things in order to get it to run on your machine.

# Screenshots and Impressions

![A screenshot of EcoRE booting](./doc/repo/resources/eco_loading.png)

![Eco with its standard theme](./doc/repo/resources/eco_standard_theme.png)

![Eco with its pumpkin theme](./doc/repo/resources/eco_pumpkin_theme.png)

![Dragging objects](./doc/repo/resources/eco_dragging_objects.gif)

![Animation](./doc/repo/resources/eco_animation.gif)

![Outliners](./doc/repo/resources/eco_outliners.gif)
