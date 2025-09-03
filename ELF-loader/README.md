# Toy ELF Loader

A minimal ELF loader written in C for educational purposes.
This project parses and simulates loading ELF binaries (64-bit, little endian, x86-64).

⚠️ **Note:** This is not a production loader, it won’t execute binaries. Instead, it demonstrates the core concepts of how loaders work by validating ELF files, parsing headers, loading segments into simulated memory, and mapping virtual addresses to host memory.

* Validate ELF magic bytes and class (32/64-bit, endianness).
* Parse ELF header and program headers.
* Identify and handle `PT_LOAD` segments.
* Allocate memory for segments and load file contents.
* Zero-initialize `.bss` (where `p_memsz > p_filesz`).
* Maintain a mapping of ELF virtual addresses → host memory.
* Translate arbitrary virtual addresses to host pointers.
* Display a clean report of loaded segments and the entry point.

## 📜 Example Output

Here’s what running the loader on a small shared object looks like:

```
Valid ELF File acquired!
CLASS: 64 bit
ENDIANNESS: Little Endian
Type: Shared object (so/dylib)
Machine: x86_64

Program entry point: 0x1200
Program header offset: 64
Section header offset: 18768

[0] PHDR        Offset: 0x64    VirtAddr: 64    FileSize: 728   MemSize: 728    Flags: R--
[1] INTERP      Offset: 0x792   VirtAddr: 792   FileSize: 28    MemSize: 28     Flags: R--
[2] LOAD        Offset: 0x0     VirtAddr: 0     FileSize: 2392  MemSize: 2392   Flags: R--
[3] LOAD        Offset: 0x4096  VirtAddr: 4096  FileSize: 5021  MemSize: 5021   Flags: R-X
[4] LOAD        Offset: 0x12288 VirtAddr: 12288 FileSize: 1732  MemSize: 1732   Flags: R--
[5] LOAD        Offset: 0x15696 VirtAddr: 19792 FileSize: 704   MemSize: 736    Flags: RW-
[6] DYNAMIC     Offset: 0x15712 VirtAddr: 19808 FileSize: 496   MemSize: 496    Flags: RW-
[7] NOTE        Offset: 0x824   VirtAddr: 824   FileSize: 48    MemSize: 48     Flags: R--
[8] NOTE        Offset: 0x872   VirtAddr: 872   FileSize: 68    MemSize: 68     Flags: R--
[9] GNU_PROPERTY        Offset: 0x824   VirtAddr: 824   FileSize: 48    MemSize: 48     Flags: R--
[10] GNU_EH_FRAME       Offset: 0x13668 VirtAddr: 13668 FileSize: 76    MemSize: 76     Flags: R--
[11] GNU_STACK  Offset: 0x0     VirtAddr: 0     FileSize: 0     MemSize: 0      Flags: RW-
[12] GNU_RELRO  Offset: 0x15696 VirtAddr: 19792 FileSize: 688   MemSize: 688    Flags: R--

---LOAD SEGMENTS---
LOAD SEGMENT:
  VirtAddr : 0x0000000000000000
  FileOffset: 0x0000000000000000
  FileSize : 2392
  MemSize  : 2392
  Flags    : R--
  -> Allocated 2392 bytes at host address 0x559422115990

LOAD SEGMENT:
  VirtAddr : 0x0000000000001000
  FileOffset: 0x0000000000001000
  FileSize : 5021
  MemSize  : 5021
  Flags    : R-X
  -> Allocated 5021 bytes at host address 0x5594221162f0

LOAD SEGMENT:
  VirtAddr : 0x0000000000003000
  FileOffset: 0x0000000000003000
  FileSize : 1732
  MemSize  : 1732
  Flags    : R--
  -> Allocated 1732 bytes at host address 0x5594221176a0

LOAD SEGMENT:
  VirtAddr : 0x0000000000004d50
  FileOffset: 0x0000000000003d50
  FileSize : 704
  MemSize  : 736
  Flags    : RW-
  (extra 32 bytes should be zero-initialized)
  -> Allocated 736 bytes at host address 0x559422117d70

Entry point virtual: 0x4608 maps to host: 0x5594221164f0

Test1 (start)   vaddr=0x0 -> host=0x559422115990
Test2 (+16)     vaddr=0x16 -> host=0x5594221159a0
Test3 (last)    vaddr=0x2391 -> host=0x5594221162e7
Test4 (outside) vaddr=0x2392 -> host=(nil)

Loaded 4 segments. Adios now!
Loader finished successfully
```

## How It Works

1. **Validation**:

   * Checks the ELF magic bytes (`0x7F 'E' 'L' 'F'`).
   * Confirms 64-bit and little endian.

2. **Parse headers**:

   * Reads the ELF header.
   * Iterates over program headers.

3. **Load segments**:

   * For each `PT_LOAD`, allocates memory with `malloc()`.
   * Reads the file contents into memory.
   * Zero-fills any padding (`.bss`).

4. **Map virtual addresses**:

   * Maintains a table of loaded segments.
   * Provides a helper `vaddr_to_host()` to translate virtual addresses to host pointers.

5. **Entry point**:

   * Locates the program’s entry point (`e_entry`).
   * Translates it into a host address (but does not execute).

## 🛠️ Build & Run

```bash
gcc -Wall -Wextra -o toy_loader elfload.c
./toy_loader your_binary.so

# or after compiling use the loader itself as input

./toy_loader toy_loader
```

## Significance

Operating systems use loaders to bring executables into memory and prepare them for execution.
This toy loader strips the process down to its fundamentals so you can see:

* How ELF files are structured.
* How program segments are mapped.
* How virtual addresses relate to real memory.

It’s a great stepping stone toward systems programming, OS development, or just understanding what happens when you run `./a.out`.

## 📚 References

* [System V Application Binary Interface](https://refspecs.linuxfoundation.org/elf/elf.pdf) //pretty huge deal!
* `man elf`
* Brendan Gregg’s ELF resources


### Next steps could include:

* Using `mmap()` instead of `malloc()` for more realistic mapping.
* Handling relocations (`PT_DYNAMIC`).
* Actually transferring control to the loaded binary (requires careful sandboxing).
