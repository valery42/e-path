#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int* elem;
    int size;
    int top;
} Stack;

void init_stack(Stack* s, int sz) {
    s->elem = malloc(sz * sizeof(int));
    s->size = sz;
    s->top = -1;
}

void delete_stack(Stack* s) {
    free(s->elem);
}

int empty(const Stack* s) {
    return s->top == -1;
}

int full(const Stack* s) {
    return s->top == s->size - 1;
}

void push(Stack* s, int x) {
    if (full(s)) {
        s->size *= 2;
        s->elem = realloc(s->elem, s->size * sizeof(int));
    }
    s->top += 1;
    s->elem[s->top] = x;
}

int pop(Stack* s) {
    if (empty(s)) {
        fprintf(stderr, "pop() failed: Stack underflow\n");
        exit(2);
    } else {
        s->top -= 1;
        return s->elem[s->top + 1];
    }
}

int at_top(const Stack* s) {
    return s->elem[s->top];
}

int** create_matrix(int size) {
    int** matrix = malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        matrix[i] = malloc(size * sizeof(int));
    }
    return matrix;
}

void delete_matrix(int** matrix, int size) {
    for (int i = 0; i < size; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void read_matrix(FILE* f, int** matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fscanf(f, "%d", &matrix[i][j]);
        }
    }
}

int degree(int vertex, int** matrix, int size) {
    int deg = 0;
    for (int j = 0; j < size; j++) {
        if (matrix[vertex][j] > 0) {
            deg += matrix[vertex][j];
        }
    }
    return deg;
}

int eulerian_path_exists(int** matrix, int size) {
    int odd_vertices = 0;
    int odd_vertex_idx = -1;
    for (int i = 0; i < size; i++) {
        int deg = degree(i, matrix, size);
        if (deg % 2 == 1) {
            odd_vertices++;
            if (odd_vertex_idx == -1) {
                odd_vertex_idx = i + 1;
            }
        }
        if (odd_vertices > 2) {
            // Eulerian path doesn't exist
            return -1;
        }
    }
    if (odd_vertex_idx == -1) {
        // Eulerian cycle exists
        return 0;
    } else {
        // Eulerian path exists
        return odd_vertex_idx;
    }
}

int find_adjacent_vertex(int vertex, int** matrix, int size) {
    for (int j = 0; j < size; j++) {
        if (matrix[vertex][j] > 0) {
            return j;
        }
    }
}

void eulerian_path(int** matrix, int size) {
    Stack s;
    init_stack(&s, size);
    int rv = eulerian_path_exists(matrix, size);
    if (rv == -1) {
        puts("Eulerian path doesn't exist");
    } else {
        if (rv > 0) {
            printf("Eulerian path: ");
            rv--;
        } else {
            printf("Eulearian cycle: ");
        }
        int vertex = rv;
        push(&s, vertex);
        while (!empty(&s)) {
            vertex = at_top(&s);
            if (degree(vertex, matrix, size) == 0) {
                printf("%d", pop(&s) + 1);
                if (!empty(&s)) {
                    printf("->");
                }
            } else {
                int adj_vertex = find_adjacent_vertex(vertex, matrix, size);
                matrix[vertex][adj_vertex] -= 1;
                if (vertex != adj_vertex) {
                    matrix[adj_vertex][vertex] -= 1;
                }
                push(&s, adj_vertex);
            }
        }
        puts("");
    }
    delete_stack(&s);
}

int main() {
    FILE* f = fopen("graphs/graph5.txt", "r");
    if (f == NULL) {
        perror("fopen() failed");
        exit(1);
    } else {
        int size;
        fscanf(f, "%d", &size);
        int** matrix = create_matrix(size);
        read_matrix(f, matrix, size);
        fclose(f);
        eulerian_path(matrix, size);
        delete_matrix(matrix, size);
    }
}
