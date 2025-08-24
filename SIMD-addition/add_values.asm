; File: add_values.asm
section .text
    global add_values

; void add_values(const uint8_t *src, const uint8_t *src2) - c function prototype
add_values:
    movdqu xmm0, [rdi]       ; rdi = src
    movdqu xmm1, [rsi]       ; rsi = src2
    paddb  xmm0, xmm1
    movdqu [rdi], xmm0
    ret
