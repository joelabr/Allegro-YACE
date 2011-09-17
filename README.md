Allegro YACE
============
Allegro YACE is a front end for YACE (Yet Another Chip8 Emulator) written in C++ using Allegro 5.

Usage
-----
###Running
To run Allegro YACE, type the following command in a terminal\\command line (inside of the Allegro YACE directory):
`yace [<game>] [<configuration-file>]`

\[<game>\] - The Chip8 game to run. **(Optional)**
\[<configuration-file>\] - The configuration file to use. **(Optional)**

The easiest way to run Allegro YACE is simply by typing `yace` or double-clicking the executable.

###Keys
`ESCAPE - Exit Allegro YACE`
`F1 - Load game`

###Configurations
In the configuration file it's possible to change key-bindings (and soon BG-/FG-color). It's possible to have several configuration files, however, only one can be used at a time.

Compiling
---------
There's a *makefile* provided with Allegro YACE. It's very basic and doesn't allow for adding new source files without having to add them manually into the makefile. In order to build Allegro YACE, the following things are required:

+ [YACE Back end](https://github.com/Johoel/YACE)
+ Allegro 5.0.4

1. Open a terminal\\command line
2. Change directory to where Allegro YACE is

*The folder structure should look like this:*
    `include/
    src/
    YACE/
    --include/
    --src/`

3.  a\) To compile Allegro YACE **without** debugging enabled, simply write `make ALLEGRO_PATH=<path-to-allegro>`. The output file will be named *yace*

    b\) To compile Allegro YACE **with** debugging enabled, simply write `make debug ALLEGRO_PATH=<path-to-allegro>`. The output file will be named *yace-debug*.

To-Do list
----------
+ Add possibility to set user-defined colors in configuration file (default is White BG & Black FG).
