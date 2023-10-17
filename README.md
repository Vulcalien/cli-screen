# libcliscreen

*libcliscreen* is a C library that offers functions to interact with the
terminal as if it were a raster display.

## Building the library
To build the library, clone this git repository, move into its directory
and run `make`.

```shell
git clone https://github.com/Vulcalien/libcliscreen.git
cd libcliscreen
make
```

This will generate two files: the static library `libcliscreen.a` and
the shared library `libcliscreen.so` (or `libcliscreen.dll` on Windows).

## Using the library
To use *libcliscreen* you will need to include the `cliscreen.h` header
file.

```c
#include <cliscreen.h>
```

Also, make sure to link your executable with either the static or
shared library file. If you don't already have one of those files, see
the 'Building the library' section above.

### Setup the screen
To initialize the screen, call `cliscreen_create()` and then
`cliscreen_terminal_prepare()`. After that, call
`cliscreen_setsize(w, h)` to set the screen's size.

```c
cliscreen_create();
cliscreen_terminal_prepare();

cliscreen_setsize(40, 16);
```

### Draw on the screen
There are functions that can be used to put characters on the screen.
- *cliscreen_clear*(char c, const char \*color):\
    Fill the entire screen with the character 'c' and the color 'color',
    which can be an [ANSI escape code](https://en.wikipedia.org/wiki/ANSI_escape_code#Colors)
    or **NULL** for the default colors.
- *cliscreen_setchar*(int x, int y, char c, const char \*color):\
    Set a single character at the point (x, y).
- *cliscreen_fill*(int x0, int y0, int x1, int y1,
                   char c, const char \*color):\
    Fill a rectangle, given its top-left and bottom-right corners
    (x0, y0) and (x1, y1) with the character 'c'.
- *cliscreen_puts*(int x, int y,
                   const char \*str, const char \*color):\
    Write a string starting at the given position (x, y).
- *cliscreen_printf*(int x, int y
                     const char \*color, const char \*format, ...):\
    Write a formatted string starting at the given position (x, y).

```c
cliscreen_clear('.', NULL);                 // fill the screen with '.'
cliscreen_setchar(5, 5, 'A', "\033[1;31m"); // set a red 'A' at (5, 5)
cliscreen_fill(1, 3, 4, 5, ' ', NULL);      // fill a square of spaces
cliscreen_puts(0, 0, "Example", NULL);      // write "Example"

int n = 3;
cliscreen_printf(10, 10, NULL, "n=%d", n);  // write the value of n
```

### Update the screen
In order to redraw the screen, call `cliscreen_render()`. You can call
this function as much as you need, but it's most efficient to group the
changes together and then call *cliscreen_render* once.

This library was made with terminal games in mind, so calling
*cliscreen_render* 60 times a second (or more) is not discouraged.

```c
cliscreen_render();
```

### Destroy the screen
Once the screen is no longer needed, it should be destroyed with
`cliscreen_destroy()` to free any allocated memory. Then, call
`cliscreen_terminal_reset()`.

```c
cliscreen_destroy();
cliscreen_terminal_reset();
```

## More
Find out more about these and other functions by reading the
[documentation](https://github.com/Vulcalien/libcliscreen/tree/master/doc).

Download a [demo application](https://github.com/Vulcalien/libcliscreen/releases/download/0.5.0/libcliscreen-0.5.0-demo.zip)
or look at [Snake-CLI](https://github.com/Vulcalien/Snake-CLI), a game I
made using this library.

## License

libcliscreen is free software, released under the terms of the GNU
Lesser General Public License, version 3 only.
