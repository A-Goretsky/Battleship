# Battleship
Systems Final Project
by Dylan Wright, Anton Goretsky, and Stephan Doan

## What is fully integrated:
Everything works as advertised up to the final stages of networking implementation of the game phase, which utilizes new networking protocols (aka htons, due to past problems with simple buffers).

## How to Compile and Use Our Project
1. In order to use this project, you will need two terminals running the program at the same time. As a result, either open a terminal on two separate computers, or open up two terminals on one singular computer.
    * Please make the terminal the largest possible on your screen. Best result would be achieved closer to a full screen, especially with the wide board.
2. On each computer you are using, clone this git directory.
3. On each computer, through terminal, cd into the folder containing the cloned directory.
4. "Make run" in order to compile and run the contents of the directory. You will be presented with the option to run the program as either a server or a client.
5. In one terminal, run the program as a server and enter your desired board size (Small, Medium or Large). After selecting your desired board size, the server side program will then await a connection from the client side program
6. Once the server terminal is waiting for a connection, in your second terminal run the program as a client.
7. You will be prompted to enter the ip of the port of the computer running the server side of the program. In order to begin the game, enter the aforementioned computer's ip, and the two programs will begin the game.

## Files that Should Be in the Repository
* README.md: A markdown file containing an easy-to-understand description of how to use our project and the files contained within this repository. You're reading it right now!
* DESIGN: A plain text file containing an outline of our project design, work breakdown, and expected completion dates of various parts of our project.
* DEVLOG: A devlog containing the actual completion dates of various parts of our project. (Who worked on what and when)
* main.c: A .c file containing the core code for our setup, placement, gameplay, and completion phases.
* main.h: A .h file containing the headers of all functions to be used in main.c.
* print.c: A .c file containing the code for printing the boards for the placement and gameplay phases.
* print.h: A .h file containing the headers of all functions to be used in print.c.
* networking.c: A .c file containing the functions used in our connection phase.
* networking.h: A .h file containing the headers of all functions to be used in networking.c.
* makefile: A makefile to allow for the quick and easy compilation and running of our project.
