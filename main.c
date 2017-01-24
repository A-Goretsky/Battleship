#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

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

    //SETUP PHASE: SERVER ONLY
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
    
    //CONNECTION PHASE
    //new attempt at settings
    
   
   


    char b9[MESSAGE_BUFFER_SIZE];
    printf("What would you like the size of the board to be?\n\t1. Small (7x7)\n\t2. Medium (14x14)\n\t3. Large (20x20)\nEnter Value: ");
    fgets(b9, sizeof(b9), stdin);
    char *p = strchr(b9, '\n');
    *p = 0;
    
    printf("printing b9: %c\n", b9);
    printf("printing board size outside of while: %c", EMOJI_SET);

    while (1) {
      printf("Attempting Connection...\n");
      connection = server_connect( sd );
      close(sd);
      char buffer[MESSAGE_BUFFER_SIZE];
      
      //Print Rules on Client End.
      printf("JUST A RANDOM PRINT\n");
      //printf("printing BOARD_SIZE: %c\n", BOARD_SIZE);
      //strcpy(buffer, "hi");
      write(connection, b9, sizeof(b9));
      //Main while loop
      while (read(connection, buffer, sizeof(buffer) )) {
        printf("[SERVER %d] received: %s\n", getpid(), buffer );
        write(connection, buffer, sizeof(buffer));  
      }
      exit(0);
      close( connection );
      /*
      int size;
      int num2slotships;
      int num3slotships;
      int num4slotships;
      int num5slotships;
      int BOARD_SIZE;
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
      
      char guessBoard[size][size];
      char shipBoard[size][size];
      char enemyShipBoard[size][size];
      
      printBoard(shipBoard);
      printf("Number of 2 Slot Ships: %d\n", num2slotships);
      printf("Number of 3 Slot Ships: %d\n", num3slotships);
      printf("Number of 4 Slot Ships: %d\n", num4slotships);
      printf("Number of 5 Slot Ships: %d\n", num5slotships);
      
      int xcoor = 0;
      int ycoor = 0;
      char *buffer[MESSAGE_BUFFER_SIZE];
      while(num2slotships) {
          
          printf("Where would you like to place the first point of your 2 slot ship?\n");
          int row = 0;
          printf("Enter the x coordinate: ");
          fgets(buffer, sizeof(buffer), stdin);
          row = atoi(buffer);
          gb 
          int column = 0;
          printf("Enter the y coordinate: ");
          fgets(buffer, sizeof(buffer), stdin);
          column = atoi(buffer);
          
          if (shipBoard[row][column] == '0') {
            
          }
      }
      
      
      */
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

    char b2[MESSAGE_BUFFER_SIZE];
    read(sd, b2, sizeof(b2));
    printf("wassup\n");
    //atoi(b2);
    printf("printing b2: %c\n", b2);
    //printSettings((int)b2, EMOJI_SET);
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
