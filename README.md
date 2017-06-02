# libstx - Dynamic String Handling Library

## Why use libstx
libstx is a fairly standard string implementation when compared to most other C
string libraries, however there are a few special properties it was created with
in mind that separates it from the rest.

#### #1. Memory and allocation
Memory management is completely manual. For example, before appending to a stx,
there must be enough memory allocated to the stx in the first place or else
undefined behavior will occur.

This is an advantage in the sense that
ultimate control of memory is given to the programmer, just as in traditional C,
and because of that memory doesn't need to be over-allocated or allocated at the
wrong time. Also, since reallocations can be more directly controlled,
the lifetimes of references to a stx can be more tightly controlled.

However this is a disadvantage as it makes buffer overruns easier to
accidentally implement, and as usual manual memory management is just as manual as
it's always been.

#### #2. Function composition
Almost every function defined by libstx can be composed with every other function
defined in the library. What this means is that most function's return value
can be used as a parameter in another function of the library. This feature is
true for all functions so long as they don't modify the stx memory buffer with
functions such as _malloc_() or _realloc_().

#### #3. Great utf8 support.
utf8 characters can be inserted into a string by using their utf32 encoding,
and a function for calculating the number of utf8 characters in a string is
provided. A standalone function called stxutf8f32 is also provided to convert a
to convert a utf32 encoding to a utf8 encoding in such a way that isn't
dependent on any of the data structures provided by libstx (i.e. converts a
uint32_t to a char *).

## How to Install

Before installing, the file named "config.mk" should be edited to fit the
requirements of your system. Once the configuration file is all set, installing
libstx is as simple as running the following command with root privileges:

`make install`

After this is run, the library can be linked to other C code when compiling with
the "-lstx" flag e.g.

`cc -o myfoomyfoo.c -lstx`

## Examples
#### #1. Printing hello world with extra steps
```C
#include <libstx.h>

int
main(int argc, char **argv)
{
	stx sp; // Doesn't have to be initialized if using stxalloc.
	stxalloc(&sp, 5);

	// Now initialize the contents of sp with "hello".
	stxlstrip(stxcpy_str(&sp, "    hello"), " ", 1);

	stxapp_str(&sp, " world");

	// This will print out "hello world" to stdout.
	printf("%.*s\n", sp.len, sp.mem);
}
```

#### #2. Finding a substring and slicing it
```C
#include <libstx.h>

int
main(int argc, char **argv)
{
	stx sp; // Doesn't have to be initialized if using stxdup.
	stxdup_str(&sp, "hello world");

	// Create a reference to the memory of "sp"
	spx slice = stxfind_str(stxref(&sp), "world");

	// This will print out "world" to stdout.
	printf("%.*s\n", slice.len, slice.mem);
}
```

#### #3. Getting the length of a string with utf8 characters.
```C
#include <libstx.h>

int
main(int argc, char **argv)
{
	// Initialize a immutable spx reference with multibyte characters.
	spx str = {10, "a£ก𐊀"}

	// This is an O(n) function just like the strlen() function.
	size_t n = stxutf8len(str);

	// This will print out the number '4' to stdout.
	printf("%zu\n", n);
}
```

## Implementation
libstx implements two basic data structures, the stx and the spx. The spx
structure is designed to contain mutable and resizable strings, while
the spx structure contains only a pointer to a string and it's length.

## Compatibility
libstx is compatible with the C99 standard and onwards. C11 _Generic macros are
provided for certain functions as well if C11 is being used.

## Contributing
Please follow the general style of the codebase when contributing to libstx.
There isn't a defintive style-guide written yet, however the general rules are:

1) Tabs instead of spaces

2) Line-length should not exceed 80 characters unless it is a string-literal.

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
