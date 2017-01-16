#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "networking.h"

void process( char * s );
void sub_server( int sd );

int main( int argc, char *argv[] ) {
  char b1[MESSAGE_BUFFER_SIZE]; 
  fgets( b1, sizeof(b1), stdin );
  char *p = strchr(b1, '\n');
  *p = 0;
  if(strcmp(b1, "s")){
    int sd, connection;
  
    sd = server_setup();
      
    while (1) {
  
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
  }else if(strcmp(b1, "c")){
    char *host;
    if (argc != 2 ) {
      printf("host not specified, conneting to 127.0.0.1\n");
      host = "127.0.0.1";
    }
    else
      host = argv[1];
    
    int sd;
  
    sd = client_connect( host );
  
    char b2[MESSAGE_BUFFER_SIZE];
    
    while (1) {
      printf("enter message: ");
      fgets( b2, sizeof(2), stdin );
      char *p = strchr(b2, '\n');
      *p = 0;
    
      write( sd, b2, sizeof(b2) );
      read( sd, b2, sizeof(b2) );
      printf( "received: %s\n", b2 );
    }
  }else{
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
