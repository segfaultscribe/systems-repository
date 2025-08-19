# HEXDITCH : A hexdump implementation from scratch 

A basic implementation of hexdump.

A hexdump in C is a program or function that displays the contents of a file (or memory) in `hexadecimal` and `ASCII` format.

output froma binary file might look like this
```
00000000  48 65 6c 6c 6f 20 77 6f  72 6c 64 21     |Hello world!|
```

Supports `ASCII` representation and also `colored` output.

usage
```
gcc hexditch.c -o hd
./hd -h or ./hd --help  

# further instructions present in help 😉
```
