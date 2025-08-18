# Implementing a Limited `printf` in C

This project implements a subset version of `printf` called `sysout`, which supports only basic format specifiers characters (`%c`), strings (`%s`), and integers (`%d`).


### Components

- `print_char`: Outputs a single character using `write`.
- `print_str`: Outputs a string one character at a time.
- `print_int`: Outputs an integer, digit by digit.
- `sysout`: A variadic function that mimics `printf` behavior using a custom format parser.


### Why?

- Use of **variadic arguments** with `va_list`, `va_start`, `va_arg`, and `va_end`.
- Manual **format string parsing**.
- Writing **type-specific print functions** using only low-level I/O (`write`).
- Basic **runtime dispatch** based on type.
- pointers (if that's what you're looking for)


### Extendability

This minimal version can be extended to handle more format specifiers (like `%f`, `%x`, `%u`), padding, width, precision, or even output to files using `my_fprintf`.

It's a good way to solidify C fundamentals and explore C.

### Run code

```
gcc sysout.c -o so
./so
```
