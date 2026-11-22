*This project has been created as part of the 42 curriculum by <login>.*
 
# ft_printf
 
## Description
 
**ft_printf** This project is a recreation of the standard C printf() function. It aims to understand how formatted output works in C by implementing a custom ft_printf() that handles different format specifiers, variadic arguments, and output conversion.

The project focuses on working with variadic functions, pointers, data types, and low-level output using the write() function.
 
The project produces a static library, `libftprintf.a`, exposing:
 
```c
int ft_printf(const char *format, ...);
```
 
`ft_printf` writes the formatted output to the standard output and returns the number of characters printed, or `-1` if an error occurs.
 
### Supported conversions
 
| Specifier | Description |
|-----------|-------------|
| `%c` | a single character |
| `%s` | a string (`(null)` is printed for a NULL pointer) |
| `%p` | a `void *` pointer in hexadecimal, prefixed with `0x` (`(nil)` for NULL, like glibc) |
| `%d` / `%i` | a signed decimal integer |
| `%u` | an unsigned decimal integer |
| `%x` | an unsigned integer in lowercase hexadecimal |
| `%X` | an unsigned integer in uppercase hexadecimal |
| `%%` | a percent sign |
 
As required by the subject, the buffer management of the original `printf()` is **not** implemented: every character is sent directly with `write`.
 
### Project structure
 
| File | Content |
|------|---------|
| `ft_printf.h` | prototypes shared by all files |
| `ft_printf.c` | `ft_printf` (entry point) and `parsing` (walks the format string) |
| `dispatch.c` | `dispatch` (chooses the right printing function for a specifier) |
| `print_chars.c` | `ft_putchar`, `ft_putstr` |
| `print_numbers.c` | `ft_putnbr`, `ft_putnbr_un` |
| `print_hex.c` | `ft_hex`, `hex_format` |
| `Makefile` | builds `libftprintf.a` |
 
## Instructions
 
### Compilation
 
```bash
make          # builds libftprintf.a at the root of the repository
make clean    # removes the object files
make fclean   # removes the object files and libftprintf.a
make re       # fclean + all
```
 
The library is built with `cc -Wall -Wextra -Werror` and archived with `ar rcs`. Running `make` twice does not relink anything.
 
### Usage
 
Create a `main.c` that includes the header:
 
```c
#include "ft_printf.h"
 
int	main(void)
{
	int	n;
 
	n = ft_printf("Hello %s, you are %d years old (%x)\n", "42", 7, 255);
	ft_printf("printed %d characters\n", n);
	return (0);
}
```
 
Compile it against the library:
 
```bash
make
cc -Wall -Wextra -Werror main.c -L. -lftprintf -I. -o test
./test
```
 
### Testing
 
The output and the return value of `ft_printf` were compared with the real `printf` on, among others: `0`, `-1`, `INT_MIN`, `INT_MAX`, `UINT_MAX`, `%x` of `-1`, `NULL` for `%s` and `%p`, `'\0'` for `%c`, and an empty format string. The two functions should not be mixed in the same test without `\n` or `fflush`, because the real `printf` is buffered and `ft_printf` is not.
 
## Algorithm and data structures
 
### Overall design: three layers
 
```
ft_printf  ->  parsing  ->  dispatch  ->  ft_putchar / ft_putstr / ft_putnbr / ft_hex ...
```
 
1. **`ft_printf`** only handles the variadic list: `va_start`, a call to `parsing`, `va_end`, and the return. Because `va_end` is called in one single place, every early exit from `parsing` is cleaned up correctly.
2. **`parsing`** walks the format string with an index. A normal character is printed as is. A `%` hands the following character to `dispatch`, and the index moves forward by 2 (the `%` and its specifier). A lone `%` at the end of the string is detected so the index never moves past the terminating `'\0'`.
3. **`dispatch`** is a chain of `if` statements, one per specifier. Each branch reads the right type from the argument list and calls the printing function. An unknown specifier returns `-1`.
This separation keeps every function under the 25 lines of the Norm and makes the project extensible: adding a conversion (for example `%b`) means adding one branch in `dispatch` and one printing function.
 
### Return value: counts travel up
 
Every printing function returns the number of characters it printed, or `-1` on a `write` failure. `dispatch` passes that value up, and `parsing` accumulates it in a single `total`. No global variable is needed. When a function makes several calls that can fail, it stores each result in a separate variable, tests it against `-1` first, and only then adds it to its count, so a failure cannot be hidden inside the count.
 
### The argument list (`va_list`)
 
`va_list` is only a cursor over the variadic arguments, and it can only move forward. It is created in `ft_printf` and passed **by pointer** (`va_list *`) to `parsing` and `dispatch`. This way every function moves the same cursor, and the code does not depend on how a given platform represents `va_list`. The type given to `va_arg` always matches the type the caller pushed: `int` for `%c`, `%d`, `%i`; `unsigned int` for `%u`, `%x`, `%X`; `char *` for `%s`; `void *` for `%p`. `%c` is read as an `int` because of the default argument promotions.
 
### Number printing: recursion
 
A number in base `b` is printed by printing `n / b` first (if `n >= b`) and then the last digit `n % b`. This produces the digits in the right order without any buffer. The recursion depth is at most about 20 levels for a 64-bit value, so there is no risk of stack problems.
 
- **`ft_putnbr_un`** prints an `unsigned int` in base 10 (`%u`).
- **`ft_putnbr`** handles the sign, then delegates to `ft_putnbr_un`. The value is stored in a `long` before being negated, which makes `INT_MIN` safe (`-(-2147483648)` fits in a `long`).
- **`ft_hex`** takes an `unsigned long` and a **base string** (`"0123456789abcdef"` or `"0123456789ABCDEF"`). A single function therefore serves `%x`, `%X` and `%p`. An `unsigned int` converts to `unsigned long` without changing its value, and the 64-bit type keeps the whole address for `%p`.
- **`hex_format`** adds what is specific to `%p`: the `0x` prefix and the `(nil)` output for a NULL pointer.
**Why recursion instead of a buffer?** It is shorter, needs no size computation, and keeps the code easy to read within the Norm. The cost is one `write` call per character, which is acceptable here because the subject forbids reproducing the buffering of `printf` and does not ask for performance. A fixed-size char array filled from the end and written with a single `write` would be a valid alternative with fewer system calls.
 
### Data structures and memory
 
No dynamic allocation is used, so there are no leaks to manage. The only data are local integers (index, running total, check values), the `va_list` cursor, and string literals used as digit tables.
 
### Edge cases
 
- `%s` with NULL prints `(null)`; `%p` with NULL prints `(nil)` (glibc behavior, to be adapted on macOS where `0x0` is printed).
- `%x` of a negative `int` prints the 32-bit two's complement value (`-1` gives `ffffffff`), because the argument is read as `unsigned int`.
- `%c` with `'\0'` prints a null byte and counts as one character.
- A format ending with a lone `%`, or an unknown specifier, is undefined behavior in the real `printf`. Here the function returns `-1` instead of reading past the end of the string.
## Resources
 
### References
 
- `man 3 printf`, `man 3 stdarg`, `man 2 write`
- ISO C standard, section on variable arguments (`<stdarg.h>`)
- System V AMD64 ABI, for how variadic arguments are passed on x86-64
- GNU Make manual, for pattern rules and dependencies
- `man ar`, for creating static libraries
- 42 Norm documentation
### Use of AI
 
AI was used as a learning and support tool during the development of this project. It helped me understand certain concepts, clarify how some functions work, and identify possible issues during debugging.

I also used it to discuss implementation approaches and improve my understanding of C programming concepts, especially variadic functions and memory management.

All the code was reviewed, tested, and adapted by me. I made sure to understand the logic behind the implementation rather than simply relying on generated solutions.
 