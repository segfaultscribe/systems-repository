# C program to check the direction in which the stack in memory drows

A program's address space consists of three main segments, code, stack and heap. Usually the variables and functions we declare in a program are allocated memory on the stack when executing. Dynamically allocated memory however resides in the heap. In this simple code, we're trying to come up with a program to check which direction the stack grows in memory. The stack could grow up or down and in most modern systems it grows down.

A very interesting consideration here is the fact that we're using C. In the C programming standards, a stack is never mentioned and stack is not a necessary structure for C programs. However this program works because most modern C compilers use the stack anyway. I believe this program can be written in many ways but here we explore on of the most simple ways without recusion.

You might also note that C allows pointer comparison only if both pointers:

- Are part of the same array/object, or
- Point one-past-the-end of the same array, or
- Are both null

In our program we use two entirely separate variables not linked to each other through the same object or array , hence we make use of the `intptr_t` cast to avoid undefined behaviour and remain safe.

#### To run
```
gcc stack_growth.c -o sg
./sg
```


