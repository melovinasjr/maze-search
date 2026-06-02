#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_SIZE 10000
#define MAX_ROWS 102
#define MAX_COLS 102

typedef struct {
    short int vert;
    short int horiz;
} offsets;

typedef struct {
    short int row;
    short int col;
    short int dir;
} element;

/* 8 possible directions: N, NE, E, SE, S, SW, W, NW */
offsets move[8] = {
    {-1,  0}, {-1,  1}, {0, 1}, {1, 1},
    { 1,  0}, { 1, -1}, {0,-1}, {-1,-1}
};

element stack[MAX_STACK_SIZE];
int top = -1;

int maze[MAX_ROWS][MAX_COLS];
int mark[MAX_ROWS][MAX_COLS];

void push(element item) {
    if (top >= MAX_STACK_SIZE - 1) {
        printf("Error: stack overflow.\n");
        exit(1);
    }
    stack[++top] = item;
}

element pop(void) {
    if (top < 0) {
        printf("Error: stack underflow.\n");
        exit(1);
    }
    return stack[top--];
}

int main(void) {
    int m, p;
    int i, j;

    printf("Enter maze size (rows cols): ");
    if (scanf("%d %d", &m, &p) != 2) {
        printf("Invalid input for maze size.\n");
        return 1;
    }

    if (m <= 0 || p <= 0 || m + 2 > MAX_ROWS || p + 2 > MAX_COLS) {
        printf("Invalid maze size. Maximum usable size is %d x %d.\n", MAX_ROWS - 2, MAX_COLS - 2);
        return 1;
    }

    for (i = 0; i < m + 2; i++) {
        for (j = 0; j < p + 2; j++) {
            maze[i][j] = 1;
            mark[i][j] = 0;
        }
    }

    printf("Enter maze values row by row (0 = path, 1 = wall):\n");
    for (i = 1; i <= m; i++) {
        for (j = 1; j <= p; j++) {
            if (scanf("%d", &maze[i][j]) != 1) {
                printf("Invalid maze input.\n");
                return 1;
            }
            if (maze[i][j] != 0 && maze[i][j] != 1) {
                printf("Maze values must only be 0 or 1.\n");
                return 1;
            }
        }
    }

    if (maze[1][1] != 0) {
        printf("No path exists. The starting point (1,1) is blocked.\n");
        printf("\nPress Enter to exit...");
        getchar();
        getchar();
        return 0;
    }

    if (maze[m][p] != 0) {
        printf("No path exists. The exit point (%d,%d) is blocked.\n", m, p);
        printf("\nPress Enter to exit...");
        getchar();
        getchar();
        return 0;
    }

    int row, col, dir;
    int found = 0;
    int steps = 0;
    element pos;

    mark[1][1] = 1;
    pos.row = 1;
    pos.col = 1;
    pos.dir = 0;
    push(pos);

    while (top > -1 && !found) {
        pos = pop();
        row = pos.row;
        col = pos.col;
        dir = pos.dir;

        while (dir < 8 && !found) {
            int nextRow = row + move[dir].vert;
            int nextCol = col + move[dir].horiz;

            if (nextRow == m && nextCol == p && maze[nextRow][nextCol] == 0) {
                found = 1;
            } else if (maze[nextRow][nextCol] == 0 && mark[nextRow][nextCol] == 0) {
                mark[nextRow][nextCol] = 1;

                pos.row = row;
                pos.col = col;
                pos.dir = dir + 1;
                push(pos);

                row = nextRow;
                col = nextCol;
                dir = 0;
            } else {
                dir++;
            }
        }
    }

    if (found) {
        printf("\nPath found successfully.\n");
        printf("Path coordinates:\n");

        for (i = 0; i <= top; i++) {
            printf("Step %d: (%d, %d)\n", i + 1, stack[i].row, stack[i].col);
        }

        printf("Step %d: (%d, %d)\n", top + 2, row, col);
        printf("Step %d: (%d, %d)\n", top + 3, m, p);

        steps = top + 3;
        printf("Total steps in path: %d\n", steps);
    } else {
        printf("\nNo path exists in the maze.\n");
    }

    printf("\nPress Enter to exit...");
    getchar();
    getchar();

    return 0;
}                                                                                                                                                                               