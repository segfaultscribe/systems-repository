# File: Makefile

# Compiler and tools
NASM = nasm
CC = gcc

# Flags
NASMFLAGS = -f elf64
CFLAGS = -O2 -msse2

# Files
ASM_SRC = add_values.asm
ASM_OBJ = add_values.o
C_SRC = add.c
BIN = test

all: $(BIN)

$(ASM_OBJ): $(ASM_SRC)
	$(NASM) $(NASMFLAGS) -o $@ $<

$(BIN): $(C_SRC) $(ASM_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

run: $(BIN)
	./$(BIN)

clean:
	rm -f $(BIN) $(ASM_OBJ)

.PHONY: all run clean
