#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "networking.h"
#include "main.h"

void process( char * s );
void sub_server( int sd );
char inputSettings(char *question, char *type);

int main( int argc, char *argv[] ) {
  char b1[MESSAGE_BUFFER_SIZE];
  printf("Server (s) or Client (c)?\n");
  fgets( b1, sizeof(b1), stdin );
  char *p = strchr(b1, '\n');
  *p = 0;

  //Important variables

  int BOARD_SIZE = 0;
  int EMOJI_SET = 0;

  //Server Related Everything.

  if(!strcmp(b1, "s")){
    int sd, connection;
    printf("Successfully received server input...\n");
    sd = server_setup();

   //----------------------------------------------------SETUP PHASE: SERVER ONLY---------------------------------------------
    //Ask user to set rules.
    char b9[MESSAGE_BUFFER_SIZE];
    printf("What would you like the size of the board to be?\n\t1. Small (8x8)\n\t2. Medium (14x14)\n\t3. Large (20x20)\nEnter Value: ");
    fgets(b9, sizeof(b9), stdin);
    char *p = strchr(b9, '\n');
    *p = 0;
    if(!strcmp(b9, "1")){
        BOARD_SIZE = 1;
    }
    else if(!strcmp(b9,"2")){
        BOARD_SIZE = 2;
    }
    else if(!strcmp(b9,"3")){
        BOARD_SIZE = 3;
    }
    else{
        printf("Value not in range\n");
    }
    int netVal = htons(BOARD_SIZE);
    while (1) {
      printf("Attempting Connection...\n");
      connection = server_connect( sd );
      close(sd);
      char buffer[MESSAGE_BUFFER_SIZE];
      printf("printing BOARD_SIZE: %d\n", BOARD_SIZE);
      printf("printing netVal: %x\n", netVal);

      //Send Rules to Client.
      write(connection, &netVal, sizeof(netVal));

    //----------------------------------------------------PLACEMENT PHASE (SERVER)----------------------------------------------------

    //variables both server and client need for placement
    int size;

    //setting variables according to entered BOARD_SIZE
    if (BOARD_SIZE == 1) {
        size = 8;
    }
    if (BOARD_SIZE == 2) {
        size = 14;
    }
    if (BOARD_SIZE == 3) {
        size = 20;
    }

    //creating boards of appropriate size
    char guessBoard_S[size][size];
    char shipBoard_S[size][size];

    memcpy(shipBoard_S, shipPlacement(BOARD_SIZE), sizeof(char) * size * size);

    //----------------------------------------------------END OF PLACEMENT PHASE (SERVER)----------------------------------------------------

      //Main while loop
      while (read(connection, buffer, sizeof(buffer) )) {
        printf("[SERVER %d] received: %s\n", getpid(), buffer );
        write(connection, buffer, sizeof(buffer));
      }
      exit(0);
      close( connection );

    }
  }

  //Client Related Everything.

  else if(!strcmp(b1, "c")){

    //Client to server connection
    char host[MESSAGE_BUFFER_SIZE];
    printf("What IP would you like to connect to?\n");
    fgets( host, sizeof(host), stdin );
    printf("fgets here\n");
    char *cut = strchr(host, '\n');
    *cut = 0;

    int sd;

    sd = client_connect( host );


    //Receiving Rules from server
    int rule = 0;
    read(sd, &rule, sizeof(rule));
    char b2[MESSAGE_BUFFER_SIZE];
    printf("wassup\n");
    printf("printing received: %x\n", rule);
    printf("printing ntohs(rule): %d\n", ntohs(rule));
    rule = ntohs(rule);
    if(rule==1){
        printf("Board size is Small (7x7)\n");
    }
    else if(rule==2){
        printf("Board size is Medium (14x14)\n");
    }
    else if(rule==3){
        printf("Board size is Large (20x20)\n");
    }
    else{
        printf("Something went wrong during the connection phase.\n");
    }

    //----------------------------------------------------PLACEMENT PHASE (CLIENT)----------------------------------------------------

    //variables both server and client need for placement
    int size;

    //setting variables according to entered BOARD_SIZE
    if (BOARD_SIZE == 1) {
          size = 8;
    }
    if (BOARD_SIZE == 2) {
          size = 14;
    }
    if (BOARD_SIZE == 3) {
          size = 20;
    }

    //creating boards of appropriate size
    char guessBoard_C[size][size];
    char shipBoard_C[size][size];

    //Set board to blank spaces.

    memcpy(shipBoard_C, shipPlacement(BOARD_SIZE), sizeof(char) * size * size);
    //----------------------------------------------------END OF PLACEMENT PHASE (CLIENT)----------------------------------------------------

    //------------------------------Game------------------------------------
    while (1) {
      printf("enter message: ");
      fgets( b2, sizeof(b2), stdin );
      char *p = strchr(b2, '\n');
      *p = 0;

      write( sd, b2, sizeof(b2) );
      read( sd, b2, sizeof(b2) );
      printf( "received: %s\n", b2 );
    }
  }
  else{
    printf("invalid option");
  }
  return 0;
}

void process( char * s ) {

  while ( *s ) {
    *s = (*s - 'a' + 13) % 26 + 'a';
    s++;
  }
}

void printSettings(int board, int emoji) {
    char answer[100];
    strcpy(answer, "~~~Review Rules~~~\nBoard Size: ");
    if (board == 1) {
        strcat(answer, "Donald Trump's Hands\n");
    }
    if (board == 2) {
        strcat(answer, "Bourgeoisie\n");
    }
    if (board == 3) {
        strcat(answer, "Yuge\n");
    }
    strcat(answer, "Display Type: ");
    if (emoji == 1) {
        strcat(answer, "Emoji\n\n");
    }
    if (emoji == 2) {
        strcat(answer, "Standard ASCII\n\n");
    }
    printf("%s", answer);
}

char ** shipPlacement(int BOARD_SIZE) {

    //variables both server and client need for placement
    int size;
    int num2slotships;
    int num3slotships;
    int num4slotships;
    int num5slotships;

    //setting variables according to entered BOARD_SIZE
    if (BOARD_SIZE == 1) {
        size = 8;
        int num2slotships = 1;
        int num3slotships = 2;
        int num4slotships = 1;
        int num5slotships = 1;
    }
    if (BOARD_SIZE == 2) {
        size = 10;
        int num2slotships = 2;
        int num3slotships = 3;
        int num4slotships = 1;
        int num5slotships = 2;
    }
    if (BOARD_SIZE == 3) {
        size = 14;
        int num2slotships = 2;
        int num3slotships = 3;
        int num4slotships = 2;
        int num5slotships = 3;
    }

    //creating board to edit
    char shipBoard[size][size];

    while(num2slotships + num3slotships + num4slotships + num5slotships) {
        //printing board and number of remaining ships
        printBoard(shipBoard, size);
        printf("Number of 2 Slot Ships: %d\n", num2slotships);
        printf("Number of 3 Slot Ships: %d\n", num3slotships);
        printf("Number of 4 Slot Ships: %d\n", num4slotships);
        printf("Number of 5 Slot Ships: %d\n", num5slotships);

        //ask for a ship input
        printf("Which type of ship would you like to place down?\nEnter Value:");

        //loop until acceptable input entered
        int acceptableInput = 0;
        while(!acceptableInput) {
            char *shipInput[MESSAGE_BUFFER_SIZE];
            fgets(shipInput, sizeof(shipInput), stdin);

            //cut off trailing \n
            char *cut = strchr(shipInput, '\n');
            *cut = 0;

            //convert string input to int
            int shipSelect = 0;
            shipSelect = atoi(shipInput);

            //choose proper input for placement function based off of entered ship selection
            if (shipSelect == 1) {memcpy(shipBoard, placeShip(shipBoard, 2, size), sizeof(char) * size * size); acceptableInput = 1;}
            else if (shipSelect == 2)  {memcpy(shipBoard, placeShip(shipBoard, 3, size), sizeof(char) * size * size); acceptableInput = 1;}
            else if (shipSelect == 3)  {memcpy(shipBoard, placeShip(shipBoard, 4, size), sizeof(char) * size * size); acceptableInput = 1;}
            else if (shipSelect == 4)  {memcpy(shipBoard, placeShip(shipBoard, 5, size), sizeof(char) * size * size); acceptableInput = 1;}
            else {printf("That is not a valid input.\n");}
        }
    }
    return shipBoard;
}

void printBoard(char* board[], int size) {
/*" \\  1   2   3   4   5   6   7   8   9   10  11  12  13  14
    ---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
    1  | o | o | o | o |   |   |   |   |   |   |   |   |   |   |
    2  |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
    3  |   |   |   |   |   |   | o |   |   |   |   |   |   |   |
    4  |   |   |   |   |   |   | o |   |   |   |   |   |   |   |
    5  |   |   |   |   |   |   | o |   |   |   |   |   |   |   |
    6  |   |   |   |   |   |   | o |   |   |   |   |   |   |   |
    7  |   |   |   |   |   |   | o |   |   |   |   |   |   |   |
    8  |   |   |   |   |   |   | o |   |   |   | o | o |   |   |
    9  |   |   |   |   |   |   | o |   |   |   |   |   |   |   |
    10 |   |   |   |   |   |   | o |   |   |   |   |   |   | o |
    11 |   |   |   |   |   |   | o |   |   |   |   |   |   | o |
    12 |   | o | o | o |   |   | o |   |   |   |   |   |   | o |
    13 |   |   |   |   |   |   | o |   |   |   |   |   |   | o |
    14 |   |   |   |   |   |   | o |   |   |   |   |   |   |   |
    ---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+*/

    if (size == 8) {
        printf(" \\   ");
        int x = 1;
        for (x; x <= size; x++) {
            printf("%d   ", x);
        }
    }

    if (size = 14) {
        printf(" \\   ");
        int x = 1;
        for (x; x <= 10; x++) {
            printf("%d   ", x);
        }
        int y = 11;
        for (y; y <= 14; y++) {
            printf("%d  ");
        }
    }

    printf("\n");

    if (size == 8) {
        printf("---+---+---+---+---+---+---+---+---+\n");
    }

    if (size == 14) {
        printf("---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+\n");
    }

    int row = 0;
    for(row; row < size; row++) {

        printf("%c", row);
        if (atoi(board[row]) < 10) {
                printf("  ");
            }
            else {
                printf(" ");
            }

        int column = 0;
        for (column; column < size; column++) {
            printf("| ");
            printf("%c", board[row][column]);
            printf(" ");
        }

        printf("|\n");
    }

    if (size == 8) {
        printf("---+---+---+---+---+---+---+---+---+\n");
    }

    if (size == 14) {
        printf("---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+\n");
    }
}

char ** placeShip(char** shipBoard, int shipLength, int size) {

    printf("~~~~~How Placement Works~~~~~\n");
    printf("- You will be prompted to enter one coordinate (Row and Column) and the direction of placement.\n");
    printf("- That coordinate will act as the starting point, with the rest of the ship extending to the\ndirection of choice. Ships will appear on the board as they are placed.\n");

    int row = 0;
    int column = 0;
    int direction = 0;
    int valid = 0;
    while(!valid) {
        valid = 1;
        //Print Board.
        //BOARD PRINTING METHOD HERE. Clear terminal and print board at top with a few new lines.
        execlp("clear", "clear");
        printBoard(shipBoard, size);

        //-------------Asking for first coordinate-------------
        printf("Where would you like to place the first point of your ship?\n");
        //ask for row
        //loop to make sure row input is in the array
        char *buffer[MESSAGE_BUFFER_SIZE];
        int rowcheck = 0;
        while(!rowcheck) {
            row = 0;
            printf("Enter the Row: ");
            fgets(buffer, sizeof(buffer), stdin);
            row = atoi(buffer);

            //if column is within range, pass column check
            if((row > 0) && (row <= size)) {
                rowcheck = 1;
            }

            //else print error and ask for another input
            else {
                printf("That is not a valid row number.\n");
            }
        }

        //ask for column
        //loop to make sure column input is in the array
        int columncheck = 0;
        while(!columncheck) {
            column = 0;
            printf("Enter the Column: ");
            fgets(buffer, sizeof(buffer), stdin);
            column = atoi(buffer);

            //if column is within range, pass column check
            if((column > 0) && (column <= size)) {
                columncheck = 1;
            }

            //else print error and ask for another input
            else {
                printf("That is not a valid column number.\n");
            }
        }

        //ask for direction
        int directioncheck = 0;
        while(!directioncheck) {
            direction = 0;
            printf("Where would you like your ship to point?\n\t1. North\n\t2. South\n\t3. West\n\t4. East\nEnter Value: ");
            fgets(buffer, sizeof(buffer), stdin);
            direction = atoi(buffer);
            //if direction is within range, pass direction check
            if((direction > 0) && (direction <= 4)) {
                directioncheck = 1;
            }
            else {
                printf("That is not a valid choice of direction.\n");
            }
        }

        //check for ship/array conflicts
        //if direction = north, check column -> column - length, check for array conflicts, check for ship conflicts.
        if (direction = 1) {
            int columnpointer = column - 1;
            for(columnpointer; columnpointer < (column + shipLength - 1); columnpointer++) {
                if ((columnpointer < 0) || (columnpointer >= size)) {
                    valid = 0;
                }
                else if (!(shipBoard[row][columnpointer] == '0')) {
                    valid = 0;
                }
            }
        }
        //if direction = south, check column -> column - length, check for end of array, check for ship conflicts.
        if (direction = 2) {
            int columnpointer = column - 1;
            for(columnpointer; columnpointer > (column - shipLength - 1); columnpointer--) {
                if ((columnpointer < 0) || (columnpointer >= size)) {
                    valid = 0;
                }
                else if (!(shipBoard[row][columnpointer] == '0')) {
                    valid = 0;
                }
            }
        }
        //if direction = east, check row -> row + length, check for end of array, check for ship conflicts.
        if (direction = 3) {
            int rowpointer = row - 1;
            for(rowpointer; rowpointer < (row + shipLength - 1); rowpointer++) {
                if ((rowpointer < 0) || (rowpointer >= size)) {
                    valid = 0;
                }
                else if (!(shipBoard[rowpointer][column] == '0')) {
                    valid = 0;
                }
            }
        }
        //if direction = west, check row -> row - length, check for end of array, check for ship conflicts.
        if (direction = 4) {
            int rowpointer = row - 1;
            for(rowpointer; rowpointer > (row - shipLength - 1); rowpointer--) {
                if ((rowpointer < 0) || (rowpointer >= size)) {
                    valid = 0;
                }
                else if (!(shipBoard[rowpointer][column] == '0')) {
                    valid = 0;
                }
            }
        }

        //after the row, column, and direction have been chosen and all inputs are valid
        //if the ship either goes off the board
        //or the ship crosses over another ship
        //the ship placement is not valid and therefore the loop restarts
        if (!valid) {
            printf("That is not a valid ship placement.\n");
        }
    }

    //place ships
    if (direction = 1) {
        int columnpointer = column - 1;
        for(columnpointer; columnpointer < (column + shipLength - 1); columnpointer++) {
            shipBoard[row][columnpointer] = 'o';
        }
    }
    if (direction = 2) {
        int columnpointer = column - 1;
        for(columnpointer; columnpointer > (column - shipLength - 1); columnpointer--) {
            shipBoard[row][columnpointer] = 'o';
        }
    }
    if (direction = 3) {
        int rowpointer = row - 1;
        for(rowpointer; rowpointer > (row - shipLength - 1); rowpointer++) {
            shipBoard[rowpointer][column] = 'o';
        }
    }
    if (direction = 4) {
        int rowpointer = row - 1;
        for(rowpointer; rowpointer > (row - shipLength - 1); rowpointer--) {
            shipBoard[rowpointer][column] = 'o';
        }
    }

    //return editted board
    return shipBoard;
}
