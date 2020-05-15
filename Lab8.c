/*
 * Name:        Prarthana Sarpal
 * Date:        March 30th, 2020
 * Version:     1.05
 * Description: The Reversi game, with AI
 * 
 * Rules (for this version)
 *  'B' = black tile, 'W' = white tile, 'U' = unoccupied tile
 *  Black player always plays first
 *  Game is over and computer wins if human makes illegal move
 *  Computer AI plays the move with the highest score & lowest row and/ or 
 *  lowest column placement
 */

/* 
 * File:   main.c
 * Author: prarthanasarpal
 *
 * Created on March 30, 2020, 8:35 PM
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

bool checkLegalInDirection(char board[][26], int dimension, int row, int col, char colour, int deltaRow, int deltaCol);
bool positionInBounds(int n, int row, int col);
bool moveValid(char board[][26], int dimension, char colour, int rows, int cols);
bool moveAvailable(char board[][26], int dimension, char colour, int rows, int cols);

bool anyMoveAvailable(char board[][26], int dimension, char colour);
bool findMoves(char board[][26], int dimension, char colour);

int checkValidAndFlip(char board[][26], int dimension, int row, int col, char colour, bool flip);
int flipInDirection(char board[][26], int dimension, int row, int col, char colour, int deltaRow, int deltaCol, bool flip);

char findOpposite(char turn);

int main(void) {

    // VARIABLES
    static char board[26][26] = {}; // 2D static game board
    int dimension; // user input
    char computerColor, userColor, turn, winner;
    char userRow, userCol;
    bool gameOver = false;

    int score = 0;
    int maxScore = 0;
    char computerRow, computerCol;

    bool foundAvailableMoveOpposite = false;
    bool foundAvailableMove = false;

    // INPUT
    printf("Enter the board dimension: ");
    scanf("%d", &dimension);

    // initialize game board
    initializeBoard(board, dimension); // works!

    // determine computer color // works!
    // black color plays always first (for this program)
    printf("Computer plays (B/W): ");

    int count = 0;
    do {

        if (getchar() != '!') {
            computerColor = getchar();
        }
        count++;

    } while (count == 0);

    // determine user and turn color // works!
    if (computerColor == 'W') {
        userColor = 'B';
        turn = userColor;
    } else if (computerColor == 'B') {
        userColor = 'W';
        turn = computerColor;
    }

    // print board
    printBoard(board, dimension); // works!

    // START GAME
    // put in do while loop while game is not over
    // START WHILE LOOP
    while (!gameOver) {

        if (turn == computerColor && gameOver == false) {
            // make the move with the best score
            // if same score: use row and/ or column with lower index
            for (int rows = 0; rows < dimension; rows++) {
                for (int cols = 0; cols < dimension; cols++) {

                    if (moveAvailable(board, dimension, computerColor, rows, cols)) {

                        for (int i = -1; i < 2; i++) {
                            for (int j = 1; j > -2; j--) {
                                if (checkLegalInDirection(board, dimension, rows, cols, computerColor, i, j)) {

                                    score = flipInDirection(board, dimension, rows, cols, computerColor, i, j, false) + score;

                                }
                            }
                        }

                        // compare with maxScore    
                        if (score != 0 && score > maxScore) {
                            foundAvailableMove = true;

                            maxScore = score;
                            computerRow = rows;
                            computerCol = cols;
                        }

                    }

                    score = 0;
                }
            }

            if (foundAvailableMove == false) {
                printf("%c player has no valid move.\n", turn);

            } else if (foundAvailableMove == true) {

                // play move with best score and lowest row, col index
                printf("Computer places %c at %c%c.\n", computerColor, (computerRow + 97), (computerCol + 97));

                // update board
                board[computerRow][computerCol] = computerColor;

                // works !
                for (int i = -1; i < 2; i++) {
                    for (int j = 1; j > -2; j--) {
                        if (checkLegalInDirection(board, dimension, computerRow, computerCol, computerColor, j, i)) {
                            int s = flipInDirection(board, dimension, computerRow, computerCol, computerColor, j, i, false);

                            if (s > 0) {
                                flipInDirection(board, dimension, computerRow, computerCol, computerColor, j, i, true);
                            }
                        }
                    }
                }

                // print updated board
                printBoard(board, dimension);

                //reset max score
                maxScore = 0;
                score = 0;
            }

        } else if (turn == userColor) {
            // user plays first
            printf("Enter move for color %c (RowCol): ", userColor);

            // get user rows and columns // works!
            int count = 0;
            do {

                if (getchar() != '!') {
                    userRow = getchar();
                    userCol = getchar();
                }
                count++;

            } while (count == 0);

            userRow = userRow - 97;
            userCol = userCol - 97;

            // check if valid move and change board
            if (moveValid(board, dimension, userColor, userRow, userCol)) {

                // update game board
                board[userRow][userCol] = userColor;


                // pass true for flip and false for not flipping                
                // works!
                for (int i = -1; i < 2; i++) {
                    for (int j = 1; j > -2; j--) {
                        if (checkLegalInDirection(board, dimension, userRow, userCol, userColor, j, i)) {
                            int s = flipInDirection(board, dimension, userRow, userCol, userColor, j, i, false);

                            if (s > 0) {
                                flipInDirection(board, dimension, userRow, userCol, userColor, j, i, true);
                            }
                        }
                    }
                }


                // print updated board
                printBoard(board, dimension);

            } else {
                printf("Invalid move.\n");
                winner = findOpposite(userColor);
                gameOver = true;
            }
        }

        if (!gameOver) {

            // check if board full first
            bool isBoardFull = true;

            for (int rows = 0; rows < dimension; rows++) {
                for (int col = 0; col < dimension; col++) {
                    if (board[rows][col] == 'U') {
                        isBoardFull = false;
                    }
                }
            }

            if (isBoardFull == true) {
                gameOver = true;
                winner = turn;
            } else if (findMoves(board, dimension, findOpposite(turn))) {
                foundAvailableMoveOpposite = true;
            } else if (findMoves(board, dimension, turn)) {
                foundAvailableMove = true;
            }

            if (foundAvailableMoveOpposite) {
                turn = findOpposite(turn);
            } else if (foundAvailableMoveOpposite == false && foundAvailableMove == true) {
                turn = turn;
            } else {
                gameOver = true;
            }

        }

        if (gameOver) {
            printf("%c player wins.", winner);
            printf("\n");
        }

        // set everything to false again;
        foundAvailableMove = false;
        foundAvailableMoveOpposite = true;

    }
    // END WHILE LOOP

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
            }// unoccupied
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

            if (moveValid(board, dimension, colour, rows, cols)) {
                // print move
                printf("%c%c\n", (97 + rows), (97 + cols));
            }

        }
    }

}

bool moveValid(char board[][26], int dimension, char colour, int rows, int cols) {

    if (board[rows][cols] == 'U') {

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
        if (board[newRow][newCol] == findOpposite(colour)) {
            // legal: keep walking till we hit our own color
            newRow = newRow + deltaRow;
            newCol = newCol + deltaCol;

            for (int i = 0; i < dimension && positionInBounds(dimension, newRow, newCol); i++) {

                //newRow = newRow + deltaRow;
                //newCol = newCol + deltaCol;
                if (board[newRow][newCol] == 'U') {
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

    int score = 0;

    // row is userRow, col is userCol, color is userColor
    for (int i = -1; i < 2; i++) {
        for (int j = 1; j > -1; j--) {

            while (checkLegalInDirection(board, dimension, row, col, colour, i, j)) {
                // check if boolean flip is true               
                if (flip) {
                    flipInDirection(board, dimension, row, col, colour, i, j, true);
                } else {
                    score = flipInDirection(board, dimension, row, col, colour, i, j, false) + score;
                }
            }

        }
    }

    return score;
}

int flipInDirection(char board[][26], int dimension, int row, int col, char colour, int deltaRow, int deltaCol, bool flip) {

    int score = 0;
    int newRow = row + deltaRow;
    int newCol = col + deltaCol;

    do {

        if (flip) {
            // only flips tiles if flip is true
            if (board[newRow][newCol] == findOpposite(colour)) {
                board[newRow][newCol] = colour;
            }
        }

        // else, just keeps score
        newRow = newRow + deltaRow;
        newCol = newCol + deltaCol;

        score++;

    } while (board[newRow][newCol] != colour && positionInBounds(dimension, deltaRow, deltaCol));

    return score;
}

bool moveAvailable(char board[][26], int dimension, char colour, int rows, int cols) {

    if (moveValid(board, dimension, colour, rows, cols)) {
        return true;
    }

    return false;
}

char findOpposite(char turn) {

    // returns opposite turn of what's passed
    if (turn == 'W') {
        return 'B';
    } else {
        return 'W';
    }

}

bool anyMoveAvailable(char board[][26], int dimension, char colour) {

    for (int rows = 0; rows < dimension; rows++) {
        for (int cols = 0; cols < dimension; cols++) {

            if (checkValidAndFlip(board, dimension, rows, cols, colour, false) > 0) {
                return true;
            }

        }
    }

    return false;
}

bool findMoves(char board[][26], int dimension, char colour) {

    for (int rows = 0; rows < dimension; rows++) {
        for (int cols = 0; cols < dimension; cols++) {

            if (moveValid(board, dimension, colour, rows, cols)) {
                return true;
            }

        }
    }

    return false;
}