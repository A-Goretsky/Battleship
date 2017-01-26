#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>

void printBoard_place(char board[][14], int size) {
    if (size == 8) {
        //printf("Size == 8");
        printf(" \\   ");
        int x = 1;
        for (x; x <= size; x++) {
            printf("%d   ", x);
        }
    }

    if (size >= 10) {
        printf(" \\   ");
        int x = 1;
        for (x; x < 9; x++) {
            printf("%d   ", x);
        }
        int y = 9;
        for (y; y <= size; y++) {
            printf("%d  ", y);
        }
    }

    printf("\n");

    if (size == 8) {
        printf("---+---+---+---+---+---+---+---+---+\n");
    }

    if (size == 10) {
        printf("---+---+---+---+---+---+---+---+---+---+---+\n");
    }

    if (size == 14) {
        printf("---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+\n");
    }

    int row = 0;
    int column = 0;
    row++;
    for(row; row <= size; row++) {
        printf("%d", row);
        fflush( stdout );
        if (row < 10) {
                printf("  ");
            }
        else {
                printf(" ");
            }

        for (column; column < size; column++) {
            printf("| ");
            fflush(stdout);
            printf("%c", board[row - 1][column]);
            fflush(stdout);
            printf(" ");
            fflush(stdout);
        }

        printf("|\n");
        fflush(stdout);
        column = 0;
    }

    if (size == 8) {
        printf("---+---+---+---+---+---+---+---+---+\n");
    }

    if (size == 10) {
        printf("---+---+---+---+---+---+---+---+---+---+---+\n");
    }

    if (size == 14) {
        printf("---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+\n");
    }
}

void printBoard_game(char shipBoard[][14], char guessBoard[][14], int size) {

    //printf("~~~~~~~~~~ Your Ship Board ~~~~~~~~~~");
    //printf("")

    /*~~~~~~~~~~ Your Ship Board ~~~~~~~~~~                                    ~~~~~~~~~~ Your Guess Board ~~~~~~~~~~
     //  1   2   3   4   5   6   7   8   9  10  11  12  13  14                //  1   2   3   4   5   6   7   8   9  10  11  12  13  14
    ---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+             ---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
    1  | o | o | o | o |   |   |   |   |   |   |   |   |   |   |             1  | o | o | o | o |   |   |   |   |   |   |   |   |   |   |
    2  |   |   |   |   |   |   |   |   |   |   |   |   |   |   |             2  |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
    3  |   |   |   |   |   |   | o |   |   |   |   |   |   |   |             3  |   |   |   |   |   |   | o |   |   |   |   |   |   |   |
    4  |   |   |   |   |   |   | o |   |   |   |   |   |   |   |             4  |   |   |   |   |   |   | o |   |   |   |   |   |   |   |
    5  |   |   |   |   |   |   | o |   |   |   |   |   |   |   |             5  |   |   |   |   |   |   | o |   |   |   |   |   |   |   |
    6  |   |   |   |   |   |   | o |   |   |   |   |   |   |   |             6  |   |   |   |   |   |   | o |   |   |   |   |   |   |   |
    7  |   |   |   |   |   |   | o |   |   |   |   |   |   |   |             7  |   |   |   |   |   |   | o |   |   |   |   |   |   |   |
    8  |   |   |   |   |   |   | o |   |   |   | o | o |   |   |             8  |   |   |   |   |   |   | o |   |   |   | o | o |   |   |
    9  |   |   |   |   |   |   | o |   |   |   |   |   |   |   |             9  |   |   |   |   |   |   | o |   |   |   |   |   |   |   |
    10 |   |   |   |   |   |   | o |   |   |   |   |   |   | o |             10 |   |   |   |   |   |   | o |   |   |   |   |   |   | o |
    11 |   |   |   |   |   |   | o |   |   |   |   |   |   | o |             11 |   |   |   |   |   |   | o |   |   |   |   |   |   | o |
    12 |   | o | o | o |   |   | o |   |   |   |   |   |   | o |             12 |   | o | o | o |   |   | o |   |   |   |   |   |   | o |
    13 |   |   |   |   |   |   | o |   |   |   |   |   |   | o |             13 |   |   |   |   |   |   | o |   |   |   |   |   |   | o |
    14 |   |   |   |   |   |   | o |   |   |   |   |   |   |   |             14 |   |   |   |   |   |   | o |   |   |   |   |   |   |   |
    ---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+             ---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+*/

    printf("~~~~~~~~~~ Your Ship Board ~~~~~~~~~~                                    ~~~~~~~~~~ Your Guess Board ~~~~~~~~~~");
    if (size == 8) {
        //printf("Size == 8");
        printf(" \\   ");
        int x = 1;
        for (x; x <= size; x++) {
            printf("%d   ", x);
        }

        printf("             ");

        //printf("Size == 8");
        printf(" \\   ");
        int x1 = 1;
        for (x1; x1 <= size; x1++) {
            printf("%d   ", x1);
        }
    }

    if (size >= 10) {
        printf(" \\   ");
        int x = 1;
        for (x; x < 9; x++) {
            printf("%d   ", x);
        }
        int y = 9;
        for (y; y <= size; y++) {
            printf("%d  ", y);
        }

        printf("             ");

        printf(" \\   ");
        int x1 = 1;
        for (x1; x1 < 9; x1++) {
            printf("%d   ", x1);
        }
        int y1 = 9;
        for (y1; y1 <= size; y1++) {
            printf("%d  ", y1);
        }
    }

    printf("\n");

    if (size == 8) {
        printf("---+---+---+---+---+---+---+---+---+\n");
        printf("             ");
        printf("---+---+---+---+---+---+---+---+---+\n");
    }

    if (size == 10) {
        printf("---+---+---+---+---+---+---+---+---+---+---+\n");
        printf("             ");
        printf("---+---+---+---+---+---+---+---+---+---+---+\n");
    }

    if (size == 14) {
        printf("---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+\n");
        printf("             ");
        printf("---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+\n");
    }

    //info lines
    int row = 0;
    int column = 0;
    int column2 = 0;
    column2++;
    row++;
    for(row; row <= size; row++) {
        printf("%d", row);
        fflush( stdout );
        if (row < 10) {
                printf("  ");
        }
        else {
                printf(" ");
        }

        for (column; column < size; column++) {
            printf("| ");
            fflush(stdout);
            printf("%c", shipBoard[row - 1][column]);
            fflush(stdout);
            printf(" ");
            fflush(stdout);
        }

        printf("|");
        fflush(stdout);
        column = 0;

        printf("             ");

        printf("%d", row);
        fflush( stdout );
        if (row < 10) {
                printf("  ");
        }
        else {
                printf(" ");
        }

        for (column2; column2 < size; column2++) {
            printf("| ");
            fflush(stdout);
            printf("%c", guessBoard[row - 1][column2]);
            fflush(stdout);
            printf(" ");
            fflush(stdout);
        }
        printf("|\n");
        fflush(stdout);
        column2 = 0;

    }

    if (size == 8) {
        printf("---+---+---+---+---+---+---+---+---+\n");
        printf("             ");
        printf("---+---+---+---+---+---+---+---+---+\n");
    }

    if (size == 10) {
        printf("---+---+---+---+---+---+---+---+---+---+---+\n");
        printf("             ");
        printf("---+---+---+---+---+---+---+---+---+---+---+\n");
    }

    if (size == 14) {
        printf("---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+\n");
        printf("             ");
        printf("---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+\n");
    }
}
