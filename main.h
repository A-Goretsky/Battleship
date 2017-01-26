#ifndef MAIN_H
#define MAIN_H

void printSettings(int board, int emoji);

char ** shipPlacement(int BOARD_SIZE);

char ** placeShip(char shipBoard[][14], int shipLength, int size);


#endif
