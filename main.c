#include "matrix.h"

int main() {
    double al[5][5] = {{4, 12, 4, 8, 7}, {6, 0, 6, 3, 1}, {1, 3, 2, 1, 3}, {3, 5, 4, 4, 5}, {4, 2, 9, 5, 0}};
    double o[4][4] = {{5, 1, 3, 7}, {2, 6, 4, 2}, {-2, -8, 7, 6}, {1, 0, 2, 5}};
    double f[3][3] = {{1, 3, 5}, {7, 6, 2}, {1, 4, 4}};
    double f2[2][3] = {{2, 4, 7}, {3, 6, 1}};
    matrix* al2 = init(5, 5);
    matrix* o2 = init(4, 4);
    matrix* a = init(3, 3);
    matrix* b = init(3, 2);

    if (!a) {
        printf("NULL pointer\n");
        return 1;
    }
    if (!b) {
        printf("NULL pointer\n");
        return 1;
    }

    set_matrix_static(al2, 5, 5, al, 1);
    set_matrix_static(o2, 4, 4, o, 1);
    set_matrix_static(a, 3, 3, f, 0);
    set_matrix_static(b, 3, 2, f2, 1);

    display(a);
    printf("\n");
    display(b);

    matrix* c = get_from_cmd();

    if (c) {
        display(c);
    }

    display(inverse(al2));
    display(flatten(b));

    return 0;
}
