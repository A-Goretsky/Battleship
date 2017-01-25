#ifndef MAIN_H
#define MAIN_H

void printSettings(int board, int emoji);

char ** shipPlacement(int BOARD_SIZE);

char ** placeShip(char** shipBoard, int shipLength, int size);

void printBoard(char* board[], int size);

#endif
