Tiny Shell in C

A very simple shell implementation in C demonstrating basic process management and I/O redirection.

- Execute commands with arguments(uses execvp() + fork())
- Background execution with &
- Input (<) and output (>) redirection
- Single pipe support (|) between two commands
- Handles Ctrl+C without exiting the shell
- Interactive prompt loop until exit command

<br>

### Example
```
mysh> ls -l
mysh> cat file.txt | grep hello
mysh> sort < unsorted.txt > sorted.txt
mysh> sleep 5 &
mysh> exit
Bye!
```

### Possible extensions towards a more functional shell

- Multiple pipes
- Built-in commands (e.g., cd, exit)
- Command history
- Better signal handling (prevent zombies)
- show command history
- configurable prompt

### Build & Run
```
gcc -o my_shell shell.c
./my_shell
```