#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print(int board[9][9]) {
        for (int r = 0; r < 9; ++r) {
                for (int c = 0; c < 9; ++c) {
                        printf("%d ", board[r][c]);
                }
                putchar('\n');
        }
}

#define init(a) memset(a, 0, sizeof(a))

bool check(int board[9][9]) {
        int used[10];

        // rows
        for (int r = 0; r < 9; ++r) {
                init(used);
                for (int c = 0; c < 9; ++c) {
                        int n = board[r][c];
                        if (used[n]) return false;
                        if (n) used[n] = 1;
                }
        }

        // columns
        for (int c = 0; c < 9; ++c) {
                init(used);
                for (int r = 0; r < 9; ++r) {
                        int n = board[r][c];
                        if (used[n]) return false;
                        if (n) used[n] = 1;
                }
        }

        // grids
        for (int g = 0; g < 9; ++g) {
                int gr = g / 3, gc = g % 3;
                init(used);
                for (int i = 0; i < 9; ++i) {
                        int r = i / 3, c = i % 3;
                        int n = board[3 * gr + r][3 * gc + c];
                        if (used[n]) return false;
                        if (n) used[n] = 1;
                }
        }

        // cross
        init(used);
        for (int i = 0; i < 9; ++i) {
                int n = board[i][i];
                if (used[n]) return false;
                if (n) used[n] = 1;
        }
        init(used);
        for (int i = 0; i < 9; ++i) {
                int n = board[9 - i - 1][i];
                if (used[n]) return false;
                if (n) used[n] = 1;
        }

        return true;
}

bool solve(int board[9][9], int pos) {
        int row = pos / 9, col = pos % 9;

        if (pos == 9 * 9) return true;
        if (board[row][col]) return solve(board, pos + 1);

        // generate a random permutation of [1 .. 9]
        int shuffle[9];
        for (int i = 0; i < 9; ++i) shuffle[i] = i + 1;

        srand(time(NULL));
        for (int i = 0; i < 9; ++i) {
                int j = rand() % 9;
                int tmp = shuffle[i];
                shuffle[i] = shuffle[j];
                shuffle[j] = tmp;
        }

        for (int i = 0; i < 9; ++i) {
                board[row][col] = shuffle[i];
                if (check(board) && solve(board, pos + 1)) {
                        return true;
                }
        }

        board[row][col] = 0;

        return false;
}

bool read(int board[9][9], const char *fname) {
        FILE *fp = fopen(fname, "r");
        for (int r = 0; r < 9; ++r) {
                for (int c = 0; c < 9; ++c) {
                        if (fscanf(fp, "%d", &board[r][c]) != 1) return false;
                }
        }
        fclose(fp);
        return true;
}

int main(int argc, const char *argv[]) {
        int board[9][9];

        if (argc > 1) {
                bool sudoku_read = read(board, argv[1]);
                if (!sudoku_read) {
                        printf("cannot read a legal sudoku from %s\n", argv[1]);
                        return 1;
                } else {
                        printf("puzzle: \n");
                        print(board);
                        printf("solution: \n");
                }
        } else {
                init(board);
        }

        bool solved = solve(board, 0);

        print(board);

        if (!solved) {
                printf("cannot solve\n");
                return 1;
        }

        return 0;
}
