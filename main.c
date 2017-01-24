#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include "networking.h"

void process( char * s );
void sub_server( int sd );
int inputSettings(char *question, char *type);

int main() {

  char b1[MESSAGE_BUFFER_SIZE];
  printf("Server (s) or Client (c)?\n");
  fgets( b1, sizeof(b1), stdin );
  char *p = strchr(b1, '\n');
  *p = 0;
  if(!strcmp(b1, "s")){
    int sd, connection;
    printf("Successfully received server input...\n");
    sd = server_setup();

    //SETUP PHASE: SERVER ONLY
    int CONFIRMATION = 0;
    while(!CONFIRMATION) {
      int BOARD_SIZE = inputSettings("What would you like the size of the board to be?\n\
                                      \t1. Donald Trump's Hands\n\
                                      \t2. Bourgeoisie\n\
                                      \t3. Yuge\n\
                                      Enter Value: ", "board");
		  int EMOJI_SET = inputSettings("Would you like Emoji or standard ASCII graphics?\n\
		                                    \t1. Emoji\n\
		                                    \t2.Standard\n\
		                                    Enter Value: ", "emoji");

		  char *settings = "Are you okay with these settings?\n";
		  char BSString[16];
		  sprintf(BSString, "%d", BOARD_SIZE);
		  char ESString[16];
		  sprintf(ESString, "%d", EMOJI_SET);
		  strcat(strcat(strcat(settings, BSString), ESString),
		         "\t1. Yes, these settings are correct.\n\
		          \t2. No, I would like to start over.\n\
		          Enter Value: ");
		
		  CONFIRMATION = inputSettings(settings, "confirm");
    }
    
    //CONNECTION PHASE
    
    while (1) {
      printf("Attempting Connection...\n");
      connection = server_connect( sd );

      int f = fork();
      if ( f == 0 ) {

        close(sd);
        sub_server( connection );

        exit(0);
      }
      else {
        close( connection );
      }
    }
  }
  else if(!strcmp(b1, "c")){
    char *host;
    printf("What IP would you like to connect to?\n");
    fgets( host, sizeof(host), stdin );
    char *cut = strchr(host, '\n');
    *cut = 0;

    int sd;

    sd = client_connect( host );

    char b2[MESSAGE_BUFFER_SIZE];

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


void sub_server( int sd ) {

  char buffer[MESSAGE_BUFFER_SIZE];
  while (read( sd, buffer, sizeof(buffer) )) {

    printf("[SERVER %d] received: %s\n", getpid(), buffer );
    process( buffer );
    write( sd, buffer, sizeof(buffer));
  }

}
void process( char * s ) {

  while ( *s ) {
    *s = (*s - 'a' + 13) % 26 + 'a';
    s++;
  }
}


//used for setup phase
int inputSettings(char *question, char *type) {
  char input[1000];
  char *p;
  while(1) {
    //Ask for setting input
    printf("%s", question);
    fgets(input, sizeof(input), stdin);

    //remove newline char added when entering number
    p = strchr(input, '\n');
    p = 0;

    //if entering board
  	if (!strcmp(type, "board")) {
      if ((atoi(input) >= 1) || (atoi(input) <= 3)) {
        return atoi(input);
      }
      else {
        printf("Value not in range, please enter again.\n");
      }
    }

  	if (!strcmp(type, "emoji")) {
      if ((atoi(input) >= 1) || (atoi(input) <= 2))  {
        return atoi(input);
      }
      else {
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
