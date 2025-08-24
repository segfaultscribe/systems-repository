# Optimizing C using SIMD assembly

More like a how-to, this project helps you learn how to write `SIMD assembly` and connect it to C to improve performance. In this experiment, we use SIMD assembly which is the first example provided in [ffmppeg's first assembly lesson](https://github.com/FFmpeg/asm-lessons/blob/main/lesson_01/index.md). The x86inc.asm macros have been removed so that you can build and run the code without needing to build the ffmpeg environment.

<br>

You can compare the example from the lesson with this code to see how `raw NASM SIMD` changes when using `x86inc.asm` macros.

✌🏼
<hr>

### Build instructions

```
make        # Builds everything
make run    # Runs the binary
make clean  # Cleans up

# can also do

make
./test
```