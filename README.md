# Systems Repository: A playground for the curious

A playground for systems programming. Dive into OS internals, C, assembly, memory allocators, and more through hands-on projects.
Grows at a rate of one project or upgrade-to-existing-project per day, but as always is subject to the irregularities of human existence. 

- [dynamic-array](./dynamic-array/) : Implementing dynamically growing arrays in C. No limit required in declaration unlike normal C arrays.
- [stack-growth](./stack-growth/) : Figuring out which direction the stack in memory grows(Up or Down).
- [File handling refresher](./FileHandling/) : A straightforward refresher to file handling in C.
- [custom printf()](./custom-printf/) : A custom printf() function from scratch using variadic arguments and write().
- [hexdump from scratch](./hexdump-II/) : `hexditch` a hexdump implementation in C with basic features. Supports ascii and colors.
- [slumber](./slumber/) : A small program that wraps around nanosleep to create a seacond based sleep() function called `slumber`.
- [bump-allocator](./baby_malloc/) :  `baby_malloc()`, a simulation(?) of how memory management works under the hood. Good project to get started with memory. 
- [steal](./steal/) : A custom `cp` function. Handles batch copying and is being maintained to handle multithreading.
- [SIMD-addition](./SIMD-addition/) : learn how to write `SIMD assembly`(add two 16 byte arrays) and connect it to C to improve performance.
- [signal-logger](./signal-logger/) : log signals sent to the terminal, learn about signals in C
- [json-parse](./json-parser/) : JSON parser written in C, designed to help understand how JSON parsing works under the hood using a `deterministic finite automaton (DFA)` style `state machine` approach. Learn state machines in C!
- [TCP-chat-server](./TCP-chat-server/) : multithreaded TCP chat server written in C using POSIX sockets and pthread.
- [ELF-loader](./ELF-loader/) : A minimal `Executable and linking format (ELF) loader` to peek under the hood of how the OS loads programs (is a wee bit heavy project in comparison).
- [Tiny-shell](./toy-shell/) : A tiny shell in C. Barebones implementation of a shell. uses fork(), execvp(), handles '<', '>', '|' etc.