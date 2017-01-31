#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "networking.h"
#include "main.h"
#include "print.h"




int main() {
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
    char conbuff1[MESSAGE_BUFFER_SIZE];
    printf("What would you like the size of the board to be?\n\t1. Small (8x8)\n\t2. Medium (10x10)\n\t3. Large (14x14)\nEnter Value: ");
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

      read(connection, conbuff1, 25);

    //----------------------------------------------------PLACEMENT PHASE (SERVER)----------------------------------------------------

    //variables both server and client need for placement
    int size;

    //setting variables according to entered BOARD_SIZE
    if (BOARD_SIZE == 1) {
        size = 8;
    }
    if (BOARD_SIZE == 2) {
        size = 10;
    }
    if (BOARD_SIZE == 3) {
        size = 14;
    }

    //creating boards of appropriate size
    char guessBoard_S[14][14];
    char shipBoard_S[14][14];

    int temp1_s = 0;
    int temp2_s = 0;
    for (temp1_s; temp1_s < size; temp1_s++) {
        for (temp2_s; temp2_s < size; temp2_s++) {
            guessBoard_S[temp1_s][temp2_s] = ' ';
            shipBoard_S[temp1_s][temp2_s] = ' ';
        }
        temp2_s = 0;
    }

    memcpy(shipBoard_S, shipPlacement(BOARD_SIZE), sizeof(char) * 14 * 14);

    //printf("\n");
    //printf("PRINTING BOARD AFTER PLACEMENT\n");
    //printBoard_place(shipBoard_S, size);
    //----------------------------------------------------END OF PLACEMENT PHASE (SERVER)----------------------------------------------------

    //----------------------------------------------------GAME PHASE (SERVER)----------------------------------------------------
    int game = 1;
    int CONFIRMATION = 0;
    int x, y, hit, hitN, row, column, rowcheck, columncheck, lastcheck, boardres_S, boardresN_S;
    int xnet, ynet;
    x = 0;
    y = 0;
    xnet = 0;
    ynet = 0;
    hit = 0;
    hitN = 0;
    char *b3[MESSAGE_BUFFER_SIZE];
    char *conbuff[MESSAGE_BUFFER_SIZE];
    while(game){
        while(!CONFIRMATION) {
            printf("What coordinates would you like to attack?\n");
            //ask for row
            //loop to make sure row input is in the array
            rowcheck = 0;
            while(!rowcheck) {
                row = 0;
                printf("Enter the Row: ");
                fgets(b3, sizeof(b3), stdin);
                row = atoi(b3);
                //if row is within range, pass row check
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
            columncheck = 0;
            while(!columncheck) {
                column = 0;
                printf("Enter the Column: ");
                fgets(b3, sizeof(b3), stdin);
                column = atoi(b3);

                //if column is within range, pass column check
                if((column > 0) && (column <= size)) {
                    columncheck = 1;
                }

                //else print error and ask for another input
                else {
                    printf("That is not a valid column number.\n");
                }
            }

            //check if guessed
            if (!(guessBoard_S[row-1][column-1] == ' ')) {
                printf("You've already guessed this location.");
            }
            else {
                //confirm selection
                lastcheck = 0;
                printf("Are you sure you want to attack these coordinates?\n\t1. Yes\n\t2. No\nEnter Value: ");
                fgets(b3, sizeof(b3), stdin);
                lastcheck = atoi(b3);

                if (lastcheck == 1) {
                    CONFIRMATION = 1;
                }
            }
        }
        //NETWORKING BEGINS
        printf("Printing x before server send: %d", x);
        printf("Printing x before server send: %d", y);
        ynet = htons(row-1);
        xnet = htons(column-1);
        printf("Printing xnet before server send: %d", xnet);
        printf("Printing xynet before server send: %d", ynet);
        //Send y coord to client.
        printf("conversions complete\n");
        write(connection, &ynet, sizeof(ynet));

        printf("sent y\n");
        //Receive Confirmation Message (auto)
        read(connection, conbuff, 30);


        //Send x coord to client.

        write(connection, &xnet, sizeof(xnet));
        printf("sent x\n");

        //Receive Confirmation Message (boolean from client)
        read(connection, &hitN, sizeof(hitN));
        hit = ntohs(hitN);

        printf("printing hit: %d\n", hit);
        //Hit or miss array set.
        if(hit){
            guessBoard_S[row-1][column-1] = '*';
        }
        else{
            guessBoard_S[row-1][column-1] = 'x';
        }

        //Signal client can start writing.
        write(connection, "turn complete", 30);
        //next turn
        printf("completed attack\n");
        xnet = 0;
        ynet = 0;
        //Receive client x coord
        printf("reading coord from client\n");
        read(connection, &ynet, sizeof(ynet));
        printf("received y\n");
        y = ntohs(y);
        //Send confirmation message
        write(connection, "coordinate received", 30);

        //Receive client y coord
        read(connection, &xnet, sizeof(xnet));
        x = ntohs(x);
        //Send boolean: True = hit; false = miss
        boardres_S = shipBoard_S[y][x] == 'o';
        boardresN_S = htons(boardres_S);

        write(connection, &boardresN_S, sizeof(boardresN_S));

        //STEPHAN
        //Receive Confirmation message
        read(connection, conbuff, sizeof(conbuff));
    }
    //----------------------------------------------------END OF GAME PHASE (SERVER)----------------------------------------------------


      //Main while loop
      while (read(connection, buffer, sizeof(buffer) )) {
        printf("[SERVER %d] received: %s\n", getpid(), buffer );
        write(connection, buffer, sizeof(buffer));
      }
      exit(0);
      close( connection );

    } //closes while
  } //closes server

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
    BOARD_SIZE = 0;
    read(sd, &BOARD_SIZE, sizeof(BOARD_SIZE));

    //STEPHAN
    //Send response to client
    write(sd, "rules recieved", 30);

    char b2[MESSAGE_BUFFER_SIZE];
    printf("wassup\n");
    printf("printing received: %x\n", BOARD_SIZE);
    printf("printing ntohs(BOARD_SIZE): %d\n", ntohs(BOARD_SIZE));
    BOARD_SIZE = ntohs(BOARD_SIZE);
    if(BOARD_SIZE==1){
        printf("Board Size is Small (8x8)\n");
    }
    else if(BOARD_SIZE==2){
        printf("Board Size is Medium (14x14)\n");
    }
    else if(BOARD_SIZE==3){
        printf("Board Size is Large (20x20)\n");
    }
    else{
        printf("Something went wrong during the connection phase.\n");
    }

    //----------------------------------------------------PLACEMENT PHASE (CLIENT)----------------------------------------------------

    //variables both server and client need for placement
    int size;

    printf("Printing size at creation: %d\n", size);
    //setting variables according to entered BOARD_SIZE

    printf("Printing Board_size %d\n", BOARD_SIZE);
    if (BOARD_SIZE == 1) {
          size = 8;
    }
    if (BOARD_SIZE == 2) {
          size = 10;
    }
    if (BOARD_SIZE == 3) {
          size = 14;
    }

    printf("Printing size after Board_size test: %d\n", size);
    //creating boards of appropriate size
    char guessBoard_C[14][14];
    char shipBoard_C[14][14];

    //Set board to blank spaces.
    int temp1_c = 0;
    int temp2_c = 0;
    for (temp1_c; temp1_c < size; temp1_c++) {
        for (temp2_c; temp2_c < size; temp2_c++) {
            guessBoard_C[temp1_c][temp2_c] = ' ';
            shipBoard_C[temp1_c][temp2_c] = ' ';
        }
        temp2_c = 0;
    }

    memcpy(shipBoard_C, shipPlacement(BOARD_SIZE), sizeof(char) * 14 * 14);

    //printf("\n");
    //printf("PRINTING BOARD AFTER PLACEMENT\n");
    //printBoard_place(shipBoard_C, size);
    //----------------------------------------------------END OF PLACEMENT PHASE (CLIENT)----------------------------------------------------

    //----------------------------------------------------GAME PHASE (CLIENT)----------------------------------------------------
    int game = 1;
    int CONFIRMATION = 0;
    int x, y, hit, hitN, row, column, rowcheck, columncheck, lastcheck, error, boardresN, boardres;
    boardres = 0;
    boardresN = 0;
    x = 0;
    y = 0;
    int xnet, ynet;
    xnet = 0;
    ynet = 0;
    hit = 0;
    hitN = 0;
    char *b3[MESSAGE_BUFFER_SIZE];
    char conbuff[MESSAGE_BUFFER_SIZE];
    while(game){

        //Read y coord from server
        printf("y read begins\n");
        error = read(sd, &ynet, sizeof(ynet));
        if(error){
            perror("problemo");
        }

        //Send conf message
        write(sd, "coordinate received", 30);

        printf("received y\n");
        //Read y coord from server
        read(sd, &xnet, sizeof(xnet));

        printf("received x\n");
        printf("Printing xnet before conversion: %d", xnet);
        printf("Printing ynet before conversion: %d", ynet);

        //Convert from network to normal int
        x = ntohs(x);
        y = ntohs(y);
        printf("printing y: %d\n", y);
        printf("printing x: %d\n", x);
        //Send boolean conf. true = server hit; false = server missed
        boardres = 0;
        boardresN = 0;
        boardres = shipBoard_C[x][y] == 'o';
        boardresN = htons(boardres);

        write(sd, boardresN, sizeof(boardresN));


        //wait for confirmation message from server for client_connect's turn
        read(sd, conbuff, sizeof(conbuff));

        //NEXT TURN BEGINS
        while(!CONFIRMATION){
            printf("What coordinates would you like to attack?\n");
            //ask for row
            //loop to make sure row input is in the array
            *b3[MESSAGE_BUFFER_SIZE];
            rowcheck = 0;
            while(!rowcheck) {
                row = 0;
                printf("Enter the Row: ");
                fgets(b3, sizeof(b3), stdin);
                row = atoi(b3);

                //if row is within range, pass row check
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
                fgets(b3, sizeof(b3), stdin);
                column = atoi(b3);

                //if column is within range, pass column check
                if((column > 0) && (column <= size)) {
                    columncheck = 1;
                }

                //else print error and ask for another input
                else {
                    printf("That is not a valid column number.\n");
                }
            }

            //check if guessed
            if (!(guessBoard_C[row-1][column-1] == ' ')) {
                printf("You've already guessed this location.");
            }
            else {
                //confirm selection
                int lastcheck = 0;
                printf("Are you sure you want to attack these coordinates?\n\t1. Yes\n\t2. No\nEnter Value: ");
                fgets(b3, sizeof(b3), stdin);
                lastcheck = atoi(b3);

                if (lastcheck == 1) {
                    CONFIRMATION = 1;
                }
            }
        }

        ynet = 0;
        xnet = 0;
        //Conver from int to network (to send to server)
        ynet = htons(row-1);
        xnet = htons(column-1);

        //CLIENT TURN NETWORKING BEGINS (Sending to server)

        //Send y coord to server
        write(sd, &ynet, sizeof(int));
        //Receive confirmation from server
        read(sd, conbuff, sizeof(conbuff));

        //Send x coord to server
        write(sd, &xnet, sizeof(int));
        //Receive confirmation from server (Received true or false for hit)
        read(sd, &hitN, sizeof(hitN));
        hit = ntohs(hitN);
        //Update personal (client) guess board with hit or miss
        if(hit){
            guessBoard_C[row-1][column-1] = '*';
        }
        else{
            guessBoard_C[row-1][column-1] = 'x';
        }
    }
    //----------------------------------------------------END OF GAME PHASE (CLIENT)----------------------------------------------------



    while (1) {
      printf("enter message: ");
      fgets( b2, sizeof(b2), stdin );
      char *p = strchr(b2, '\n');
      *p = 0;

      write( sd, b2, sizeof(b2) );
      read( sd, b2, sizeof(b2) );
      printf( "received: %s\n", b2 );
    }

  } //closes if c

  else{
    printf("invalid option");
  }

  return 0;
} //close main











char ** shipPlacement(int BOARD_SIZE) {

    printf("SHIP PLACEMENT RUN\n");

    //variables both server and client need for placement
    int size;
    int num2slotships;
    int num3slotships;
    int num4slotships;
    int num5slotships;

    //creating board to edit
    char shipBoard[14][14];

    int CONFIRMATION = 0;
    while (!CONFIRMATION) {
        //setting variables according to entered BOARD_SIZE
        if (BOARD_SIZE == 1) {
            size = 8;
            num2slotships = 1;
            num3slotships = 2;
            num4slotships = 1;
            num5slotships = 1;
        }
        if (BOARD_SIZE == 2) {
            size = 10;
            num2slotships = 2;
            num3slotships = 3;
            num4slotships = 2;
            num5slotships = 1;
        }
        if (BOARD_SIZE == 3) {
            size = 14;
            num2slotships = 2;
            num3slotships = 3;
            num4slotships = 2;
            num5slotships = 3;
        }

        int temp1 = 0;
        int temp2 = 0;
        for (temp1; temp1 < size; temp1++) {
            for (temp2; temp2 < size; temp2++) {
                shipBoard[temp1][temp2] = ' ';
            }
            temp2 = 0;
        }

        printf("\033[2J");

        printf("~~~~~How Placement Works~~~~~\n");
        printf("- You will be prompted to enter one coordinate (Row and Column) and the direction of placement.\n");
        printf("- That coordinate will act as the starting point, with the rest of the ship extending to the\ndirection of choice. Ships will appear on the board as they are placed.\n");
        printf("\t1. Continue\n");

        int continuev = 0;
        while (!continuev) {
            printf("Enter Value: ");
            char input[MESSAGE_BUFFER_SIZE];
            fgets(input, sizeof(input), stdin);
            char *cutoff = strchr(input, '\n');
            *cutoff = 0;

            if (atoi(input) == 1) {
                continuev = atoi(input);
            }
            else {
                printf("Please enter the number 1...\n");
            }
        }

        printf("\n");

        while(num2slotships + num3slotships + num4slotships + num5slotships) {

            printf("\033[2J");

            //printing board and number of remaining ships
            printf("~~~~~~~Displaying Board~~~~~~~\n");
            printf("\n");
            printBoard_place(shipBoard, size);
            printf("~~~~~~~Ships left to place~~~~~~~\n");
            printf("Number of 2 Slot Ships: %d\n", num2slotships);
            printf("Number of 3 Slot Ships: %d\n", num3slotships);
            printf("Number of 4 Slot Ships: %d\n", num4slotships);
            printf("Number of 5 Slot Ships: %d\n", num5slotships);
            printf("\n");

            //loop until acceptable input entered
            int acceptableInput = 0;
            while(!acceptableInput) {
                //ask for a ship input
                printf("Which type of ship would you like to place down?\n\t1. 2 Slot Ship\n\t2. 3 Slot Ship\n\t3. 4 Slot Ship\n\t4. 5 Slot Ship\nEnter value: ");
                char *shipInput[MESSAGE_BUFFER_SIZE];
                fgets(shipInput, sizeof(shipInput), stdin);

                //cut off trailing \n
                char *cut = strchr(shipInput, '\n');
                *cut = 0;

                //convert string input to int
                int shipSelect = 0;
                shipSelect = atoi(shipInput);

                //choose proper input for placement function based off of entered ship selection
                if (shipSelect == 1) {
                    if (num2slotships) {
                        memcpy(shipBoard, placeShip(shipBoard, 2, size), sizeof(char) * size * size);
                        acceptableInput = 1;
                        num2slotships--;
                    }
                    else {
                        printf("\033[2J");
                        printf("You have no more 2 slot ships to place.\n");
                        printf("\n");
                        printf("~~~~~~~Displaying Board~~~~~~~\n");
                        printf("\n");
                        printBoard_place(shipBoard, size);
                        printf("\n");
                        printf("~~~~~~~Ships left to place~~~~~~~\n");
                        printf("Number of 2 Slot Ships: %d\n", num2slotships);
                        printf("Number of 3 Slot Ships: %d\n", num3slotships);
                        printf("Number of 4 Slot Ships: %d\n", num4slotships);
                        printf("Number of 5 Slot Ships: %d\n", num5slotships);
                        printf("\n");
                    }
                }
                else if (shipSelect == 2)  {
                    if (num3slotships) {
                        memcpy(shipBoard, placeShip(shipBoard, 3, size), sizeof(char) * size * size);
                        acceptableInput = 1;
                        num3slotships--;
                    }
                    else {
                        printf("\033[2J");
                        printf("You have no more 3 slot ships to place.\n");
                        printf("\n");
                        printf("~~~~~~~Displaying Board~~~~~~~\n");
                        printf("\n");
                        printBoard_place(shipBoard, size);
                        printf("\n");
                        printf("~~~~~~~Ships left to place~~~~~~~\n");
                        printf("Number of 2 Slot Ships: %d\n", num2slotships);
                        printf("Number of 3 Slot Ships: %d\n", num3slotships);
                        printf("Number of 4 Slot Ships: %d\n", num4slotships);
                        printf("Number of 5 Slot Ships: %d\n", num5slotships);
                        printf("\n");
                    }
                }
                else if (shipSelect == 3)  {
                    if (num4slotships) {
                        memcpy(shipBoard, placeShip(shipBoard, 4, size), sizeof(char) * size * size);
                        acceptableInput = 1;
                        num4slotships--;
                    }
                    else {
                        printf("\033[2J");
                        printf("You have no more 4 slot ships to place.\n");
                        printf("\n");
                        printf("~~~~~~~Displaying Board~~~~~~~\n");
                        printf("\n");
                        printBoard_place(shipBoard, size);
                        printf("\n");
                        printf("~~~~~~~Ships left to place~~~~~~~\n");
                        printf("Number of 2 Slot Ships: %d\n", num2slotships);
                        printf("Number of 3 Slot Ships: %d\n", num3slotships);
                        printf("Number of 4 Slot Ships: %d\n", num4slotships);
                        printf("Number of 5 Slot Ships: %d\n", num5slotships);
                        printf("\n");
                    }
                }
                else if (shipSelect == 4)  {
                    if (num5slotships) {
                        memcpy(shipBoard, placeShip(shipBoard, 5, size), sizeof(char) * size * size);
                        acceptableInput = 1;
                        num5slotships--;
                    }
                    else {
                        printf("\033[2J");
                        printf("You have no more 5 slot ships to place.\n");
                        printf("\n");
                        printf("~~~~~~~Displaying Board~~~~~~~\n");
                        printf("\n");
                        printBoard_place(shipBoard, size);
                        printf("\n");
                        printf("~~~~~~~Ships left to place~~~~~~~\n");
                        printf("Number of 2 Slot Ships: %d\n", num2slotships);
                        printf("Number of 3 Slot Ships: %d\n", num3slotships);
                        printf("Number of 4 Slot Ships: %d\n", num4slotships);
                        printf("Number of 5 Slot Ships: %d\n", num5slotships);
                        printf("\n");
                    }
                }
                else {
                    printf("\033[2J");
                    printf("-%d- is not a valid input.\n", shipSelect);
                    printf("\n");
                    printf("~~~~~~~Displaying Board~~~~~~~\n");
                    printf("\n");
                    printBoard_place(shipBoard, size);
                    printf("\n");
                    printf("~~~~~~~Ships left to place~~~~~~~\n");
                    printf("Number of 2 Slot Ships: %d\n", num2slotships);
                    printf("Number of 3 Slot Ships: %d\n", num3slotships);
                    printf("Number of 4 Slot Ships: %d\n", num4slotships);
                    printf("Number of 5 Slot Ships: %d\n", num5slotships);
                    printf("\n");
                }
            }
        }

        printf("\033[2J");

        //printing board and number of remaining ships
        printf("~~~~~~~Displaying Board~~~~~~~\n");
        printf("\n");
        printBoard_place(shipBoard, size);
        printf("All ships have been placed.\n");
        printf("\n");


        //confirmation of ships
        int finalcheck = 0;
        while (!finalcheck) {

            printf("Are you happy with this ship placement?\n\t1. Yes, I would like continue to the game.\n\t2. No, I want to reset all my ships.\nEnter Value: ");
            char *input[MESSAGE_BUFFER_SIZE];
            fgets(input, sizeof(input), stdin);

            //cut off trailing \n
            char *cutinp = strchr(input, '\n');
            *cutinp = 0;
            int input1 = atoi(input);
            if ((input1 < 1) || (input1 > 2)) {
                printf("\033[2J");
                printf("That is not a valid input. Please enter either 1 or 2.\n");
                printf("\n");
                printf("~~~~~~~Displaying Board~~~~~~~\n");
                printf("\n");
                printBoard_place(shipBoard, size);
                printf("All ships have been placed.\n");
                printf("\n");
            }
            else {
                finalcheck = 1;
                if (input1 == 1) {
                    CONFIRMATION = 1;
                }
            }
        }
    }

    //printf("PRINTING BOARD INSIDE SHIP PLACEMENT\n");
    //printBoard_place(shipBoard, size);
    //printf("RETURNING BOARD\n");
    return shipBoard;
}













char ** placeShip(char shipBoard[][14], int shipLength, int size) {

    int row = 0;
    int column = 0;
    int direction = 0;
    int valid = 0;

    printf("\033[2J");
    //printf("\n");
    //printf("\n");
    //printf("\n");
    //printf("\n");
    //printf("\n");

    while(!valid) {
        valid = 1;
        //Print Board.
        //BOARD PRINTING METHOD HERE. Clear terminal and print board at top with a few new lines.
        printf("~~~~~~~Displaying Board~~~~~~~\n");
        printf("\n");
        printf("You are placing down a ship of length: %d\n", shipLength);
        printf("\n");
        printBoard_place(shipBoard, size);
        printf("\n");

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

            //if row is within range, pass row check
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
        //if direction = north, check row -> row - length, check for array conflicts, check for ship conflicts.
        if (direction == 1) {
            printf("STARTING CHECKER FOR DIR 1\n");
            int rowpointer = row - 1;
            printf("printing ORIGINAL rowpointer: %d\n", rowpointer);
            printf("rowpointer (OUT OF FOR): %d\n", rowpointer);
            printf("row (OUT OF FOR): %d\n", row);
            printf("shipLength (OUT OF FOR): %d\n", shipLength);
            printf("row - shipLength - 1 (OUT OF FOR) %d\n", row - shipLength - 1);
            for(rowpointer; rowpointer > (row - shipLength - 1); rowpointer--) {
                printf("rowpointer: %d\n", rowpointer);
                if ((rowpointer < 0) || (rowpointer >= size)) {
                    printf("OUT OF ARRAY ERROR\n");
                    valid = 0;
                }
                else if (!(shipBoard[rowpointer][column - 1] == ' ')) {
                    printf("SHIP CONFLICT ERROR\n");
                    valid = 0;
                }
            }
        }
        //if direction = south, check row -> row + length, check for end of array, check for ship conflicts.
        if (direction == 2) {
            printf("STARTING CHECKER FOR DIR 2\n");
            int rowpointer = row - 1;
            printf("printing ORIGINAL rowpointer: %d\n", rowpointer);
            for(rowpointer; rowpointer < (row + shipLength - 1); rowpointer++) {
                printf("rowpointer: %d\n", rowpointer);
                if ((rowpointer < 0) || (rowpointer >= size)) {
                    printf("OUT OF ARRAY ERROR\n");
                    valid = 0;
                }
                else if (!(shipBoard[rowpointer][column - 1] == ' ')) {
                    printf("SHIP CONFLICT ERROR\n");
                    valid = 0;
                }
            }
        }
        //if direction = west, check column -> column - length, check for end of array, check for ship conflicts.
        if (direction == 3) {
            printf("STARTING CHECKER FOR DIR 3\n");
            int columnpointer = column - 1;
            printf("printing ORIGINAL columnpointer: %d\n", columnpointer);
            printf("columnpointer (OUT OF FOR): %d\n", columnpointer);
            printf("column (OUT OF FOR): %d\n", column);
            printf("shipLength (OUT OF FOR): %d\n", shipLength);
            printf("column - shipLength - 1 (OUT OF FOR) %d\n", column - shipLength - 1);
            for(columnpointer; columnpointer > (column - shipLength - 1); columnpointer--) {
                printf("printing columnpointer: %d\n", columnpointer);
                if ((columnpointer < 0) || (columnpointer >= size)) {
                    printf("OUT OF ARRAY ERROR\n");
                    valid = 0;
                }
                else if (!(shipBoard[row - 1][columnpointer] == ' ')) {
                    printf("SHIP CONFLICT ERROR\n");
                    valid = 0;
                }
            }
        }
        //if direction = east, check column -> row + length, check for end of array, check for ship conflicts.
        if (direction == 4) {
            printf("STARTING CHECKER FOR DIR 4\n");
            int columnpointer = column - 1;
            printf("printing ORIGINAL columnpointer: %d\n", columnpointer);
            for(columnpointer; columnpointer < (column + shipLength - 1); columnpointer++) {
                printf("printing columnpointer: %d\n", columnpointer);
                if ((columnpointer < 0) || (columnpointer >= size)) {
                    printf("OUT OF ARRAY ERROR\n");
                    valid = 0;
                }
                else if (!(shipBoard[row - 1][columnpointer] == ' ')) {
                    printf("SHIP CONFLICT ERROR\n");
                    valid = 0;
                }
            }
        }

        //after the row, column, and direction have been chosen and all inputs are valid
        //if the ship either goes off the board
        //or the ship crosses over another ship
        //the ship placement is not valid and therefore the loop restarts
        if (!valid) {
            printf("\033[2J");
            printf("That is not a valid ship placement.\n");
            printf("\n");

        }

    }

    printf("PLACE SHIPS SECTION REACHED\n");

    //place ships
    if (direction == 1) {
        int rowpointer = row - 1;
        for(rowpointer; rowpointer > (row - shipLength - 1); rowpointer--) {
            shipBoard[rowpointer][column - 1] = 'o';
        }
    }
    if (direction == 2) {
        int rowpointer = row - 1;
        for(rowpointer; rowpointer < (row + shipLength - 1); rowpointer++) {
            shipBoard[rowpointer][column - 1] = 'o';
        }
    }
    if (direction == 3) {
        int columnpointer = column - 1;
        for(columnpointer; columnpointer > (column - shipLength - 1); columnpointer--) {
            //printf("printing columnpointer: %d\n", columnpointer);
            shipBoard[row - 1][columnpointer] = 'o';
        }
    }
    if (direction == 4) {
        int columnpointer = column - 1;
        for(columnpointer; columnpointer < (column + shipLength - 1); columnpointer++) {
            //printf("printing columnpointer: %d\n", columnpointer);
            shipBoard[row - 1][columnpointer] = 'o';
        }
    }

    //execlp("clear", "clear");

    //return editted board

    return shipBoard;
}
