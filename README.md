# Matrix
A simple implementation of a mathematical matrix with C.

### Usage

Simply add `#include "path/to/matrix.h` to preprocessor directives as needed. The include in `matrix.c` should also be adjusted according to its location relative to `matrix.h`.

### Memory

Memory leaks are handled by `init()` and `free_matrices()`. There is no need to free any matrix struct after initialization.
