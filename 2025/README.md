# 2025
This year is different than previous years. Only 12 days. 
As a result, I've decided to improve my C coding skills. 
Everything that is not part of the standard library will be handwritten by me.
I will compile using the compiler flags
```bash
gcc -std=c2x \
    -Wall -Wextra -Wpedantic \
    -Wshadow -Wformat=2 -Wimplicit-fallthrough -Wmissing-prototypes \
    -fsanitize=address,undefined \
    -O0 -g3 -fno-omit-frame-pointer \
    -o dayxx \
    dayxx.c
```
