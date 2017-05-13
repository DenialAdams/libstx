## libstx - Dynamic String Handling Library

## How to install

Before installing, the file named "config.mk" should be edited to fit the
requirements of your system. Once the configuration file is all set, installing
libstx is as simple as running the following command with root privileges:

`
make install
`

After this is run, the library can be linked to other C code when compiling with
the "-lstx" flag e.g.

`cc -o myfoo myfoo.c -lstx`

## Examples
TODO

## Implementation
libstx implements two basic data structures, the stx and the spx. The spx
structure is designed to contain mutable and resizable strings, while
the spx structure contains only a pointer of a string and it's length.

## Why use libstx
libstx is a fairly standard string implementation when compared to most other C
string libraries, however there are a few special properties it was created with
in mind that seperates it from the rest.

#### (0)
Memory management is completely manual. For example, before appending to a stx,
there must be enough memory allocated to the stx in the first place or else
undefined behavior will occur.

This is an advantage in the sense that
ultimate control of memory is given to the programmer, just as in traditional C,
and because of that memory doesn't need to be over-allocated or allocated at the
wrong time. Also, since reallocations can be more directly controlled,
the lifetimes of references to a stx can be more tightly controlled.

However this is a disadvantage as it makes buffer overruns easier to
accidently implement, and as usual manual memory management is just as manual as
it's always been.

#### (1)
Almost every function defined by libstx is composible with every other function
defined in the library. What this means is that most function's return value
can be used as a parameter in another function of the library. This feature is
true for all functions so long as they don't modify the stx memory buffer with
functions such as _malloc_() or _realloc_().

## Compatibility
libstx is compatible with the C99 standard and onwards. C11 _Generic macros are
provided for certain functions as well if C11 is being used.

## Contributing
Please follow the general style of the codebase when contributing to libstx.
There isn't a defintive style-guide written yet, however the general rules are:

1) tabs instead of spaces

2) line-length should not exceed 80 characters unless it is a string-literal.

3) Function names, function types, and function curly braces get their own line,
while keyword curly braces stay on the same line (K&R style).

Also, as a general rule of thumb, all tests should pass with every commit
(unless there is a good reason to break them, such as a test not working or a
major version change). To run all tests use the command

`make check`

There is also a handy git hook provided in the bin/ folder of the
project to make sure commit messages are formatted properly called "commit-msg".
To use it, just symlink the file to your .git/hooks/ folder in your local clone
of the repository.
