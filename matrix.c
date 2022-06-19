#include <string.h>
#include <stdlib.h>
#include "matrix.h"

static matrix** matrices = NULL;
static int allocations = 0;

matrix* init(int rows, int cols) {
    if (!allocations) {
        atexit(free_matrices);
    }

    matrix* new = malloc(sizeof(matrix));

    if (!new) {
        printf("NULL pointer when creating the %i x %i matrix\n", rows, cols);
        return NULL;
    }

    new->rows = rows;
    new->cols = cols;

    new->m = malloc(sizeof(double*) * rows);

    if (!new->m) {
        printf("NULL pointer when adding rows to the %i x %i matrix\n", rows, cols);
        free_matrix(new);
        return NULL;
    }

    for (int row = 0; row < rows; row++) {
        new->m[row] = malloc(sizeof(double) * cols);

        if (!new->m[row]) {
            printf("NULL pointer when adding columns to the %i x %i matrix\n", rows, cols);
            free_matrix(new);
            return NULL;
        }
    }

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            new->m[row][col] = 0;
        }
    }

    matrix** temp = realloc(matrices, sizeof(matrix) * (allocations + 1));

    if (!temp) {
        free_matrix(new);
        return NULL;
    }

    matrices = temp;
    matrices[allocations] = new;
    allocations++;

    return new;
}

matrix* copy(matrix* m) {
    if (!m) {
        printf("NULL matrix provided\n");
        return NULL;
    }

    matrix* new = init(m->rows, m->cols);

    if (!new) {
        printf("NULL pointer when creating new matrix\n");
        return NULL;
    }

    set(new, m);

    return new;
}

matrix* add(matrix* a, matrix* b) {
    if (!a || !b) {
        printf("One or more given matrix was null\n");
        return NULL;
    }
    if (a->rows != b->rows || a->cols != b->cols) {
        printf("Dimensions for the given matrices do not match.\nCannot add the %i x %i to the %i x %i matrix\n",
               a->rows, a->cols, b->rows, b->cols);
        return NULL;
    }

    matrix* new = init(a->rows, a->cols);

    if (!new) {
        printf("NULL pointer when creating the sum matrix\n");
        return NULL;
    }

    for (int row = 0; row < a->rows; row++) {
        for (int col = 0; col < a->cols; col++) {
            new->m[row][col] = a->m[row][col] + b->m[row][col];
        }
    }

    return new;
}

matrix* multiply(matrix* a, matrix* b) {
    if (!a || !b) {
        printf("One or more given matrix was null\n");
        return NULL;
    }
    if (a->cols != b->rows) {
        printf("Given matrices cannot be multiplied to due a mismatch regarding their dimensions\n");
        return NULL;
    }

    matrix* new = init(a->rows, b->cols);

    if (!new) {
        printf("NULL pointer when creating the matrix product\n");
        return NULL;
    }

    for (int row = 0; row < a->rows; row++) {
        for (int col = 0; col < b->cols; col++) {
            double total = 0;

            for (int relative_pos = 0; relative_pos < a->cols; relative_pos++) {
                total += a->m[row][relative_pos] * b->m[relative_pos][col];
            }

            new->m[row][col] = total;
        }
    }

    return new;
}

matrix* multiply_scalar(matrix* m, double s) {
    if (!m) {
        printf("NULL matrix provided\n");
        return NULL;
    }

    matrix* new = init(m->rows, m->cols);

    if (!new) {
        printf("NULL pointer when creating the scaled matrix\n");
        return NULL;
    }

    for (int row = 0; row < m->rows; row++) {
        for (int col = 0; col < m->cols; col++) {
            m->m[row][col] *= s;
        }
    }

    return new;
}

void multiply_scalar_d(matrix* m, double s) {
    if (!m) {
        printf("NULL matrix provided\n");
        return;
    }

    for (int row = 0; row < m->rows; row++) {
        for (int col = 0; col < m->cols; col++) {
            m->m[row][col] *= s;
        }
    }
}

matrix* inverse(matrix* m) {
    if (!m) {
        printf("NULL matrix provided\n");
        return NULL;
    }
    if (m->cols != m->rows) {
        printf("Cannot find inverse of a non-square matrix\n");
        return NULL;
    }

    matrix* new = adj(m);

    if (!new) {
        printf("NULL pointer when creating inverse matrix\n");
        return NULL;
    }

    double determinant = det(m);

    if (!determinant) {
        printf("No inverse because the determinant is 0\n");
        return NULL;
    }

    multiply_scalar_d(new, 1 / determinant);

    return new;
}

matrix* adj(matrix* m) {
    if (!m) {
        printf("NULL matrix provided\n");
        return NULL;
    }

    matrix* co = cofactor(m);
    matrix* new = transpose(co);

    if (!new) {
        printf("NULL pointer when creating adjugate matrix\n");
        return NULL;
    }

    return new;
}

matrix* transpose(matrix* m) {
    if (!m) {
        printf("NULL matrix provided\n");
        return NULL;
    }

    matrix* new = init(m->cols, m->rows);

    if (!new) {
        printf("NULL pointer when copying given matrix\n");
        return NULL;
    }

    for (int row = 0; row < m->rows; row++) {
        for (int col = 0; col < m->cols; col++) {
            new->m[col][row] = m->m[row][col];
        }
    }

    return new;
}

matrix* cofactor(matrix* m) {
    if (!m) {
        printf("NULL matrix provided\n");
        return NULL;
    }
    if (m->rows != m->cols) {
        printf("Cannot find cofactor matrix of non-square matrix\n");
        return NULL;
    }

    matrix* new = init(m->rows, m->cols);

    if (!new) {
        printf("NULL pointer when creating cofactor matrix\n");
        return NULL;
    }

    for (int row = 0; row < m->rows; row++) {
        for (int col = 0; col < m->cols; col++) {
            matrix* minor = get(m, 0, m->rows, 0, m->cols, row, col);
            new->m[row][col] = det(minor);

            if ((row + col) & 1) {
                new->m[row][col] *= -1;
            }
        }
    }

    return new;
}

matrix* flatten(matrix* m) {
    if (!m) {
        printf("NULL matrix provided\n");
        return NULL;
    }

    matrix* new = init(1, m->rows * m->cols);

    if (!new) {
        printf("NULL pointer when creating flattened matrix\n");
        return NULL;
    }

    for (int row = 0; row < m->rows; row++) {
        for (int col = 0; col < m->cols; col++) {
            new->m[0][row * m->cols + col] = m->m[row][col];
        }
    }

    return new;
}

matrix* get(matrix* m, int rMin, int rMax, int cMin, int cMax, int ignoreRow, int ignoreColumn) {
    if (!m) {
        printf("NULL matrix provided\n");
        return NULL;
    }
    if (rMin >= rMax || cMin >= cMax) {
        printf("Invalid dimensions\n");
        return NULL;
    }
    if (rMin < 0 || cMin < 0 || rMax > m->rows || cMax > m->cols) {
        printf("Dimensions provided are out of bounds\n");
        return NULL;
    }

    short rowFlag = 0;
    short colFlag = 0;
    matrix* new = init(rMax - rMin - 1, cMax - cMin - 1);

    if (!new) {
        printf("NULL matrix when creating sub-matrix\n");
        return NULL;
    }

    for (int row = rMin; row < rMax; row++, colFlag = 0) {
        if (row != ignoreRow) {
            for (int col = cMin; col < cMax; col++) {
                if (col != ignoreColumn) {
                    new->m[row - rMin - rowFlag][col - cMin - colFlag] = m->m[row][col];
                } else {
                    colFlag = 1;
                }
            }
        }
        else {
            rowFlag = 1;
        }
    }

    return new;
}

matrix* get_from_cmd(void) {
    int rows = 0;
    int cols = 0;

    while ((rows = get_int("Enter rows for matrix:")) < 1) {
        printf("Number of rows for the matrix must be 1 or greater\n");
    }

    while ((cols = get_int("Enter cols for matrix:")) < 1) {
        printf("Number of columns for the matrix must be 1 or greater\n");
    }

    matrix* new = init(rows, cols);

    if (!new) {
        printf("NULL pointer when creating new matrix\n");
        return NULL;
    }

    printf("Please enter values going down each column in order\n");

    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            new->m[j][i] = get_double(">");
        }
    }

    return new;
}

double det(matrix* m) {
    if (!m) {
        printf("NULL matrix provided\n");
        return 0.0;
    }
    if (m->rows != m->cols) {
        printf("Cannot find determinant of non-square matrix\n");
        return 0.0;
    }
    if (m->rows == 2 && m->cols == 2) {
        double ad = m->m[0][0] * m->m[1][1];
        double bc = m->m[0][1] * m->m[1][0];

        return ad - bc;
    }

    double determinant = 0.0;

    for (int col = 0; col < m->cols; col++) {
        matrix* minor = get(m, 0, m->rows, 0, m->cols, 0, col);

        double temp = m->m[0][col] * det(minor);

        if (col & 1) {
            temp *= -1;
        }

        determinant += temp;
    }

    return determinant;
}

double get_double(char* msg) {
    double out = 0;
    char buffer[1024];
    const int buffer_size = sizeof(buffer);

    printf("%s", msg);

    while (fgets(buffer, buffer_size, stdin) != NULL) {
        size_t num_len = strlen(buffer);

        if (buffer[num_len - 1] == '\n') {
            buffer[num_len - 1] = '\0';
            num_len--;
        }

        if (sscanf_s(buffer, "%lf", &out, num_len) < 1) {
            printf("Error encountered when getting integer input\n");
        }
        else {
            break;
        }
    }

    return out;
}

int get_int(char* msg) {
    int out = 0;
    char buffer[1024];
    const int buffer_size = sizeof(buffer);

    printf("%s", msg);

    while (out < 1 && fgets(buffer, buffer_size, stdin) != NULL) {
        size_t num_len = strlen(buffer);

        if (buffer[num_len - 1] == '\n') {
            buffer[num_len - 1] = '\0';
            num_len--;
        }

        if (sscanf_s(buffer, "%i", &out, num_len) < 1) {
            printf("Error encountered when getting integer input\n");
        }
    }
    return out;
}

void set(matrix* a, matrix* b) {
    if (!a || !b) {
        printf("NULL matrix provided\n");
        return;
    }
    if (a->rows != b->rows || a->cols != b->cols) {
        printf("Dimensions do not match\n");
        return;
    }

    for (int row = 0; row < a->rows; row++) {
        for (int col = 0; col < a->cols; col++) {
            a->m[row][col] = b->m[row][col];
        }
    }
}

void set_matrix(matrix* m, double** n, int nRows, int nCols) {
    if (!m) {
        printf("NULL matrix provided\n");
        return;
    }
    if (!n) {
        printf("NULL 2D array provided\n");
        return;
    }
    if (m->cols != nCols || m->rows != nRows) {
        printf("Matrix and array sizes do not match\n");
        return;
    }

    for (int row = 0; row < nRows; row++) {
        for (int col = 0; col < nCols; col++) {
            m->m[row][col] = n[row][col];
        }
    }
}

void set_matrix_static(matrix* m, int nRows, int nCols, double n[nCols][nRows], int flipped) {
    if (!m) {
        printf("NULL matrix provided\n");
        return;
    }
    if (m->cols != nCols || m->rows != nRows) {
        printf("Matrix and array sizes do not match\n");
        return;
    }

    if (!flipped) {
        for (int row = 0; row < m->rows; row++) {
            for (int col = 0; col < m->cols; col++) {
                m->m[row][col] = n[row][col];
            }
        }
    }
    else {
        for (int row = 0; row < m->rows; row++) {
            for (int col = 0; col < m->cols; col++) {
                m->m[row][col] = n[col][row];
            }
        }
    }
}

void clear(matrix* m) {
    if (!m) {
        printf("NULL matrix provided\n");
        return;
    }

    for (int row = 0; row < m->rows; row++) {
        for (int col = 0; col < m->cols; col++) {
            m->m[row][col] = 0.0;
        }
    }
}

void display(matrix* m) {
    printf("[%i x %i]\n", m->rows, m->cols);
    for (int row = 0; row < m->rows; row++) {
        for (int col = 0; col < m->cols; col++) {
            printf("%lf ", m->m[row][col]);
        }
        printf("\n");
    }
}

void free_matrix(matrix* m) {
    if (m) {
        if (m->m) {
            for (int rows = 0; rows < m->rows; rows++) {
                if (m->m[rows]) {
                    free(m->m[rows]);
                }
            }
            free(m->m);
        }
        free(m);
    }
}

void free_matrices(void) {
    if (matrices) {
        for (int i = 0; i < allocations; i++) {
            if (matrices[i]) {
                free_matrix(matrices[i]);
            }
        }
        free(matrices);
    }
}
