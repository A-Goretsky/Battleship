#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "networking.h"

void process( char * s );
void sub_server( int sd );
char inputSettings(char *question, char *type);

int main( int argc, char *argv[] ) {
  char b1[MESSAGE_BUFFER_SIZE];
  printf("Server (s) or Client (c)?\n");
  fgets( b1, sizeof(b1), stdin );
  char *p = strchr(b1, '\n');
  *p = 0;
  int BOARD_SIZE = 0;
  int EMOJI_SET = 0;
  if(!strcmp(b1, "s")){
    int sd, connection;
    printf("Successfully received server input...\n");
    sd = server_setup();

    //----------------------------------------------------SETUP PHASE: SERVER ONLY----------------------------------------------------
    /*int CONFIRMATION = 0;
    while(!CONFIRMATION) {
          BOARD_SIZE = inputSettings("What would you like the size of the board to be?\n\t1. Small (7x7)\n\t2. Medium (14x14)\n\t3. Large (20x20)\nEnter Value: ", "board");
		  EMOJI_SET = inputSettings("Would you like Emoji or standard ASCII graphics?\n\t1. Emoji\n\t2. Standard\nEnter Value: ", "emoji");
		  printf("printing within while: %d", BOARD_SIZE);
		  char settings[50];
		  strcpy(settings,  "Are you okay with these settings?\n");
		  printSettings(BOARD_SIZE, EMOJI_SET);
		  strcat(settings, "\t1. Yes, these settings are correct.\n\t2. No, I would like to start over.\nEnter Value: ");
		  CONFIRMATION = inputSettings(settings, "confirm");
    }*/

    //----------------------------------------------------CONNECTION PHASE----------------------------------------------------
    //new attempt at settings





    char b9[MESSAGE_BUFFER_SIZE];
    printf("What would you like the size of the board to be?\n\t1. Small (7x7)\n\t2. Medium (14x14)\n\t3. Large (20x20)\nEnter Value: ");
    fgets(b9, sizeof(b9), stdin);
    char *p = strchr(b9, '\n');
    *p = 0;
    if(!strcmp(b9, "1")){
        BOARD_SIZE = 1;
    }else if(!strcmp(b9,"2")){
        BOARD_SIZE = 2;
    }else if(!strcmp(b9,"3")){
        BOARD_SIZE = 3;
    }else{
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

      //Main while loop
      while (read(connection, buffer, sizeof(buffer) )) {
        printf("[SERVER %d] received: %s\n", getpid(), buffer );
        write(connection, buffer, sizeof(buffer));
      }
      exit(0);
      close( connection );

    }
  }

  else if(!strcmp(b1, "c")){
    char *host[MESSAGE_BUFFER_SIZE];
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

    //


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

/*
        //----------------------------------------------------PLACEMENT PHASE----------------------------------------------------
        //variables both server and client need for placement
        int size;
        int num2slotships;
        int num3slotships;
        int num4slotships;
        int num5slotships;
        int BOARD_SIZE;
      
        //setting variables according to entered BOARD_SIZE
        if (BOARD_SIZE == 1) {
            size = 7;
            int num2slotships = 1;
            int num3slotships = 2;
            int num4slotships = 1;
            int num5slotships = 1;
        }
        if (BOARD_SIZE == 2) {
            size = 14;
            int num2slotships = 2;
            int num3slotships = 3;
            int num4slotships = 1;
            int num5slotships = 2;
        }
        if (BOARD_SIZE == 3) {
            size = 20;
            int num2slotships = 2;
            int num3slotships = 3;
            int num4slotships = 2;
            int num5slotships = 3;
        }
      
        //creating boards of appropriate size
        char guessBoard[size][size];
        char shipBoard[size][size];
        char enemyShipBoard[size][size];
      
        //printing board and number of remaining ships
        printBoard(shipBoard);
        printf("Number of 2 Slot Ships: %d\n", num2slotships);
        printf("Number of 3 Slot Ships: %d\n", num3slotships);
        printf("Number of 4 Slot Ships: %d\n", num4slotships);
        printf("Number of 5 Slot Ships: %d\n", num5slotships);
        //--------------------------ACTUAL PLACEMENT BEGINS HERE--------------------------
        char *buffer[MESSAGE_BUFFER_SIZE];
        //temp loop to place all 2 slot ships first, may replace later with random ship selection.
        while(num2slotships) {
          
            //-------------Asking for first coordinate-------------
            printf("Where would you like to place the first point of your 2 slot ship?\n");
          
            //ask for row
            //loop to make sure row input is in the array
            int rowcheck = 0;
            while(!rowcheck) {
                int row = 0;
                printf("Enter the x coordinate: ");
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
                int column = 0;
                printf("Enter the y coordinate: ");
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
              
            //check if there is a ship already at the entered location
            if (shipBoard[row][column] == '0')) {
                printf("There is already a ship there. Please enter different coordinates.\n");
            }
              
              
            //-------------Asking for second coordinate-------------
            printf("Where would you like to place the second point of your 2 slot ship?\n");
              
            //ask for row
            //loop to make sure row input is in the array
            int row2check = 0;
            while(!row2check) {
                int row2 = 0;
                printf("Enter the x coordinate: ");
                fgets(buffer, sizeof(buffer), stdin);
                row2 = atoi(buffer);
                  
                //if column is within range, pass column check
                if((row2 > 0) && (row2 <= size)) {
                    row2check = 1;
                }
                  
                //else print error and ask for another input
                else {
                    printf("That is not a valid row number.\n");
                }
            }
              
            //ask for column
            //loop to make sure column input is in the array
            int column2check = 0;
            while(!column2check) {
                int column2 = 0;
                printf("Enter the y coordinate: ");
                fgets(buffer, sizeof(buffer), stdin);
                column2 = atoi(buffer);
                
                //if column is within range, pass column check
                if((column2 > 0) && (column2 <= size)) {
                    column2check = 1;
                }
                
                //else print error and ask for another input
                else {
                    printf("That is not a valid column number.\n");
                }
            }
              
            //now here comes the difficult part!
            //check that the ship coordinates:
                //are appropriately sized (ex: 1 apart if the ship is 2 slots long)
                //are horizontally or vertically distanced from one another (no diagonal ships)
                //do not cross over any other ships
            //if the distance between the rows == the ships length and the distance between the columns == 0, or vice versa
            //everything is okay
            //if not, then they have to place shit differently
            if(!(abs(row2 - row) == 1 && abs(column2 - column) == 0) || (abs(row2 - row) == 0 && abs(column2 - column) == 1)) {
                printf("Ships don't bend like that. Please place the front and back of your ship appropriately.\n")
            }
            //next, check that the ship does not cross over any other ships
            
            
        }
      
      //----------------------------------------------------END OF PLACEMENT PHASE----------------------------------------------------
      */

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

//used for setup phase
char inputSettings(char *question, char *type) {
  //char input[16];
  //memset(&input[0], 0, sizeof(input));
  //char *p;
  while(1) {
    char input[16];
    char *p;
    //Ask for setting input
    printf("%s", question);
    fgets(input, sizeof(input), stdin);
    //printf("FGETS COMPLETE\n");
    //remove newline char added when entering number
    p = strchr(input, '\n');
    p = 0;

    //if entering board
  	if (!strcmp(type, "board")) {
      if ((atoi(input) >= 1) && (atoi(input) <= 3)) {
        return atoi(input);
      }
      else {
        printf("Value not in range, please enter again.\n");
      }
    }

  	if (!strcmp(type, "emoji")) {
      if ((atoi(input) >= 1) && (atoi(input) <= 2))  {
        //printf("EMOJI VALUE RIGHT\n");
        //printf("Printing input within emoji: %s\n", input);
        //printf("printing atoi within emoji: %d\n", atoi(input));
        return atoi(input);
      }
      else {
        //printf("EMOJI VALUE WRONG\n");
        printf("Value not in range, please enter again.\n");
      }
    }

  	if (!strcmp(type, "confirm")) {
      if (atoi(input) == 1) {
        return atoi(input);
      }
      else if (atoi(input) == 2) {
        printf("~~~~~~~~~Starting Settings Over~~~~~~~~~\n");
        return 0;
      }
      else {
        printf("Value not in range, please enter again\n");
      }
    }
  }
}
