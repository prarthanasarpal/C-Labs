/*
 * Name:        Prarthana Sarpal
 * Date:        March 3rd, 2020
 * Version:     1.07
 * Description: The Reversi game, without AI.
 *              Plays the Reversi game for ONE run.
 */

/* 
 * File:   main.c
 * Author: prarthanasarpal
 *
 * Created on March 3, 2020, 7:31 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * 
 */
void initializeBoard(char board[][26], int dimension);
void printBoard(char board[][26], int dimension);
void printMoves(char board[][26], int dimension, char colour);
void flipInDirection(char board[][26], int dimension, int row, int col, char colour, int deltaRow, int deltaCol);

bool checkLegalInDirection(char board[][26], int dimension, int row, int col, char colour, int deltaRow, int deltaCol);
bool positionInBounds(int n, int row, int col);
bool moveValid(char board[][26], int dimension, char colour, int rows, int cols);

int checkValidAndFlip(char board[][26], int dimension, int row, int col, char colour, bool flip);

//void printAvailableMoves(char board[][26], int dimension, char colour);
//bool isMoveValid(char board[][26], int dimension, int rows, int cols);

int main(void) {
    // Notes
    // U - unoccupied
    // B - occupied by black
    // W - occupied by white

    // VARIABLES
    static char board[26][26] = {}; // 2D static game board
    int dimension; // user input

    // INPUT
    printf("Enter the board dimension: ");
    scanf("%d", &dimension);

    // initialize game board
    initializeBoard(board, dimension); // works!

    // print board
    printBoard(board, dimension); // works!

    // get board configuration  // works!
    // change board as moves are entered
    char inputColor;
    int inputRow, inputCol;

    printf("Enter board configuration: \n");

    do {
        // get user input
        if (getchar() != '!') {
            inputColor = getchar();

            if (inputColor != '!') {
                inputRow = getchar();

                if (inputRow != '!') {
                    inputCol = getchar();
                }
            }
        }

        // save moves to board
        if (inputColor != '!') {
            board[inputRow - 97][inputCol - 97] = inputColor;
        } 
        
    } while (inputColor != '!' && inputRow != '!' && inputCol != '!');

    // print updated board
    printBoard(board, dimension);

    // printing available moves for W
    //printAvailableMoves(board, dimension, 'W');
    printMoves(board, dimension, 'W');

    // printing available moves for B
    //printAvailableMoves(board, dimension, 'B');
    printMoves(board, dimension, 'B');

    // get a user inputted move
    printf("Enter a move: \n");
    
    int count = 0;
    char userColor; 
    int userRow, userCol;
    
    do {
        
        // works!
        if(getchar() != '!' ) {
            userColor = getchar();
            userRow = getchar();
            userCol = getchar();
        } 
    
    } while(getchar() == '!');
   
    userRow = userRow - 97;
    userCol = userCol - 97;
    
    // print if valid or not AND flip if needed.
    if(moveValid(board, dimension, userColor, userRow, userCol)) {
        
        // print
        printf("Valid move.\n");
        
        // update game board
        board[userRow][userCol] = userColor;
        
        // pass true for flip and false for not flipping
        checkValidAndFlip(board, dimension, userRow, userCol, userColor, true);
  
    } else {
        // print game board
        printf("Invalid move.\n");
    }
    
    // print game board
    printBoard(board, dimension);
    
    return 0;
}

void initializeBoard(char board[][26], int dimension) {

    int initialX = (dimension / 2) - 1;
    int initialY = (dimension / 2) - 1;

    for (int rows = 0; rows < dimension; rows++) {
        for (int cols = 0; cols < dimension; cols++) {

            // initial configuration
            if (rows == initialX && cols == initialY) {
                board[rows][cols] = 'W';
            } else if (rows == initialX && cols == (initialY + 1)) {
                board[rows][cols] = 'B';
            } else if (rows == (initialX + 1) && cols == initialY) {
                board[rows][cols] = 'B';
            } else if (rows == (initialX + 1) && cols == (initialY + 1)) {
                board[rows][cols] = 'W';
            }                // unoccupied
            else {
                board[rows][cols] = 'U';
            }

        }
    }

}

void printBoard(char board[][26], int dimension) {

    // Has to print with alphabet border!

    // printing horizontal alphabet
    int i = 0;
    printf("  ");
    do {
        printf("%c", (97 + i));
        i++;
    } while (i < dimension);

    printf("\n");

    // printing board with vertical alphabet
    for (int rows = 0; rows < dimension; rows++) {
        // printing vertical alphabet
        printf("%c ", (97 + rows));

        for (int cols = 0; cols < dimension; cols++) {
            printf("%c", board[rows][cols]);
        }
        printf("\n");
    }

}

void printMoves(char board[][26], int dimension, char colour) {
    
    printf("Available moves for %c: \n", colour);

    for (int rows = 0; rows < dimension; rows++) {
        for (int cols = 0; cols < dimension; cols++) {
            
            if(moveValid(board, dimension, colour, rows, cols)) {
                // print move
                printf("%c%c\n", (97 + rows), (97 + cols));
            }
            
        }
    }
    
}

bool moveValid(char board[][26], int dimension, char colour, int rows, int cols) {
    
    if (board[rows][cols] != 'W' && board[rows][cols] != 'B') {
        
        for (int i = -1; i < 2; i++) {
            for (int j = 1; j > -2; j--) {
                // works! 
                        
                if (checkLegalInDirection(board, dimension, rows, cols, colour, i, j)) {
                    return true;

                }

            }
        }
    }
    
    return false;
}

bool checkLegalInDirection(char board[][26], int dimension, int row, int col, char colour, int deltaRow, int deltaCol) {
    // works!

    int newRow = row + deltaRow;
    int newCol = col + deltaCol;

    if (positionInBounds(dimension, newRow, newCol)) {

        // check for color
        if (board[newRow][newCol] != colour && board[newRow][newCol] != 'U') {
            // legal: keep walking till we hit our own color
            newRow = newRow + deltaRow;
            newCol = newCol + deltaCol;

            for (int i = 0; i < dimension && positionInBounds(dimension, newRow, newCol); i++) {

                //newRow = newRow + deltaRow;
                //newCol = newCol + deltaCol;
                if(board[newRow][newCol] == 'U') {
                    return false;
                }
                
                if (board[newRow][newCol] == colour && positionInBounds(dimension, newRow, newCol)) {
                    //foundOwnColourInBounds = true;
                    return true;
                }
                
                newRow = newRow + deltaRow;
                newCol = newCol + deltaCol;

            }
        }

    }

    return false;
}

bool positionInBounds(int n, int deltaRow, int deltaCol) {

    if (deltaRow < n && deltaCol < n && deltaRow >= 0 && deltaCol >= 0) {
        return true;
    }

    return false;
}

// returns score
int checkValidAndFlip(char board[][26], int dimension, int row, int col, char colour, bool flip) {
    
    // row is userRow, col is userCol, color is userColor
    for(int i = -1; i < 2; i++) {
        for(int j = 1; j > -1; j--) {
            
            // flips in EVERY legal direction
            if(checkLegalInDirection(board, dimension, row, col, colour, i, j)) {
                // check if boolean flip is true
                if(flip) {
                    flipInDirection(board, dimension, row, col, colour, i, j);
                }
            }
            
        }
    }
    
    return 0;
}

void flipInDirection(char board[][26], int dimension, int row, int col, char colour, int deltaRow, int deltaCol) {
    
    int newRow = row + deltaRow;
    int newCol = col + deltaCol;
    
    do {
        
        if(board[newRow][newCol] != 'U') {
            board[newRow][newCol] = colour;
        }
        
        newRow = newRow + deltaRow;
        newCol = newCol + deltaCol;
        
    } while(board[newRow][newCol] != colour && positionInBounds(dimension, deltaRow, deltaCol));
    
}

// old code 
/*
void printAvailableMoves(char board[][26], int dimension, char colour) {

    printf("Available moves for %c: \n", colour);

    for (int rows = 0; rows < dimension; rows++) {
        for (int cols = 0; cols < dimension; cols++) {

            if (isMoveValid(board, dimension, rows, cols)) {
                // move is JUST UNOCCUPIED
                // check if legal in 8 possible directions
                // works!
                
                for (int i = -1; i < 2; i++) {
                    for (int j = 1; j > -2; j--) {
                        // works! 
                        
                        if (checkLegalInDirection(board, dimension, rows, cols, colour, i, j)) {
                            // print move
                            printf("%c%c\n", (97 + rows), (97 + cols));

                        }

                    }
                }

            }

        }
    }

}

bool isMoveValid(char board[][26], int dimension, int rows, int cols) {

    // move IS occupied -- NOT VALID
    if (board[rows][cols] == 'W' || board[rows][cols] == 'B') {
        return false;
    }
    // move is NOT occupied
    return true;

}
 * 
*/