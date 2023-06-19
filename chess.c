#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <ctype.h>
#include "chess.h"

int capturedFlag;

int checkCheck(struct Piece( * board)[boardWidth], int turn,struct enPassantInfo enPassantFlag) {
    int flag = 0;
    struct Piece copyBoard[boardHeight][boardWidth];
    int checkedPlayer = turn;
    memcpy(copyBoard, board, sizeof(struct Piece) * 64);
    struct Piece allPieces[48]; // This will hold all the pieces and their info(ID's colors index)
    int k = 0; // this will count the pieces on the board
    struct Piece King;
    for (int i = 0; i != 8; i++) { // Save the pieces on the board in allPieces[]
        for (int j = 0; j != 8; j++) {
            if (board[i][j].pieceID != pieceSpace) {
                allPieces[k] = board[i][j];
                allPieces[k].position.index1 = board[i][j].position.index1;
                allPieces[k].position.index2 = board[i][j].position.index2;
                k++; //if its a piece count+1
            }
            if (board[i][j].pieceID == pieceKing && board[i][j].pieceColorID == checkedPlayer) { // Save checked player's king
                King = board[i][j];
                King.position.index1 = board[i][j].position.index1;
                King.position.index2 = board[i][j].position.index2;
            }
        }
    }
    for (int i = 0; i < k; i++) {
        if (allPieces[i].pieceColorID == checkedPlayer) { // clear the friendly pieces of checked player
            allPieces[i].pieceID = pieceSpace;
            allPieces[i].pieceColorID = colorSpace;
            strcpy(allPieces[i].pieceName, "0");
        }
    }

    // IF KING IS UNDER THREAT SWITCH CHECK FLAG
    for (int i = 0; i != k; i++) {
            memcpy(board,copyBoard,sizeof(struct Piece)*64);
        switch (allPieces[i].pieceID) {
        case piecePawn: {
            if (pawnPotentialMoves(board, allPieces[i].position.index1, allPieces[i].position.index2,enPassantFlag) != 0) { // If a pawn's potential move/capture is on top of the king switch the check flag.
                for (int x = 0; x != 8; x++) {
                    for (int y = 0; y != 8; y++) {
                        if (board[x][y].pieceID == piecePotentialCapture && x == King.position.index1 && y == King.position.index2) {
                            flag += 1;
                            memcpy(board, copyBoard, sizeof(struct Piece) * 64);
                            break;
                        }
                    }
                }
                break;
            } else {
                memcpy(board, copyBoard, sizeof(struct Piece) * 64);
                break;
            }
        }
        case pieceRook: {
            if (rookPotentialMoves(board, allPieces[i].position.index1, allPieces[i].position.index2) != 0) { // If a rook's potential move/capture is on top of the king switch the check flag.
                for (int x = 0; x != 8; x++) {
                    for (int y = 0; y != 8; y++) {
                        if (board[x][y].pieceID == piecePotentialCapture && x == King.position.index1 && y == King.position.index2) {
                            flag += 1;
                            memcpy(board, copyBoard, sizeof(struct Piece) * 64);
                            break;
                        }
                    }
                }
                break;
            } else {
                memcpy(board, copyBoard, sizeof(struct Piece) * 64);
                break;
            }
        }
        case pieceKnight: {
            if (knightPotentialMoves(board, allPieces[i].position.index1, allPieces[i].position.index2) != 0) { // If a knight's potential move/capture is on top of the king switch the check flag.
                for (int x = 0; x != 8; x++) {
                    for (int y = 0; y != 8; y++) {
                        if (board[x][y].pieceID == piecePotentialCapture && x == King.position.index1 && y == King.position.index2) {
                            flag += 1;
                            memcpy(board, copyBoard, sizeof(struct Piece) * 64);
                            break;
                        }
                    }
                }
                break;
            } else {
                memcpy(board, copyBoard, sizeof(struct Piece) * 64);
                break;
            }
        }
        case pieceBishop: {
            if (bishopPotentialMoves(board, allPieces[i].position.index1, allPieces[i].position.index2) != 0) { // If a bishop's potential move/capture is on top of the king switch the check flag.
                for (int x = 0; x != 8; x++) {
                    for (int y = 0; y != 8; y++) {
                        if (board[x][y].pieceID == piecePotentialCapture && x == King.position.index1 && y == King.position.index2) {
                            flag += 1;
                            memcpy(board, copyBoard, sizeof(struct Piece) * 64);
                            break;
                        }
                    }
                }
                break;
            } else {
                memcpy(board, copyBoard, sizeof(struct Piece) * 64);
                break;
            }
        }
        case pieceQueen: {
            if (queenPotentialMoves(board, allPieces[i].position.index1, allPieces[i].position.index2) != 0) { // If a queen's potential move/capture is on top of the king switch the check flag.
                for (int x = 0; x != 8; x++) {
                    for (int y = 0; y != 8; y++) {
                        if (board[x][y].pieceID == piecePotentialCapture && x == King.position.index1 && y == King.position.index2) {
                            flag += 1;
                            memcpy(board, copyBoard, sizeof(struct Piece) * 64);
                            break;
                        }
                    }
                }
                break;
            } else {
                memcpy(board, copyBoard, sizeof(struct Piece) * 64);
                break;
            }
        }
        }
    }

    memcpy(board, copyBoard, sizeof(struct Piece) * 64);
    if (flag == 0) return 0; //NO CHECK
    else return 1;

}

int evaluateCheck(struct Piece copyBoard[boardHeight][boardWidth], struct Piece( * board)[boardWidth], int pos1, int pos2, int * turn, char * userInputPos,struct enPassantInfo enPassantFlag ) {
    int flag = 1;
    char userInputTar[16];
    switch (board[pos1][pos2].pieceID) {
    case pieceSpace: { // If player chooses an empty square
        printf("YOU CAN'T SELECT AN EMPTY SQUARE\n");
        flag += 1;
        Sleep(1000);
        break;
    }
    case piecePawn: {
        if (board[pos1][pos2].pieceColorID != * turn) { // If player selects a piece of the opponent break the loop and ask input again
            if ( * turn == turnBlack) {
                printf("YOU ARE PLAYING AS BLACK. CHOOSE A BLACK PIECE\n");
                Sleep(1000);
            } else {
                printf("YOU ARE PLAYING AS WHITE. CHOOSE A WHITE PIECE\n");
                Sleep(1000);
            }
            flag += 1;
            break;
        }
        if (pawnPotentialMoves(board, pos1, pos2,enPassantFlag) == 0) {
            printf("Choose a valid piece! ");
            Sleep(1000);
            flag += 1;
            break;
        } else {
            render(board, * turn);
            do {
                printf("Moving %s Type where you want to move: ", userInputPos);
                printPotentialMoves(board);
                fgets(userInputTar, 16, stdin);
                for (int i = 0; i < strlen(userInputTar); i++) {
                    if (userInputTar[i] == '\n') userInputTar[i] = '\0'; //get rid of newline character
                }
                if (strlen(userInputTar) != 2 || userInputTar[1] - '0' < 1 || userInputTar[1] - '0' > 8 || (userInputTar[0] > 'H' && userInputTar[0] < 'a') || userInputTar[0] > 'h') {
                    printf("ENTER A VALID SQUARE!!\n");
                }
            } while (strlen(userInputTar) != 2 || userInputTar[1] - '0' < 1 || userInputTar[1] - '0' > 8 || (userInputTar[0] > 'H' && userInputTar[0] < 'a') || userInputTar[0] > 'h');
            int * convertedTargetPos = convertToArrayIndex(userInputTar);
            if ((movePiece(copyBoard, board, pos1, pos2, convertedTargetPos[0], convertedTargetPos[1])) == 0) {
                if ((checkCheck(board, ( * turn),enPassantFlag)) == 0) {
                    * turn = (( * turn) == turnWhite) ? turnBlack : turnWhite;
                    free(convertedTargetPos);
                    flag = 0;
                    render(board, * turn);
                    break;
                } else {
                    memcpy(board, copyBoard, sizeof(struct Piece) * 64);
                    free(convertedTargetPos);
                    printf("ILLEGAL MOVE!! You are in Check!\n");
                    Sleep(1000);
                    flag += 1;
                    break;
                }
            } else {
                memcpy(board, copyBoard, sizeof(struct Piece) * 64);
                Sleep(1000);
                flag += 1;
                free(convertedTargetPos);
                break;
            }
            break;
        }
    }

    case pieceRook: {
        if (board[pos1][pos2].pieceColorID != * turn) { // If player selects a piece of the opponent break the loop and ask input again
            if ( * turn == turnBlack) {
                printf("YOU ARE PLAYING AS BLACK. CHOOSE A BLACK PIECE\n");
                Sleep(1000);
            } else {
                printf("YOU ARE PLAYING AS WHITE. CHOOSE A WHITE PIECE\n");
                Sleep(1000);
            }
            flag += 1;
            break;
        }
        if (rookPotentialMoves(board, pos1, pos2) == 0) {
            printf("Choose a valid piece! ");
            Sleep(1000);
            flag += 1;
            break;
        } else {
            render(board, * turn);
            do {
                printf("Moving %s Type where you want to move: ", userInputPos);
                printPotentialMoves(board);
                fgets(userInputTar, 16, stdin);
                for (int i = 0; i < strlen(userInputTar); i++) {
                    if (userInputTar[i] == '\n') userInputTar[i] = '\0'; //get rid of newline character
                }
                if (strlen(userInputTar) != 2 || userInputTar[1] - '0' < 1 || userInputTar[1] - '0' > 8 || (userInputTar[0] > 'H' && userInputTar[0] < 'a') || userInputTar[0] > 'h') {
                    printf("ENTER A VALID SQUARE!!\n");
                }
            } while (strlen(userInputTar) != 2 || userInputTar[1] - '0' < 1 || userInputTar[1] - '0' > 8 || (userInputTar[0] > 'H' && userInputTar[0] < 'a') || userInputTar[0] > 'h');
            int * convertedTargetPos = convertToArrayIndex(userInputTar);
            if ((movePiece(copyBoard, board, pos1, pos2, convertedTargetPos[0], convertedTargetPos[1])) == 0) {
                if ((checkCheck(board, ( * turn),enPassantFlag)) == 0) {
                    * turn = (( * turn) == turnWhite) ? turnBlack : turnWhite;
                    free(convertedTargetPos);
                    flag = 0;
                    render(board, * turn);
                    break;
                } else {
                    memcpy(board, copyBoard, sizeof(struct Piece) * 64);
                    free(convertedTargetPos);
                    printf("ILLEGAL MOVE!! You are in Check!\n");
                    Sleep(1000);
                    flag += 1;
                    break;
                }
            } else {
                memcpy(board, copyBoard, sizeof(struct Piece) * 64);
                Sleep(1000);
                flag += 1;
                free(convertedTargetPos);
                break;
            }
            break;
        }
    }

    case pieceKnight: {
        if (board[pos1][pos2].pieceColorID != * turn) { // If player selects a piece of the opponent break the loop and ask input again
            if ( * turn == turnBlack) {
                printf("YOU ARE PLAYING AS BLACK. CHOOSE A BLACK PIECE\n");
                Sleep(1000);
            } else {
                printf("YOU ARE PLAYING AS WHITE. CHOOSE A WHITE PIECE\n");
                Sleep(1000);
            }
            flag += 1;
            break;
        }
        if (knightPotentialMoves(board, pos1, pos2) == 0) {
            printf("Choose a valid piece! ");
            Sleep(1000);
            flag += 1;
            break;
        } else {
            render(board, * turn);
            do {
                printf("Moving %s Type where you want to move: ", userInputPos);
                printPotentialMoves(board);
                fgets(userInputTar, 16, stdin);
                for (int i = 0; i < strlen(userInputTar); i++) {
                    if (userInputTar[i] == '\n') userInputTar[i] = '\0'; //get rid of newline character
                }
                if (strlen(userInputTar) != 2 || userInputTar[1] - '0' < 1 || userInputTar[1] - '0' > 8 || (userInputTar[0] > 'H' && userInputTar[0] < 'a') || userInputTar[0] > 'h') {
                    printf("ENTER A VALID SQUARE!!\n");
                }
            } while (strlen(userInputTar) != 2 || userInputTar[1] - '0' < 1 || userInputTar[1] - '0' > 8 || (userInputTar[0] > 'H' && userInputTar[0] < 'a') || userInputTar[0] > 'h');
            int * convertedTargetPos = convertToArrayIndex(userInputTar);
            if ((movePiece(copyBoard, board, pos1, pos2, convertedTargetPos[0], convertedTargetPos[1])) == 0) {
                if ((checkCheck(board, ( * turn),enPassantFlag)) == 0) {
                    * turn = (( * turn) == turnWhite) ? turnBlack : turnWhite;
                    free(convertedTargetPos);
                    flag = 0;
                    render(board, * turn);
                    break;
                } else {
                    memcpy(board, copyBoard, sizeof(struct Piece) * 64);
                    free(convertedTargetPos);
                    printf("ILLEGAL MOVE!! You are in Check!\n");
                    Sleep(1000);
                    flag += 1;
                    break;
                }
            } else {
                memcpy(board, copyBoard, sizeof(struct Piece) * 64);
                Sleep(1000);
                flag += 1;
                free(convertedTargetPos);
                break;
            }
            break;
        }
    }

    case pieceBishop: {
        if (board[pos1][pos2].pieceColorID != * turn) { // If player selects a piece of the opponent break the loop and ask input again
            if ( * turn == turnBlack) {
                printf("YOU ARE PLAYING AS BLACK. CHOOSE A BLACK PIECE\n");
                Sleep(1000);
            } else {
                printf("YOU ARE PLAYING AS WHITE. CHOOSE A WHITE PIECE\n");
                Sleep(1000);
            }
            flag += 1;
            break;
        }
        if (bishopPotentialMoves(board, pos1, pos2) == 0) {
            printf("Choose a valid piece! ");
            Sleep(1000);
            flag += 1;
            break;
        } else {
            render(board, * turn);
            do {
                printf("Moving %s Type where you want to move: ", userInputPos);
                printPotentialMoves(board);
                fgets(userInputTar, 16, stdin);
                for (int i = 0; i < strlen(userInputTar); i++) {
                    if (userInputTar[i] == '\n') userInputTar[i] = '\0'; //get rid of newline character
                }
                if (strlen(userInputTar) != 2 || userInputTar[1] - '0' < 1 || userInputTar[1] - '0' > 8 || (userInputTar[0] > 'H' && userInputTar[0] < 'a') || userInputTar[0] > 'h') {
                    printf("ENTER A VALID SQUARE!!\n");
                }
            } while (strlen(userInputTar) != 2 || userInputTar[1] - '0' < 1 || userInputTar[1] - '0' > 8 || (userInputTar[0] > 'H' && userInputTar[0] < 'a') || userInputTar[0] > 'h');
            int * convertedTargetPos = convertToArrayIndex(userInputTar);
            if ((movePiece(copyBoard, board, pos1, pos2, convertedTargetPos[0], convertedTargetPos[1])) == 0) {
                if ((checkCheck(board, ( * turn),enPassantFlag)) == 0) {
                    * turn = (( * turn) == turnWhite) ? turnBlack : turnWhite;
                    free(convertedTargetPos);
                    flag = 0;
                    render(board, * turn);
                    break;
                } else {
                    memcpy(board, copyBoard, sizeof(struct Piece) * 64);
                    free(convertedTargetPos);
                    printf("ILLEGAL MOVE!! You are in Check!\n");
                    Sleep(1000);
                    flag += 1;
                    break;
                }
            } else {
                memcpy(board, copyBoard, sizeof(struct Piece) * 64);
                Sleep(1000);
                flag += 1;
                free(convertedTargetPos);
                break;
            }
            break;
        }
    }

    case pieceQueen: {

        if (board[pos1][pos2].pieceColorID != * turn) { // If player selects a piece of the opponent break the loop and ask input again
            if ( * turn == turnBlack) {
                printf("YOU ARE PLAYING AS BLACK. CHOOSE A BLACK PIECE\n");
                Sleep(1000);
            } else {
                printf("YOU ARE PLAYING AS WHITE. CHOOSE A WHITE PIECE\n");
                Sleep(1000);
            }
            flag += 1;
            break;
        }
        if (queenPotentialMoves(board, pos1, pos2) == 0) {
            printf("Choose a valid piece! ");
            Sleep(1000);
            flag += 1;
            break;
        } else {
            render(board, * turn);
            do {
                printf("Moving %s Type where you want to move: ", userInputPos);
                printPotentialMoves(board);
                fgets(userInputTar, 16, stdin);
                for (int i = 0; i < strlen(userInputTar); i++) {
                    if (userInputTar[i] == '\n') userInputTar[i] = '\0'; //get rid of newline character
                }
                if (strlen(userInputTar) != 2 || userInputTar[1] - '0' < 1 || userInputTar[1] - '0' > 8 || (userInputTar[0] > 'H' && userInputTar[0] < 'a') || userInputTar[0] > 'h') {
                    printf("ENTER A VALID SQUARE!!\n");
                }
            } while (strlen(userInputTar) != 2 || userInputTar[1] - '0' < 1 || userInputTar[1] - '0' > 8 || (userInputTar[0] > 'H' && userInputTar[0] < 'a') || userInputTar[0] > 'h');
            int * convertedTargetPos = convertToArrayIndex(userInputTar);
            if ((movePiece(copyBoard, board, pos1, pos2, convertedTargetPos[0], convertedTargetPos[1])) == 0) {
                if ((checkCheck(board, ( * turn),enPassantFlag)) == 0) {
                    * turn = (( * turn) == turnWhite) ? turnBlack : turnWhite;
                    free(convertedTargetPos);
                    flag = 0;
                    render(board, * turn);
                    break;
                } else {
                    memcpy(board, copyBoard, sizeof(struct Piece) * 64);
                    free(convertedTargetPos);
                    printf("ILLEGAL MOVE!! You are in Check!\n");
                    Sleep(1000);
                    flag += 1;
                    break;
                }
            } else {
                memcpy(board, copyBoard, sizeof(struct Piece) * 64);
                Sleep(1000);
                flag += 1;
                free(convertedTargetPos);
                break;
            }
            break;
        }
    }
    case pieceKing: {
        if (board[pos1][pos2].pieceColorID != * turn) { // If player selects a piece of the opponent break the loop and ask input again
            if ( * turn == turnBlack) {
                printf("YOU ARE PLAYING AS BLACK. CHOOSE A BLACK PIECE\n");
                Sleep(1000);
            } else {
                printf("YOU ARE PLAYING AS WHITE. CHOOSE A WHITE PIECE\n");
                Sleep(1000);
            }
            flag += 1;
            break;
        }
        if (kingPotentialMoves(board, pos1, pos2) == 0) {
            printf("Choose a valid piece! ");
            Sleep(1000);
            flag += 1;
            break;
        } else {
            render(board, * turn);
            do {
                printf("Moving %s Type where you want to move: ", userInputPos);
                printPotentialMoves(board);
                fgets(userInputTar, 16, stdin);
                for (int i = 0; i < strlen(userInputTar); i++) {
                    if (userInputTar[i] == '\n') userInputTar[i] = '\0'; //get rid of newline character
                }
                if (strlen(userInputTar) != 2 || userInputTar[1] - '0' < 1 || userInputTar[1] - '0' > 8 || (userInputTar[0] > 'H' && userInputTar[0] < 'a') || userInputTar[0] > 'h') {
                    printf("ENTER A VALID SQUARE!!\n");
                }
            } while (strlen(userInputTar) != 2 || userInputTar[1] - '0' < 1 || userInputTar[1] - '0' > 8 || (userInputTar[0] > 'H' && userInputTar[0] < 'a') || userInputTar[0] > 'h');
            int * convertedTargetPos = convertToArrayIndex(userInputTar);
            if ((movePiece(copyBoard, board, pos1, pos2, convertedTargetPos[0], convertedTargetPos[1])) == 0) {
                if ((checkCheck(board, ( * turn),enPassantFlag)) == 0) {
                    * turn = (( * turn) == turnWhite) ? turnBlack : turnWhite;
                    free(convertedTargetPos);
                    flag = 0;
                    render(board, * turn);
                    break;
                } else {
                    memcpy(board, copyBoard, sizeof(struct Piece) * 64);
                    free(convertedTargetPos);
                    printf("ILLEGAL MOVE!! You are in Check!\n");
                    Sleep(1000);
                    flag += 1;
                    break;
                }
            } else {
                memcpy(board, copyBoard, sizeof(struct Piece) * 64);
                Sleep(1000);
                flag += 1;
                free(convertedTargetPos);
                break;
            }
            break;
        }
    }
    }
    if (flag == 0) return 0;
    else return 1;
}

int isSameColor(struct Piece piece1, struct Piece piece2) { // returns 0 if two pieces have the same color
    if (piece1.pieceColorID == piece2.pieceColorID) {
        return 0;
    } else {
        return 1;
    }
}

int * convertToArrayIndex(char * userInputPos) { // This function converts the piece position like E2 into an index of array like 6 4. (E2 is board[6][4])
    int * buffer = malloc(sizeof(int) * 2); // NEED TO FREE THIS MEMORY EXTERNALLY LATER ON!
    switch (userInputPos[0]) {
    case 'a':
    case 'A':
        buffer[1] = 0;
        break;
    case 'b':
    case 'B':
        buffer[1] = 1;
        break;
    case 'c':
    case 'C':
        buffer[1] = 2;
        break;
    case 'd':
    case 'D':
        buffer[1] = 3;
        break;
    case 'e':
    case 'E':
        buffer[1] = 4;
        break;
    case 'f':
    case 'F':
        buffer[1] = 5;
        break;
    case 'g':
    case 'G':
        buffer[1] = 6;
        break;
    case 'h':
    case 'H':
        buffer[1] = 7;
        break;
    default:
        break;
    }
    switch (userInputPos[1]) {
    case '8':
        buffer[0] = 0;
        break;
    case '7':
        buffer[0] = 1;
        break;
    case '6':
        buffer[0] = 2;
        break;
    case '5':
        buffer[0] = 3;
        break;
    case '4':
        buffer[0] = 4;
        break;
    case '3':
        buffer[0] = 5;
        break;
    case '2':
        buffer[0] = 6;
        break;
    case '1':
        buffer[0] = 7;
        break;
    default:
        break;
    }
    printf("Buffer0 Buffer1 : % d % d", buffer[0], buffer[1]);
    return buffer;
}

void initiateBoard(struct Piece board[boardHeight][boardWidth]) { // Initiate the board
    for (int i = 0; i != 8; i++) {
        for (int j = 0; j != 8; j++) {
            board[i][j].pieceID = pieceSpace; // Setting all the squares to 0
            board[i][j].pieceColorID = colorSpace;
            board[i][j].position.index1 = i;
            board[i][j].position.index2 = j;
            strcpy(board[i][j].pieceName, "0");
        }
    }
    board[0][0].pieceID = pieceRook;
    board[0][0].pieceColorID = colorBlack;
    strcpy(board[0][0].pieceName, "BR");

    board[0][1].pieceID = pieceKnight;
    board[0][1].pieceColorID = colorBlack;
    strcpy(board[0][1].pieceName, "BK");

    board[0][2].pieceID = pieceBishop;
    board[0][2].pieceColorID = colorBlack;
    strcpy(board[0][2].pieceName, "BB");

    board[0][3].pieceID = pieceQueen;
    board[0][3].pieceColorID = colorBlack;
    strcpy(board[0][3].pieceName, "BQ");

    board[0][4].pieceID = pieceKing;
    board[0][4].pieceColorID = colorBlack;
    strcpy(board[0][4].pieceName, "BG");

    board[0][5].pieceID = pieceBishop;
    board[0][5].pieceColorID = colorBlack;
    strcpy(board[0][5].pieceName, "BB");

    board[0][6].pieceID = pieceKnight;
    board[0][6].pieceColorID = colorBlack;
    strcpy(board[0][6].pieceName, "BK");

    board[0][7].pieceID = pieceRook;
    board[0][7].pieceColorID = colorBlack;
    strcpy(board[0][7].pieceName, "BR");

    for (int i = 0; i != 8; i++) { // For loop to create pawns that are in the same row
        board[1][i].pieceID = piecePawn;
        board[1][i].pieceColorID = colorBlack;
        strcpy(board[1][i].pieceName, "BP");
    }

    for (int i = 0; i != 8; i++) { // For loop to create pawns that are in the same row
        board[6][i].pieceID = piecePawn;
        board[6][i].pieceColorID = colorWhite;
        strcpy(board[6][i].pieceName, "WP");
    }
    board[7][0].pieceID = pieceRook;
    board[7][0].pieceColorID = colorWhite;
    strcpy(board[7][0].pieceName, "WR");

    board[7][1].pieceID = pieceKnight;
    board[7][1].pieceColorID = colorWhite;
    strcpy(board[7][1].pieceName, "WK");

    board[7][2].pieceID = pieceBishop;
    board[7][2].pieceColorID = colorWhite;
    strcpy(board[7][2].pieceName, "WB");

    board[7][3].pieceID = pieceQueen;
    board[7][3].pieceColorID = colorWhite;
    strcpy(board[7][3].pieceName, "WQ");

    board[7][4].pieceID = pieceKing;
    board[7][4].pieceColorID = colorWhite;
    strcpy(board[7][4].pieceName, "WG");

    board[7][5].pieceID = pieceBishop;
    board[7][5].pieceColorID = colorWhite;
    strcpy(board[7][5].pieceName, "WB");

    board[7][6].pieceID = pieceKnight;
    board[7][6].pieceColorID = colorWhite;
    strcpy(board[7][6].pieceName, "WK");

    board[7][7].pieceID = pieceRook;
    board[7][7].pieceColorID = colorWhite;
    strcpy(board[7][7].pieceName, "WR");
}

int render(struct Piece board[boardHeight][boardWidth], int turn) { // Render
    system("cls"); // CLEAR CONSOLE SCREEN ON WINDOWS (NOT PORTABLE) -----find a better solution
    printf("Chess Board | P:Pawn | R:Rook | K:Knight | B:Bishop | Q:Queen | G:King |\n");
    for (int i = 0; i != 8; i++) {
        printf("\n%d", 8 - i); // Vertical numbers
        for (int j = 0; j != 8; j++) {
            printf(" %-2s", board[i][j].pieceName); // Printing the board[8][8] array
        }
    }
    printf("\n  A  B  C  D  E  F  G  H\n"); // Horizontal letters
    if (turn == turnBlack)
        printf("Player 2| Black Playing\n");
    else printf("Player 1| White Playing\n");
    return 0;
}

void promotePawn(struct Piece( * board)[boardHeight], int pos1, int pos2) {
    board[pos1][pos2].pieceID = pieceSpace;
    board[pos1][pos2].pieceColorID = colorSpace;

    char promoteID;
    if (pos1 == 7) //Pawn is black and in the bottom row waiting for promotion
    {
        do {
            printf("Pawn is being promoted! Type the name of the piece you want to promote into [R,K,B,Q]: ");
            promoteID = getchar();
            if (promoteID != '\n') {
                // consume rest of chars up to '\n'
                int ch;
                while (((ch = getchar()) != EOF) && (ch != '\n')) /* void */;
            }
            if (promoteID != 'r' && promoteID != 'R' && promoteID != 'k' && promoteID != 'K' && promoteID != 'b' && promoteID != 'B' && promoteID != 'q' && promoteID != 'Q') {
                printf("\nInvalid piece! Accepted values are: R K Q B");
            }
        } while (promoteID != 'r' && promoteID != 'R' && promoteID != 'k' && promoteID != 'K' && promoteID != 'b' && promoteID != 'B' && promoteID != 'q' && promoteID != 'Q');

        switch (promoteID) {
        case 'r':
        case 'R': {
            board[pos1][pos2].pieceColorID = colorBlack;
            board[pos1][pos2].pieceID = pieceRook;
            strcpy(board[pos1][pos2].pieceName, "BR");
            printf("Pawn is promoted to Rook!");
            Sleep(1000);
            break;
        }
        case 'k':
        case 'K': {
            board[pos1][pos2].pieceColorID = colorBlack;
            board[pos1][pos2].pieceID = pieceKnight;
            strcpy(board[pos1][pos2].pieceName, "BK");
            printf("Pawn is promoted to Knight!");
            Sleep(1000);
            break;
        }
        case 'b':
        case 'B': {
            board[pos1][pos2].pieceColorID = colorBlack;
            board[pos1][pos2].pieceID = pieceBishop;
            strcpy(board[pos1][pos2].pieceName, "BB");
            printf("Pawn is promoted to Bishop!");
            Sleep(1000);
            break;
        }
        case 'q':
        case 'Q': {
            board[pos1][pos2].pieceColorID = colorBlack;
            board[pos1][pos2].pieceID = pieceQueen;
            strcpy(board[pos1][pos2].pieceName, "BQ");
            printf("Pawn is promoted to Queen!");
            Sleep(1000);
            break;
        }
        }
    }
    if (pos1 == 0) { //Pawn is white and in the top row waiting for promotion
        do {
            printf("Pawn is being promoted! Type the name of the piece you want to promote into [R,K,B,Q]: ");
            promoteID = getchar();
            if (promoteID != '\n') {
                // consume rest of chars up to '\n'
                int ch;
                while (((ch = getchar()) != EOF) && (ch != '\n')) /* void */;
            }
            if ((promoteID != 'r') && (promoteID != 'R') && (promoteID != 'k') && (promoteID != 'K') && (promoteID != 'b') && (promoteID != 'B') && (promoteID != 'q') && (promoteID != 'Q')) {
                printf("\nInvalid piece! Accepted values are: R K Q B r q k b");
            }
        } while ((promoteID != 'r') && (promoteID != 'R') && (promoteID != 'k') && (promoteID != 'K') && (promoteID != 'b') && (promoteID != 'B') && (promoteID != 'q') && (promoteID != 'Q'));
        switch (promoteID) {
        case 'r':
        case 'R': {
            board[pos1][pos2].pieceColorID = colorWhite;
            board[pos1][pos2].pieceID = pieceRook;
            strcpy(board[pos1][pos2].pieceName, "WR");
            printf("Pawn is promoted to Rook!");
            Sleep(1000);
            break;
        }
        case 'k':
        case 'K': {
            board[pos1][pos2].pieceColorID = colorWhite;
            board[pos1][pos2].pieceID = pieceKnight;
            strcpy(board[pos1][pos2].pieceName, "WK");
            printf("Pawn is promoted to Knight!");
            Sleep(1000);
            break;
        }
        case 'b':
        case 'B': {
            board[pos1][pos2].pieceColorID = colorWhite;
            board[pos1][pos2].pieceID = pieceBishop;
            strcpy(board[pos1][pos2].pieceName, "WB");
            printf("Pawn is promoted to Bishop!");
            Sleep(1000);
            break;
        }
        case 'q':
        case 'Q': {
            board[pos1][pos2].pieceColorID = colorWhite;
            board[pos1][pos2].pieceID = pieceQueen;
            strcpy(board[pos1][pos2].pieceName, "WQ");
            printf("Pawn is promoted to Queen!");
            Sleep(1000);
            break;
        }
        }
    }
}

int movePiece(struct Piece( * copyBoard)[boardWidth], struct Piece( * board)[boardWidth], int piecePos1, int piecePos2, int targetPos1, int targetPos2) { // MOVE A PIECE
    if (board[targetPos1][targetPos2].pieceID == piecePotentialMove || board[targetPos1][targetPos2].pieceID == piecePotentialCapture) {
        if(board[targetPos1][targetPos2].pieceID == piecePotentialCapture) capturedFlag =1; // If the target piece is a capture, switch the flag
        for (int i = 0; i != 8; i++) {
            for (int j = 0; j != 8; j++) {
                // if (board[i][j].pieceID == piecePotentialMove || board[i][j].pieceID == piecePotentialCapture) { // RESTORE THE OLD BOARD PIECES
                board[i][j].pieceID = copyBoard[i][j].pieceID;
                board[i][j].pieceColorID = copyBoard[i][j].pieceColorID;
                strcpy(board[i][j].pieceName, copyBoard[i][j].pieceName);
            }
        }
        board[targetPos1][targetPos2].pieceID = board[piecePos1][piecePos2].pieceID;
        board[targetPos1][targetPos2].pieceColorID = board[piecePos1][piecePos2].pieceColorID;
        strcpy(board[targetPos1][targetPos2].pieceName, board[piecePos1][piecePos2].pieceName);

        board[piecePos1][piecePos2].pieceID = pieceSpace;
        board[piecePos1][piecePos2].pieceColorID = colorSpace;
        strcpy(board[piecePos1][piecePos2].pieceName, "0");
        return 0;
    } else {
        printf("ENTER A VALID MOVE!!!\n");
        return 1;
    }
}

int pawnPotentialMoves(struct Piece( * board)[boardWidth], int posIndex1, int posIndex2,struct enPassantInfo enPassantFlag) { // Marks the squares(locations) that a pawn can move or capture
    char potMove[2] = "+";
    if (board[posIndex1][posIndex2].pieceColorID == colorWhite) { // WHITE PAWNS
        if (posIndex1 != 0 && board[posIndex1 - 1][posIndex2].pieceID == pieceSpace) {
            board[posIndex1 - 1][posIndex2].pieceID = piecePotentialMove;
            strcpy(board[posIndex1 - 1][posIndex2].pieceName, potMove);
            if (posIndex1 == 6 && board[posIndex1 - 2][posIndex2].pieceID == pieceSpace) {
                board[posIndex1 - 2][posIndex2].pieceID = piecePotentialMove;
                strcpy(board[posIndex1 - 2][posIndex2].pieceName, potMove);
            }
        }

        if(enPassantFlag.flag==1){
                        board[enPassantFlag.position.index1][enPassantFlag.position.index2].pieceID = piecePotentialMove;
                        strcpy(board[enPassantFlag.position.index1][enPassantFlag.position.index2].pieceName,potMove);
                        board[enPassantFlag.position.index1+1][enPassantFlag.position.index2].pieceName[0] = tolower(board[enPassantFlag.position.index1+1][enPassantFlag.position.index2].pieceName[0]);
                        board[enPassantFlag.position.index1+1][enPassantFlag.position.index2].pieceName[1] = tolower(board[enPassantFlag.position.index1+1][enPassantFlag.position.index2].pieceName[1]);
                    }
        if (posIndex1 != 0 && posIndex2 != 0 && posIndex2 != 7) { // pawn is not in the last or first column

            if (board[posIndex1 - 1][posIndex2 + 1].pieceID != pieceSpace && (isSameColor(board[posIndex1][posIndex2], board[posIndex1 - 1][posIndex2 + 1]) != 0)) { //potential capture
                board[posIndex1 - 1][posIndex2 + 1].pieceID = piecePotentialCapture;
                board[posIndex1 - 1][posIndex2 + 1].pieceName[0] = tolower(board[posIndex1 - 1][posIndex2 + 1].pieceName[0]);
                board[posIndex1 - 1][posIndex2 + 1].pieceName[1] = tolower(board[posIndex1 - 1][posIndex2 + 1].pieceName[1]);
            }
            if (board[posIndex1 - 1][posIndex2 - 1].pieceID != pieceSpace && (isSameColor(board[posIndex1][posIndex2], board[posIndex1 - 1][posIndex2 - 1])) != 0) {
                board[posIndex1 - 1][posIndex2 - 1].pieceID = piecePotentialCapture;
                board[posIndex1 - 1][posIndex2 - 1].pieceName[0] = tolower(board[posIndex1 - 1][posIndex2 - 1].pieceName[0]);
                board[posIndex1 - 1][posIndex2 - 1].pieceName[1] = tolower(board[posIndex1 - 1][posIndex2 - 1].pieceName[1]);
            }
        }
         else if (posIndex1 != 0 && posIndex2 == 0) { // pawn is in the first column
            if (board[posIndex1 - 1][posIndex2 + 1].pieceID != pieceSpace && (isSameColor(board[posIndex1][posIndex2], board[posIndex1 - 1][posIndex2 + 1])) != 0) {
                board[posIndex1 - 1][posIndex2 + 1].pieceID = piecePotentialCapture;
                board[posIndex1 - 1][posIndex2 + 1].pieceName[0] = tolower(board[posIndex1 - 1][posIndex2 + 1].pieceName[0]);
                board[posIndex1 - 1][posIndex2 + 1].pieceName[1] = tolower(board[posIndex1 - 1][posIndex2 + 1].pieceName[1]);
            }
        } else if (posIndex1 != 0 && posIndex2 == 7) { // pawn is in the last column
            if (board[posIndex1 - 1][posIndex2 - 1].pieceID != pieceSpace && (isSameColor(board[posIndex1][posIndex2], board[posIndex1 - 1][posIndex2 - 1])) != 0) {
                board[posIndex1 - 1][posIndex2 - 1].pieceID = piecePotentialCapture;
                board[posIndex1 - 1][posIndex2 - 1].pieceName[0] = tolower(board[posIndex1 - 1][posIndex2 - 1].pieceName[0]);
                board[posIndex1 - 1][posIndex2 - 1].pieceName[1] = tolower(board[posIndex1 - 1][posIndex2 - 1].pieceName[1]);
            }
        }
}
     else if (board[posIndex1][posIndex2].pieceColorID == colorBlack) { // BLACK PAWNS
        if(enPassantFlag.flag==1){
                        board[enPassantFlag.position.index1][enPassantFlag.position.index2].pieceID = piecePotentialMove;
                        strcpy(board[enPassantFlag.position.index1][enPassantFlag.position.index2].pieceName,potMove);
                        board[enPassantFlag.position.index1-1][enPassantFlag.position.index2].pieceName[0] = tolower(board[enPassantFlag.position.index1-1][enPassantFlag.position.index2].pieceName[0]);
                        board[enPassantFlag.position.index1-1][enPassantFlag.position.index2].pieceName[1] = tolower(board[enPassantFlag.position.index1-1][enPassantFlag.position.index2].pieceName[1]);
                    }
        if (posIndex1 != 7 && board[posIndex1 + 1][posIndex2].pieceID == pieceSpace) {
            board[posIndex1 + 1][posIndex2].pieceID = piecePotentialMove;
            strcpy(board[posIndex1 + 1][posIndex2].pieceName, potMove);
            if (posIndex1 == 1 && board[posIndex1 + 2][posIndex2].pieceID == pieceSpace) {
                board[posIndex1 + 2][posIndex2].pieceID = piecePotentialMove;
                strcpy(board[posIndex1 + 2][posIndex2].pieceName, potMove);
            }
        }
        if (posIndex1 != 7 && posIndex2 != 0 && posIndex2 != 7) {
            if (board[posIndex1 + 1][posIndex2 + 1].pieceID != pieceSpace && (isSameColor(board[posIndex1][posIndex2], board[posIndex1 + 1][posIndex2 + 1])) != 0) {
                board[posIndex1 + 1][posIndex2 + 1].pieceID = piecePotentialCapture;
                board[posIndex1 + 1][posIndex2 + 1].pieceName[0] = tolower(board[posIndex1 + 1][posIndex2 + 1].pieceName[0]);
                board[posIndex1 + 1][posIndex2 + 1].pieceName[1] = tolower(board[posIndex1 + 1][posIndex2 + 1].pieceName[1]);
            }
            if (board[posIndex1 + 1][posIndex2 - 1].pieceID != pieceSpace && (isSameColor(board[posIndex1][posIndex2], board[posIndex1 + 1][posIndex2 - 1])) != 0) {
                board[posIndex1 + 1][posIndex2 - 1].pieceID = piecePotentialCapture;
                board[posIndex1 + 1][posIndex2 - 1].pieceName[0] = tolower(board[posIndex1 + 1][posIndex2 - 1].pieceName[0]);
                board[posIndex1 + 1][posIndex2 - 1].pieceName[1] = tolower(board[posIndex1 + 1][posIndex2 - 1].pieceName[1]);
            }
        } else if (posIndex1 != 0 && posIndex2 == 0) {
            if (board[posIndex1 + 1][posIndex2 + 1].pieceID != pieceSpace && (isSameColor(board[posIndex1][posIndex2], board[posIndex1 + 1][posIndex2 + 1])) != 0) {
                board[posIndex1 + 1][posIndex2 + 1].pieceID = piecePotentialCapture;
                board[posIndex1 + 1][posIndex2 + 1].pieceName[0] = tolower(board[posIndex1 + 1][posIndex2 + 1].pieceName[0]);
                board[posIndex1 + 1][posIndex2 + 1].pieceName[1] = tolower(board[posIndex1 + 1][posIndex2 + 1].pieceName[1]);
            }
        } else if (posIndex1 != 0 && posIndex2 == 7) {
            if (board[posIndex1 + 1][posIndex2 - 1].pieceID != pieceSpace && (isSameColor(board[posIndex1][posIndex2], board[posIndex1 + 1][posIndex2 - 11])) != 0) {
                board[posIndex1 + 1][posIndex2 - 1].pieceID = piecePotentialCapture;
                board[posIndex1 + 1][posIndex2 - 1].pieceName[0] = tolower(board[posIndex1 + 1][posIndex2 - 1].pieceName[0]);
                board[posIndex1 + 1][posIndex2 - 1].pieceName[1] = tolower(board[posIndex1 + 1][posIndex2 - 1].pieceName[1]);
            }
        }
    }
    int flag = 0;
    for (int i = 0; i != 8; i++) {
        for (int j = 0; j != 8; j++) {
            if (board[i][j].pieceID == piecePotentialCapture || board[i][j].pieceID == piecePotentialMove) {
                flag += 1;
            }
        }
    }
    if (flag == 0) {
        return 0; // No moves
    } else return flag; // Move count
}

int rookPotentialMoves(struct Piece( * board)[boardWidth], int posIndex1, int posIndex2) { // Marks the squares(locations) that a rook can move or capture

    const char potMove[2] = "+";
    //Check upwards direction | Rook is not in the most top row
    if (posIndex1 != 0) {
        for (int i = posIndex1; i != 0; i--) {
            if (board[i - 1][posIndex2].pieceID != pieceSpace) {
                if (isSameColor(board[i - 1][posIndex2], board[posIndex1][posIndex2]) == 0) { // If the piece has same color with the rook loop will break and will not mark the location as a potential move.
                    break;
                } else { // If the piece has opponent color it will mark the location as a potential move and break the loop.
                    board[i - 1][posIndex2].pieceID = piecePotentialCapture;
                    board[i - 1][posIndex2].pieceName[0] = tolower(board[i - 1][posIndex2].pieceName[0]);
                    board[i - 1][posIndex2].pieceName[1] = tolower(board[i - 1][posIndex2].pieceName[1]);
                    break;
                }
            } else if (board[i - 1][posIndex2].pieceID == pieceSpace) {
                board[i - 1][posIndex2].pieceID = piecePotentialMove;
                strcpy(board[i - 1][posIndex2].pieceName, potMove);
            }
        }
    }
    //Downwards direction | Rook is not in the most bottom row
    if (posIndex1 != 7) {
        for (int i = posIndex1; i != 7; i++) {
            if (board[i + 1][posIndex2].pieceID != pieceSpace) {
                if (isSameColor(board[i + 1][posIndex2], board[posIndex1][posIndex2]) == 0) { // If the piece has same color with the rook loop will break and will not mark the location as a potential move.
                    break;
                } else {
                    board[i + 1][posIndex2].pieceID = piecePotentialCapture;
                    board[i + 1][posIndex2].pieceName[0] = tolower(board[i + 1][posIndex2].pieceName[0]);
                    board[i + 1][posIndex2].pieceName[1] = tolower(board[i + 1][posIndex2].pieceName[1]);
                    break;
                }
            } else if (board[i + 1][posIndex2].pieceID == pieceSpace) {
                board[i + 1][posIndex2].pieceID = piecePotentialMove;
                strcpy(board[i + 1][posIndex2].pieceName, potMove);
            }
        }
    }
    //Left direction | Rook is not in the most left column
    if (posIndex2 != 0) {
        for (int i = posIndex2; i != 0; i--) {
            if (board[posIndex1][i - 1].pieceID != pieceSpace) {
                if (isSameColor(board[posIndex1][i - 1], board[posIndex1][posIndex2]) == 0) { // If the piece has same color with the rook loop will break and will not mark the location as a potential move.
                    break;
                } else {
                    board[posIndex1][i - 1].pieceID = piecePotentialCapture;
                    board[posIndex1][i - 1].pieceName[0] = tolower(board[posIndex1][i - 1].pieceName[0]);
                    board[posIndex1][i - 1].pieceName[1] = tolower(board[posIndex1][i - 1].pieceName[1]);
                    break;
                }
            } else if (board[posIndex1][i - 1].pieceID == pieceSpace) {
                board[posIndex1][i - 1].pieceID = piecePotentialMove;
                strcpy(board[posIndex1][i - 1].pieceName, potMove);
            }
        }
    }
    //Right direction | Rook is not in the most right column
    if (posIndex2 != 7) {
        for (int i = posIndex2; i != 7; i++) {
            if (board[posIndex1][i + 1].pieceID != pieceSpace) {
                if (isSameColor(board[posIndex1][i + 1], board[posIndex1][posIndex2]) == 0) { // If the piece has same color with the rook loop will break and will not mark the location as a potential move.
                    break;
                } else {
                    board[posIndex1][i + 1].pieceID = piecePotentialCapture;
                    board[posIndex1][i + 1].pieceName[0] = tolower(board[posIndex1][i + 1].pieceName[0]);
                    board[posIndex1][i + 1].pieceName[1] = tolower(board[posIndex1][i + 1].pieceName[1]);
                    break;
                }
            } else if (board[posIndex1][i + 1].pieceID == pieceSpace) {
                board[posIndex1][i + 1].pieceID = piecePotentialMove;
                strcpy(board[posIndex1][i + 1].pieceName, potMove);
            }
        }
    }
    // check if there are any potential squares that the rook can move and return error value 1 if there is none
    int flag = 0;
    for (int i = 0; i != 8; i++) {
        for (int j = 0; j != 8; j++) {
            if (board[i][j].pieceID == piecePotentialCapture || board[i][j].pieceID == piecePotentialMove) {
                flag += 1;
            }
        }
    }
    if (flag == 0) {
        return 0; // No moves
    } else return flag; // Move count
}

int knightPotentialMoves(struct Piece( * board)[boardHeight], int posIndex1, int posIndex2) { // Marks the squares(locations) that a knight can move or capture
    const char potMove[2] = "+";
    int index1Minus1 = posIndex1 - 1;
    int index1Minus2 = posIndex1 - 2;
    int index1Plus1 = posIndex1 + 1;
    int index1Plus2 = posIndex1 + 2;

    int index2Minus1 = posIndex2 - 1;
    int index2Minus2 = posIndex2 - 2;
    int index2Plus1 = posIndex2 + 1;
    int index2Plus2 = posIndex2 + 2;

    if (index1Minus1 >= 0 && index2Minus2 >= 0) {
        if (board[posIndex1 - 1][posIndex2 - 2].pieceID != pieceSpace) { // board[i-1][j-2]
            if (isSameColor(board[posIndex1 - 1][posIndex2 - 2], board[posIndex1][posIndex2]) != 0) {
                board[posIndex1 - 1][posIndex2 - 2].pieceID = piecePotentialCapture;
                board[posIndex1 - 1][posIndex2 - 2].pieceName[0] = tolower(board[posIndex1 - 1][posIndex2 - 2].pieceName[0]);
                board[posIndex1 - 1][posIndex2 - 2].pieceName[1] = tolower(board[posIndex1 - 1][posIndex2 - 2].pieceName[1]);
            }
        } else if (board[posIndex1 - 1][posIndex2 - 2].pieceID == pieceSpace) {
            board[posIndex1 - 1][posIndex2 - 2].pieceID = piecePotentialMove;
            strcpy(board[posIndex1 - 1][posIndex2 - 2].pieceName, potMove);
        }
    }

    if (index1Minus1 >= 0 && index2Plus2 <= 7) {
        if (board[posIndex1 - 1][posIndex2 + 2].pieceID != pieceSpace) { // board[i-1][j+2]
            if (isSameColor(board[posIndex1 - 1][posIndex2 + 2], board[posIndex1][posIndex2]) != 0) {
                board[posIndex1 - 1][posIndex2 + 2].pieceID = piecePotentialCapture;
                board[posIndex1 - 1][posIndex2 + 2].pieceName[0] = tolower(board[posIndex1 - 1][posIndex2 + 2].pieceName[0]);
                board[posIndex1 - 1][posIndex2 + 2].pieceName[1] = tolower(board[posIndex1 - 1][posIndex2 + 2].pieceName[1]);
            }
        } else if (board[posIndex1 - 1][posIndex2 + 2].pieceID == pieceSpace) {
            board[posIndex1 - 1][posIndex2 + 2].pieceID = piecePotentialMove;
            strcpy(board[posIndex1 - 1][posIndex2 + 2].pieceName, potMove);
        }
    }

    if (index1Minus2 >= 0 && index2Minus1 >= 0) {
        if (board[posIndex1 - 2][posIndex2 - 1].pieceID != pieceSpace) { // board[i-2][j-1]
            if (isSameColor(board[posIndex1 - 2][posIndex2 - 1], board[posIndex1][posIndex2]) != 0) {
                board[posIndex1 - 2][posIndex2 - 1].pieceID = piecePotentialCapture;
                board[posIndex1 - 2][posIndex2 - 1].pieceName[0] = tolower(board[posIndex1 - 2][posIndex2 - 1].pieceName[0]);
                board[posIndex1 - 2][posIndex2 - 1].pieceName[1] = tolower(board[posIndex1 - 2][posIndex2 - 1].pieceName[1]);
            }
        } else if (board[posIndex1 - 2][posIndex2 - 1].pieceID == pieceSpace) {
            board[posIndex1 - 2][posIndex2 - 1].pieceID = piecePotentialMove;
            strcpy(board[posIndex1 - 2][posIndex2 - 1].pieceName, potMove);
        }
    }

    if (index1Minus2 >= 0 && index2Plus1 <= 7) {
        if (board[posIndex1 - 2][posIndex2 + 1].pieceID != pieceSpace) { // board[i-2][j+1]
            if (isSameColor(board[posIndex1 - 2][posIndex2 + 1], board[posIndex1][posIndex2]) != 0) {
                board[posIndex1 - 2][posIndex2 + 1].pieceID = piecePotentialCapture;
                board[posIndex1 - 2][posIndex2 + 1].pieceName[0] = tolower(board[posIndex1 - 2][posIndex2 + 1].pieceName[0]);
                board[posIndex1 - 2][posIndex2 + 1].pieceName[1] = tolower(board[posIndex1 - 2][posIndex2 + 1].pieceName[1]);
            }
        } else if (board[posIndex1 - 2][posIndex2 + 1].pieceID == pieceSpace) {
            board[posIndex1 - 2][posIndex2 + 1].pieceID = piecePotentialMove;
            strcpy(board[posIndex1 - 2][posIndex2 + 1].pieceName, potMove);
        }
    }

    if (index1Plus1 <= 7 && index2Minus2 >= 0) {
        if (board[posIndex1 + 1][posIndex2 - 2].pieceID != pieceSpace) { // board[i+1][j-2]
            if (isSameColor(board[posIndex1 + 1][posIndex2 - 2], board[posIndex1][posIndex2]) != 0) {
                board[posIndex1 + 1][posIndex2 - 2].pieceID = piecePotentialCapture;
                board[posIndex1 + 1][posIndex2 - 2].pieceName[0] = tolower(board[posIndex1 + 1][posIndex2 - 2].pieceName[0]);
                board[posIndex1 + 1][posIndex2 - 2].pieceName[1] = tolower(board[posIndex1 + 1][posIndex2 - 2].pieceName[1]);
            }
        } else if (board[posIndex1 + 1][posIndex2 - 2].pieceID == pieceSpace) {
            board[posIndex1 + 1][posIndex2 - 2].pieceID = piecePotentialMove;
            strcpy(board[posIndex1 + 1][posIndex2 - 2].pieceName, potMove);
        }
    }

    if (index1Plus1 <= 7 && index2Plus2 <= 7) {
        if (board[posIndex1 + 1][posIndex2 + 2].pieceID != pieceSpace) { // board[i+1][j+2]
            if (isSameColor(board[posIndex1 + 1][posIndex2 + 2], board[posIndex1][posIndex2]) != 0) {
                board[posIndex1 + 1][posIndex2 + 2].pieceID = piecePotentialCapture;
                board[posIndex1 + 1][posIndex2 + 2].pieceName[0] = tolower(board[posIndex1 + 1][posIndex2 + 2].pieceName[0]);
                board[posIndex1 + 1][posIndex2 + 2].pieceName[1] = tolower(board[posIndex1 + 1][posIndex2 + 2].pieceName[1]);
            }
        } else if (board[posIndex1 + 1][posIndex2 + 2].pieceID == pieceSpace) {
            board[posIndex1 + 1][posIndex2 + 2].pieceID = piecePotentialMove;
            strcpy(board[posIndex1 + 1][posIndex2 + 2].pieceName, potMove);
        }
    }

    if (index1Plus2 <= 7 && index2Minus1 >= 0) {
        if (board[posIndex1 + 2][posIndex2 - 1].pieceID != pieceSpace) { // board[i+2][j-1]
            if (isSameColor(board[posIndex1 + 2][posIndex2 - 1], board[posIndex1][posIndex2]) != 0) {
                board[posIndex1 + 2][posIndex2 - 1].pieceID = piecePotentialCapture;
                board[posIndex1 + 2][posIndex2 - 1].pieceName[0] = tolower(board[posIndex1 + 2][posIndex2 - 1].pieceName[0]);
                board[posIndex1 + 2][posIndex2 - 1].pieceName[1] = tolower(board[posIndex1 + 2][posIndex2 - 1].pieceName[1]);
            }
        } else if (board[posIndex1 + 2][posIndex2 - 1].pieceID == pieceSpace) {
            board[posIndex1 + 2][posIndex2 - 1].pieceID = piecePotentialMove;
            strcpy(board[posIndex1 + 2][posIndex2 - 1].pieceName, potMove);
        }
    }

    if (index1Plus2 <= 7 && index2Plus1 <= 7) {
        if (board[posIndex1 + 2][posIndex2 + 1].pieceID != pieceSpace) { // board[i+2][j+1]
            if (isSameColor(board[posIndex1 + 2][posIndex2 + 1], board[posIndex1][posIndex2]) != 0) {
                board[posIndex1 + 2][posIndex2 + 1].pieceID = piecePotentialCapture;
                board[posIndex1 + 2][posIndex2 + 1].pieceName[0] = tolower(board[posIndex1 + 2][posIndex2 + 1].pieceName[0]);
                board[posIndex1 + 2][posIndex2 + 1].pieceName[1] = tolower(board[posIndex1 + 2][posIndex2 + 1].pieceName[1]);
            }
        } else if (board[posIndex1 + 2][posIndex2 + 1].pieceID == pieceSpace) {
            board[posIndex1 + 2][posIndex2 + 1].pieceID = piecePotentialMove;
            strcpy(board[posIndex1 + 2][posIndex2 + 1].pieceName, potMove);
        }
    }

    // check if there are any potential move or capture square after calculations
    int flag = 0;
    for (int i = 0; i != 8; i++) {
        for (int j = 0; j != 8; j++) {
            if (board[i][j].pieceID == piecePotentialCapture || board[i][j].pieceID == piecePotentialMove) {
                flag += 1;
            }
        }
    }
    if (flag == 0) {
        return 0; // No moves
    } else return flag; // Move count
}

int bishopPotentialMoves(struct Piece( * board)[boardHeight], int posIndex1, int posIndex2) { // Marks the squares(locations) that a bishop can move or capture
    const char potMove[2] = "+";
    int i, j;
    // Top left direction
    for (i = posIndex1, j = posIndex2; i != 0 && j != 0; --i, --j) {
        if (board[i - 1][j - 1].pieceID != pieceSpace) {
            if (isSameColor(board[i - 1][j - 1], board[posIndex1][posIndex2]) == 0) {
                break;
            } else {
                board[i - 1][j - 1].pieceID = piecePotentialCapture;
                board[i - 1][j - 1].pieceName[0] = tolower(board[i - 1][j - 1].pieceName[0]);
                board[i - 1][j - 1].pieceName[1] = tolower(board[i - 1][j - 1].pieceName[1]);
                break;
            }
        } else if (board[i - 1][j - 1].pieceID == pieceSpace) {
            board[i - 1][j - 1].pieceID = piecePotentialMove;
            strcpy(board[i - 1][j - 1].pieceName, potMove);
        }
    }
    //Top right direction
    for (i = posIndex1, j = posIndex2; i != 0 && j != 7; --i, ++j) {
        if (board[i - 1][j + 1].pieceID != pieceSpace) {
            if (isSameColor(board[i - 1][j + 1], board[posIndex1][posIndex2]) == 0) {
                break;
            } else {
                board[i - 1][j + 1].pieceID = piecePotentialCapture;
                board[i - 1][j + 1].pieceName[0] = tolower(board[i - 1][j + 1].pieceName[0]);
                board[i - 1][j + 1].pieceName[1] = tolower(board[i - 1][j + 1].pieceName[1]);
                break;
            }
        } else if (board[i - 1][j + 1].pieceID == pieceSpace) {
            board[i - 1][j + 1].pieceID = piecePotentialMove;
            strcpy(board[i - 1][j + 1].pieceName, potMove);
        }
    }

    //Bottom left direction
    for (i = posIndex1, j = posIndex2; i != 7 && j != 0; ++i, --j) {
        if (board[i + 1][j - 1].pieceID != pieceSpace) {
            if (isSameColor(board[i + 1][j - 1], board[posIndex1][posIndex2]) == 0) {
                break;
            } else {
                board[i + 1][j - 1].pieceID = piecePotentialCapture;
                board[i + 1][j - 1].pieceName[0] = tolower(board[i + 1][j - 1].pieceName[0]);
                board[i + 1][j - 1].pieceName[1] = tolower(board[i + 1][j - 1].pieceName[1]);
                break;
            }
        } else if (board[i + 1][j - 1].pieceID == pieceSpace) {
            board[i + 1][j - 1].pieceID = piecePotentialMove;
            strcpy(board[i + 1][j - 1].pieceName, potMove);
        }
    }

    //Bottom right direction
    for (i = posIndex1, j = posIndex2; i != 7 && j != 7; ++i, ++j) {
        if (board[i + 1][j + 1].pieceID != pieceSpace) {
            if (isSameColor(board[i + 1][j + 1], board[posIndex1][posIndex2]) == 0) {
                break;
            } else {
                board[i + 1][j + 1].pieceID = piecePotentialCapture;
                board[i + 1][j + 1].pieceName[0] = tolower(board[i + 1][j + 1].pieceName[0]);
                board[i + 1][j + 1].pieceName[1] = tolower(board[i + 1][j + 1].pieceName[1]);
                break;
            }
        } else if (board[i + 1][j + 1].pieceID == pieceSpace) {
            board[i + 1][j + 1].pieceID = piecePotentialMove;
            strcpy(board[i + 1][j + 1].pieceName, potMove);
        }
    }

    // check if there are any potential move or capture square after calculations. if theres not function will return error value 1
    int flag = 0;
    for (int k = 0; k != 8; k++) {
        for (int l = 0; l != 8; l++) {
            if (board[k][l].pieceID == piecePotentialCapture || board[k][l].pieceID == piecePotentialMove) {
                flag += 1;
            }
        }
    }
    if (flag == 0) {
        return 0; // No moves
    } else return flag; // Move count
}

int queenPotentialMoves(struct Piece( * board)[boardHeight], int posIndex1, int posIndex2) { // Marks the squares(locations) that a queen can move or capture
    // Queen has the potential moves of both a rook and a bishop. Combine both
    const char potMove[2] = "+";
    // Bishop:
    // Top left direction
    int i, j;
    // Top left direction
    for (i = posIndex1, j = posIndex2; i != 0 && j != 0; --i, --j) {
        if (board[i - 1][j - 1].pieceID != pieceSpace) {
            if (isSameColor(board[i - 1][j - 1], board[posIndex1][posIndex2]) == 0) {
                break;
            } else {
                board[i - 1][j - 1].pieceID = piecePotentialCapture;
                board[i - 1][j - 1].pieceName[0] = tolower(board[i - 1][j - 1].pieceName[0]);
                board[i - 1][j - 1].pieceName[1] = tolower(board[i - 1][j - 1].pieceName[1]);
                break;
            }
        } else if (board[i - 1][j - 1].pieceID == pieceSpace) {
            board[i - 1][j - 1].pieceID = piecePotentialMove;
            strcpy(board[i - 1][j - 1].pieceName, potMove);
        }
    }
    //Top right direction
    for (i = posIndex1, j = posIndex2; i != 0 && j != 7; --i, ++j) {
        if (board[i - 1][j + 1].pieceID != pieceSpace) {
            if (isSameColor(board[i - 1][j + 1], board[posIndex1][posIndex2]) == 0) {
                break;
            } else {
                board[i - 1][j + 1].pieceID = piecePotentialCapture;
                board[i - 1][j + 1].pieceName[0] = tolower(board[i - 1][j + 1].pieceName[0]);
                board[i - 1][j + 1].pieceName[1] = tolower(board[i - 1][j + 1].pieceName[1]);
                break;
            }
        } else if (board[i - 1][j + 1].pieceID == pieceSpace) {
            board[i - 1][j + 1].pieceID = piecePotentialMove;
            strcpy(board[i - 1][j + 1].pieceName, potMove);
        }
    }

    //Bottom left direction
    for (i = posIndex1, j = posIndex2; i != 7 && j != 0; ++i, --j) {
        if (board[i + 1][j - 1].pieceID != pieceSpace) {
            if (isSameColor(board[i + 1][j - 1], board[posIndex1][posIndex2]) == 0) {
                break;
            } else {
                board[i + 1][j - 1].pieceID = piecePotentialCapture;
                board[i + 1][j - 1].pieceName[0] = tolower(board[i + 1][j - 1].pieceName[0]);
                board[i + 1][j - 1].pieceName[1] = tolower(board[i + 1][j - 1].pieceName[1]);
                break;
            }
        } else if (board[i + 1][j - 1].pieceID == pieceSpace) {
            board[i + 1][j - 1].pieceID = piecePotentialMove;
            strcpy(board[i + 1][j - 1].pieceName, potMove);
        }
    }

    //Bottom right direction
    for (i = posIndex1, j = posIndex2; i != 7 && j != 7; ++i, ++j) {
        if (board[i + 1][j + 1].pieceID != pieceSpace) {
            if (isSameColor(board[i + 1][j + 1], board[posIndex1][posIndex2]) == 0) {
                break;
            } else {
                board[i + 1][j + 1].pieceID = piecePotentialCapture;
                board[i + 1][j + 1].pieceName[0] = tolower(board[i + 1][j + 1].pieceName[0]);
                board[i + 1][j + 1].pieceName[1] = tolower(board[i + 1][j + 1].pieceName[1]);
                break;
            }
        } else if (board[i + 1][j + 1].pieceID == pieceSpace) {
            board[i + 1][j + 1].pieceID = piecePotentialMove;
            strcpy(board[i + 1][j + 1].pieceName, potMove);
        }
    }

    //Rook:
    //Check upwards direction | Rook is not in the most top row
    if (posIndex1 != 0) {
        for (int i = posIndex1; i != 0; i--) {
            if (board[i - 1][posIndex2].pieceID != pieceSpace) {
                if (isSameColor(board[i - 1][posIndex2], board[posIndex1][posIndex2]) == 0) { // If the piece has same color with the rook loop will break and will not mark the location as a potential move.
                    break;
                } else { // If the piece has opponent color it will mark the location as a potential move and break the loop.
                    board[i - 1][posIndex2].pieceID = piecePotentialCapture;
                    board[i - 1][posIndex2].pieceName[0] = tolower(board[i - 1][posIndex2].pieceName[0]);
                    board[i - 1][posIndex2].pieceName[1] = tolower(board[i - 1][posIndex2].pieceName[1]);
                    break;
                }
            } else if (board[i - 1][posIndex2].pieceID == pieceSpace) {
                board[i - 1][posIndex2].pieceID = piecePotentialMove;
                strcpy(board[i - 1][posIndex2].pieceName, potMove);
            }
        }
    }
    //Downwards direction | Rook is not in the most bottom row
    if (posIndex1 != 7) {
        for (int i = posIndex1; i != 7; i++) {
            if (board[i + 1][posIndex2].pieceID != pieceSpace) {
                if (isSameColor(board[i + 1][posIndex2], board[posIndex1][posIndex2]) == 0) { // If the piece has same color with the rook loop will break and will not mark the location as a potential move.
                    break;
                } else {
                    board[i + 1][posIndex2].pieceID = piecePotentialCapture;
                    board[i + 1][posIndex2].pieceName[0] = tolower(board[i + 1][posIndex2].pieceName[0]);
                    board[i + 1][posIndex2].pieceName[1] = tolower(board[i + 1][posIndex2].pieceName[1]);
                    break;
                }
            } else if (board[i + 1][posIndex2].pieceID == pieceSpace) {
                board[i + 1][posIndex2].pieceID = piecePotentialMove;
                strcpy(board[i + 1][posIndex2].pieceName, potMove);
            }
        }
    }
    //Left direction | Rook is not in the most left column
    if (posIndex2 != 0) {
        for (int i = posIndex2; i != 0; i--) {
            if (board[posIndex1][i - 1].pieceID != pieceSpace) {
                if (isSameColor(board[posIndex1][i - 1], board[posIndex1][posIndex2]) == 0) { // If the piece has same color with the rook loop will break and will not mark the location as a potential move.
                    break;
                } else {
                    board[posIndex1][i - 1].pieceID = piecePotentialCapture;
                    board[posIndex1][i - 1].pieceName[0] = tolower(board[posIndex1][i - 1].pieceName[0]);
                    board[posIndex1][i - 1].pieceName[1] = tolower(board[posIndex1][i - 1].pieceName[1]);
                    break;
                }
            } else if (board[posIndex1][i - 1].pieceID == pieceSpace) {
                board[posIndex1][i - 1].pieceID = piecePotentialMove;
                strcpy(board[posIndex1][i - 1].pieceName, potMove);
            }
        }
    }
    //Right direction | Rook is not in the most right column
    if (posIndex2 != 7) {
        for (int i = posIndex2; i != 7; i++) {
            if (board[posIndex1][i + 1].pieceID != pieceSpace) {
                if (isSameColor(board[posIndex1][i + 1], board[posIndex1][posIndex2]) == 0) { // If the piece has same color with the rook loop will break and will not mark the location as a potential move.
                    break;
                } else {
                    board[posIndex1][i + 1].pieceID = piecePotentialCapture;
                    board[posIndex1][i + 1].pieceName[0] = tolower(board[posIndex1][i + 1].pieceName[0]);
                    board[posIndex1][i + 1].pieceName[1] = tolower(board[posIndex1][i + 1].pieceName[1]);
                    break;
                }
            } else if (board[posIndex1][i + 1].pieceID == pieceSpace) {
                board[posIndex1][i + 1].pieceID = piecePotentialMove;
                strcpy(board[posIndex1][i + 1].pieceName, potMove);
            }
        }
    }
    // Check if there are any potential moves after calculations. If not return error value 1
    int flag = 0;
    for (int i = 0; i != 8; i++) {
        for (int j = 0; j != 8; j++) {
            if (board[i][j].pieceID == piecePotentialCapture || board[i][j].pieceID == piecePotentialMove) {
                flag += 1;
            }
        }
    }
    if (flag == 0) {
        return 0; // No moves
    } else return flag; // Move count
}

int kingPotentialMoves(struct Piece( * board)[boardHeight], int posIndex1, int posIndex2) {
    const char potMove[2] = "+";
    // King moves:
    if (posIndex1 != 0) { // KING IS NOT IN THE TOP MOST ROW
        if (board[posIndex1 - 1][posIndex2].pieceID != pieceSpace) {
            if (isSameColor(board[posIndex1 - 1][posIndex2], board[posIndex1][posIndex2]) != 0) {
                board[posIndex1 - 1][posIndex2].pieceID = piecePotentialCapture;
                board[posIndex1 - 1][posIndex2].pieceName[0] = tolower(board[posIndex1 - 1][posIndex2].pieceName[0]);
                board[posIndex1 - 1][posIndex2].pieceName[1] = tolower(board[posIndex1 - 1][posIndex2].pieceName[1]);
            }
        } else if (board[posIndex1 - 1][posIndex2].pieceID == pieceSpace) {
            board[posIndex1 - 1][posIndex2].pieceID = piecePotentialMove;
            strcpy(board[posIndex1 - 1][posIndex2].pieceName, potMove);
        }
        // upwards corners
        if (posIndex2 != 0) {
            if (board[posIndex1 - 1][posIndex2 - 1].pieceID != pieceSpace) {
                if (isSameColor(board[posIndex1 - 1][posIndex2 - 1], board[posIndex1][posIndex2]) != 0) {
                    board[posIndex1 - 1][posIndex2 - 1].pieceID = piecePotentialCapture;
                    board[posIndex1 - 1][posIndex2 - 1].pieceName[0] = tolower(board[posIndex1 - 1][posIndex2 - 1].pieceName[0]);
                    board[posIndex1 - 1][posIndex2 - 1].pieceName[1] = tolower(board[posIndex1 - 1][posIndex2 - 1].pieceName[1]);

                }
            } else if (board[posIndex1 - 1][posIndex2 - 1].pieceID == pieceSpace) {
                board[posIndex1 - 1][posIndex2 - 1].pieceID = piecePotentialMove;
                strcpy(board[posIndex1 - 1][posIndex2 - 1].pieceName, potMove);
            }
                    if(board[posIndex1][posIndex2-1].pieceID != pieceSpace){
                        if(isSameColor(board[posIndex1][posIndex2-1],board[posIndex1][posIndex2])!=0){
                            board[posIndex1][posIndex2-1].pieceID = piecePotentialCapture;
                            board[posIndex1][posIndex2-1].pieceName[0] = tolower(board[posIndex1][posIndex2-1].pieceName[0]);
                            board[posIndex1][posIndex2-1].pieceName[1] = tolower(board[posIndex1][posIndex2-1].pieceName[1]);
                        }
                    }
                    else if(board[posIndex1][posIndex2-1].pieceID == pieceSpace){
                        board[posIndex1][posIndex2-1].pieceID = piecePotentialMove;
                        strcpy(board[posIndex1][posIndex2-1].pieceName,potMove);
                    }
        }
        if (posIndex2 != 7) {
            if (board[posIndex1 - 1][posIndex2 + 1].pieceID != pieceSpace) {
                if (isSameColor(board[posIndex1 - 1][posIndex2 + 1], board[posIndex1][posIndex2]) != 0) {
                    board[posIndex1 - 1][posIndex2 + 1].pieceID = piecePotentialCapture;
                    board[posIndex1 - 1][posIndex2 + 1].pieceName[0] = tolower(board[posIndex1 - 1][posIndex2 + 1].pieceName[0]);
                    board[posIndex1 - 1][posIndex2 + 1].pieceName[1] = tolower(board[posIndex1 - 1][posIndex2 + 1].pieceName[1]);
                }
            } else if (board[posIndex1 - 1][posIndex2 + 1].pieceID == pieceSpace) {
                board[posIndex1 - 1][posIndex2 + 1].pieceID = piecePotentialMove;
                strcpy(board[posIndex1 - 1][posIndex2 + 1].pieceName, potMove);
            }
            if(board[posIndex1][posIndex2+1].pieceID != pieceSpace){
                        if(isSameColor(board[posIndex1][posIndex2+1],board[posIndex1][posIndex2])!=0){
                            board[posIndex1][posIndex2+1].pieceID = piecePotentialCapture;
                            board[posIndex1][posIndex2+1].pieceName[0] = tolower(board[posIndex1][posIndex2+1].pieceName[0]);
                            board[posIndex1][posIndex2+1].pieceName[1] = tolower(board[posIndex1][posIndex2+1].pieceName[1]);
                        }
                    }
                    else if(board[posIndex1][posIndex2+1].pieceID == pieceSpace){
                        board[posIndex1][posIndex2+1].pieceID = piecePotentialMove;
                        strcpy(board[posIndex1][posIndex2+1].pieceName,potMove);
                }
        }
}

    if (posIndex1 != 7) {  // KING IS NOT IN THE BOTTOM ROW
        if (board[posIndex1 + 1][posIndex2].pieceID != pieceSpace) {
            if (isSameColor(board[posIndex1 + 1][posIndex2], board[posIndex1][posIndex2]) != 0) {
                board[posIndex1 + 1][posIndex2].pieceID = piecePotentialCapture;
                board[posIndex1 + 1][posIndex2].pieceName[0] = tolower(board[posIndex1 + 1][posIndex2].pieceName[0]);
                board[posIndex1 + 1][posIndex2].pieceName[1] = tolower(board[posIndex1 + 1][posIndex2].pieceName[1]);

            }
        } else if (board[posIndex1 + 1][posIndex2].pieceID == pieceSpace) {
            board[posIndex1 + 1][posIndex2].pieceID = piecePotentialMove;
            strcpy(board[posIndex1 + 1][posIndex2].pieceName, potMove);
        }
        //downwards corners
        if (posIndex2 != 0) {
            if (board[posIndex1 + 1][posIndex2 - 1].pieceID != pieceSpace) {
                if (isSameColor(board[posIndex1 + 1][posIndex2 - 1], board[posIndex1][posIndex2]) != 0) {
                    board[posIndex1 + 1][posIndex2 - 1].pieceID = piecePotentialCapture;
                    board[posIndex1 + 1][posIndex2 - 1].pieceName[0] = tolower(board[posIndex1 + 1][posIndex2 - 1].pieceName[0]);
                    board[posIndex1 + 1][posIndex2 - 1].pieceName[1] = tolower(board[posIndex1 + 1][posIndex2 - 1].pieceName[1]);

                }
            } else if (board[posIndex1 + 1][posIndex2 - 1].pieceID == pieceSpace) {
                board[posIndex1 + 1][posIndex2 - 1].pieceID = piecePotentialMove;
                strcpy(board[posIndex1 + 1][posIndex2 - 1].pieceName, potMove);
            }
            if(board[posIndex1][posIndex2-1].pieceID != pieceSpace){
                        if(isSameColor(board[posIndex1][posIndex2-1],board[posIndex1][posIndex2])!=0){
                            board[posIndex1][posIndex2-1].pieceID = piecePotentialCapture;
                            board[posIndex1][posIndex2-1].pieceName[0] = tolower(board[posIndex1][posIndex2-1].pieceName[0]);
                            board[posIndex1][posIndex2-1].pieceName[1] = tolower(board[posIndex1][posIndex2-1].pieceName[1]);
                        }
                    }
                    else if(board[posIndex1][posIndex2-1].pieceID == pieceSpace){
                        board[posIndex1][posIndex2-1].pieceID = piecePotentialMove;
                        strcpy(board[posIndex1][posIndex2-1].pieceName,potMove);
                    }
        }
        if (posIndex2 != 7) {
            if (board[posIndex1 + 1][posIndex2 + 1].pieceID != pieceSpace) {
                if (isSameColor(board[posIndex1 + 1][posIndex2 + 1], board[posIndex1][posIndex2]) != 0) {
                    board[posIndex1 + 1][posIndex2 + 1].pieceID = piecePotentialCapture;
                    board[posIndex1 + 1][posIndex2 + 1].pieceName[0] = tolower(board[posIndex1 + 1][posIndex2 + 1].pieceName[0]);
                    board[posIndex1 + 1][posIndex2 + 1].pieceName[1] = tolower(board[posIndex1 + 1][posIndex2 + 1].pieceName[1]);

                }
            } else if (board[posIndex1 + 1][posIndex2 + 1].pieceID == pieceSpace) {
                board[posIndex1 + 1][posIndex2 + 1].pieceID = piecePotentialMove;
                strcpy(board[posIndex1 + 1][posIndex2 + 1].pieceName, potMove);
            }
            if(board[posIndex1][posIndex2+1].pieceID != pieceSpace){
                        if(isSameColor(board[posIndex1][posIndex2+1],board[posIndex1][posIndex2])!=0){
                            board[posIndex1][posIndex2+1].pieceID = piecePotentialCapture;
                            board[posIndex1][posIndex2+1].pieceName[0] = tolower(board[posIndex1][posIndex2+1].pieceName[0]);
                            board[posIndex1][posIndex2+1].pieceName[1] = tolower(board[posIndex1][posIndex2+1].pieceName[1]);
                        }
                    }
                    else if(board[posIndex1][posIndex2+1].pieceID == pieceSpace){
                        board[posIndex1][posIndex2+1].pieceID = piecePotentialMove;
                        strcpy(board[posIndex1][posIndex2+1].pieceName,potMove);
                }
        }
    }
    // Check if there are any potential moves after calculations. If not return error value 1
    int flag = 0;
    for (int i = 0; i != 8; i++) {
        for (int j = 0; j != 8; j++) {
            if (board[i][j].pieceID == piecePotentialCapture || board[i][j].pieceID == piecePotentialMove) {
                flag += 1;
            }
        }
    }
    if (flag == 0) {
        return 0; // No moves
    } else return flag; // Move count
}

int movePieceCopy(struct Piece( * copyBoard)[boardWidth], struct Piece( * board)[boardWidth], int piecePos1, int piecePos2, int targetPos1, int targetPos2) { // MOVE A PIECE
    if (board[targetPos1][targetPos2].pieceID == piecePotentialMove || board[targetPos1][targetPos2].pieceID == piecePotentialCapture) {
        for (int i = 0; i != 8; i++) {
            for (int j = 0; j != 8; j++) {
                // if (board[i][j].pieceID == piecePotentialMove || board[i][j].pieceID == piecePotentialCapture) { // RESTORE THE OLD BOARD PIECES
                board[i][j].pieceID = copyBoard[i][j].pieceID;
                board[i][j].pieceColorID = copyBoard[i][j].pieceColorID;
                strcpy(board[i][j].pieceName, copyBoard[i][j].pieceName);
            }
        }
        board[targetPos1][targetPos2].pieceID = board[piecePos1][piecePos2].pieceID;
        board[targetPos1][targetPos2].pieceColorID = board[piecePos1][piecePos2].pieceColorID;
        strcpy(board[targetPos1][targetPos2].pieceName, board[piecePos1][piecePos2].pieceName);

        board[piecePos1][piecePos2].pieceID = pieceSpace;
        board[piecePos1][piecePos2].pieceColorID = colorSpace;
        strcpy(board[piecePos1][piecePos2].pieceName, "0");
        return 0;
    } else {
        printf("ENTER A VALID MOVE!!!\n");
        return 1;
    }
}

int checkMate(struct Piece( * board)[boardWidth], int turn,struct enPassantInfo enPassantFlag) {
    // Copy the board info
    struct Piece copyBoard[boardHeight][boardWidth];
    struct Piece copyBoard2[boardHeight][boardWidth];
    int checkedPlayer = turn;
    int notMate = 0;
    memcpy(copyBoard, board, sizeof(struct Piece) * 64);
    struct Piece allPieces[48]; // This will hold all the pieces and their info(ID's colors index)
    int k = 0; // this will count the pieces on the board

    for (int i = 0; i < 8; i++) { // Save the pieces on the board in allPieces[]
        for (int j = 0; j < 8; j++) {
            if (board[i][j].pieceID != pieceSpace) {
                allPieces[k] = board[i][j];
                allPieces[k].position.index1 = board[i][j].position.index1;
                allPieces[k].position.index2 = board[i][j].position.index2;
                k++; //if its a piece count+1

            }
        }
    }
    for (int i = 0; i < k; i++) {
        if (allPieces[i].pieceColorID != checkedPlayer) { // clear the enemy pieces of checked player
            allPieces[i].pieceID = pieceSpace;
            allPieces[i].pieceColorID = colorSpace;
            strcpy(allPieces[i].pieceName, "0");
        }
    }

    memcpy(copyBoard2, copyBoard, sizeof(copyBoard));
    for (int i = 0; i < k; i++) {

        switch (allPieces[i].pieceID) {
        case pieceKing: {
            struct Piece potMoveBoard[boardWidth][boardHeight];
            int possibleMoveCount;
            if ((possibleMoveCount = kingPotentialMoves(copyBoard2, allPieces[i].position.index1, allPieces[i].position.index2)) != 0) {
                memcpy(potMoveBoard, copyBoard2, sizeof(struct Piece) * 64); // save potential moves board
                const int count = possibleMoveCount;
                //struct Piece possibleLocations[count];
                struct Piece* possibleLocations = malloc(sizeof(struct Piece) * count);
                int z = 0;
                for (int x = 0; x != 8; x++) {
                    for (int y = 0; y != 8; y++) {
                        if (copyBoard2[x][y].pieceID == piecePotentialCapture || copyBoard2[x][y].pieceID == piecePotentialMove) {
                            possibleLocations[z].pieceID = copyBoard2[x][y].pieceID;
                            possibleLocations[z].pieceColorID = copyBoard2[x][y].pieceColorID;
                            strcpy(possibleLocations[z].pieceName, copyBoard2[x][y].pieceName);
                            possibleLocations[z].position.index1 = copyBoard2[x][y].position.index1;
                            possibleLocations[z].position.index2 = copyBoard2[x][y].position.index2;
                            z++;
                        }
                    }
                }

                for (int j = 0; j != z; j++) {
                    movePiece(copyBoard, copyBoard2, allPieces[i].position.index1, allPieces[i].position.index2, possibleLocations[j].position.index1, possibleLocations[j].position.index2);
                    if (checkCheck(copyBoard2, checkedPlayer,enPassantFlag) == 0) {
                        notMate = 1;
                        break;
                    }
                    memcpy(copyBoard2, potMoveBoard, sizeof(struct Piece) * 64);
                }
                memcpy(copyBoard2, copyBoard, sizeof(struct Piece) * 64);
                free(possibleLocations);
                break;
            } else {
                memcpy(copyBoard2, copyBoard, sizeof(struct Piece) * 64);
                break;
            }
        }
        case pieceRook: {
            struct Piece potMoveBoard[boardWidth][boardHeight];
            int possibleMoveCount;
            if ((possibleMoveCount = rookPotentialMoves(copyBoard2, allPieces[i].position.index1, allPieces[i].position.index2)) != 0) {
                memcpy(potMoveBoard, copyBoard2, sizeof(struct Piece) * 64); // save potential moves board
                const int count = possibleMoveCount;
                //struct Piece possibleLocations[count];
                struct Piece* possibleLocations = malloc(sizeof(struct Piece) * count);
                int z = 0;
                for (int x = 0; x != 8; x++) {
                    for (int y = 0; y != 8; y++) {
                        if (copyBoard2[x][y].pieceID == piecePotentialCapture || copyBoard2[x][y].pieceID == piecePotentialMove) {
                            possibleLocations[z].pieceID = copyBoard2[x][y].pieceID;
                            possibleLocations[z].pieceColorID = copyBoard2[x][y].pieceColorID;
                            strcpy(possibleLocations[z].pieceName, copyBoard2[x][y].pieceName);
                            possibleLocations[z].position.index1 = copyBoard2[x][y].position.index1;
                            possibleLocations[z].position.index2 = copyBoard2[x][y].position.index2;
                            z++;
                        }
                    }
                }

                for (int j = 0; j != z; j++) {
                    movePiece(copyBoard, copyBoard2, allPieces[i].position.index1, allPieces[i].position.index2, possibleLocations[j].position.index1, possibleLocations[j].position.index2);
                    if (checkCheck(copyBoard2, checkedPlayer,enPassantFlag) == 0) {
                        notMate = 1;
                        break;
                    }
                    memcpy(copyBoard2, potMoveBoard, sizeof(struct Piece) * 64);
                }
                memcpy(copyBoard2, copyBoard, sizeof(struct Piece) * 64);
                free(possibleLocations);
                break;
            } else {
                memcpy(copyBoard2, copyBoard, sizeof(struct Piece) * 64);
                break;
            }
        }
        case pieceKnight: {
            struct Piece potMoveBoard[boardWidth][boardHeight];
            int possibleMoveCount;
            if ((possibleMoveCount = knightPotentialMoves(copyBoard2, allPieces[i].position.index1, allPieces[i].position.index2)) != 0) {
                memcpy(potMoveBoard, copyBoard2, sizeof(struct Piece) * 64); // save potential moves board
                const int count = possibleMoveCount;
                //struct Piece possibleLocations[count];
                struct Piece* possibleLocations = malloc(sizeof(struct Piece) * count);
                int z = 0;
                for (int x = 0; x != 8; x++) {
                    for (int y = 0; y != 8; y++) {
                        if (copyBoard2[x][y].pieceID == piecePotentialCapture || copyBoard2[x][y].pieceID == piecePotentialMove) {
                            possibleLocations[z].pieceID = copyBoard2[x][y].pieceID;
                            possibleLocations[z].pieceColorID = copyBoard2[x][y].pieceColorID;
                            strcpy(possibleLocations[z].pieceName, copyBoard2[x][y].pieceName);
                            possibleLocations[z].position.index1 = copyBoard2[x][y].position.index1;
                            possibleLocations[z].position.index2 = copyBoard2[x][y].position.index2;
                            z++;
                        }
                    }
                }

                for (int j = 0; j != z; j++) {
                    movePiece(copyBoard, copyBoard2, allPieces[i].position.index1, allPieces[i].position.index2, possibleLocations[j].position.index1, possibleLocations[j].position.index2);
                    if (checkCheck(copyBoard2, checkedPlayer,enPassantFlag) == 0) {
                        notMate = 1;
                        break;
                    }
                    memcpy(copyBoard2, potMoveBoard, sizeof(struct Piece) * 64);
                }
                memcpy(copyBoard2, copyBoard, sizeof(struct Piece) * 64);
                free(possibleLocations);
                break;
            } else {
                memcpy(copyBoard2, copyBoard, sizeof(struct Piece) * 64);
                break;
            }
        }
        case pieceBishop: {
            struct Piece potMoveBoard[boardWidth][boardHeight];
            int possibleMoveCount;
            if ((possibleMoveCount = bishopPotentialMoves(copyBoard2, allPieces[i].position.index1, allPieces[i].position.index2)) != 0) {
                memcpy(potMoveBoard, copyBoard2, sizeof(struct Piece) * 64); // save potential moves board
                const int count = possibleMoveCount;
                //struct Piece possibleLocations[count];
                struct Piece* possibleLocations = malloc(sizeof(struct Piece) * count);
                int z = 0;
                for (int x = 0; x != 8; x++) {
                    for (int y = 0; y != 8; y++) {
                        if (copyBoard2[x][y].pieceID == piecePotentialCapture || copyBoard2[x][y].pieceID == piecePotentialMove) {
                            possibleLocations[z].pieceID = copyBoard2[x][y].pieceID;
                            possibleLocations[z].pieceColorID = copyBoard2[x][y].pieceColorID;
                            strcpy(possibleLocations[z].pieceName, copyBoard2[x][y].pieceName);
                            possibleLocations[z].position.index1 = copyBoard2[x][y].position.index1;
                            possibleLocations[z].position.index2 = copyBoard2[x][y].position.index2;
                            z++;
                        }
                    }
                }

                for (int j = 0; j != z; j++) {
                    movePiece(copyBoard, copyBoard2, allPieces[i].position.index1, allPieces[i].position.index2, possibleLocations[j].position.index1, possibleLocations[j].position.index2);
                    if (checkCheck(copyBoard2, checkedPlayer,enPassantFlag) == 0) {
                        notMate = 1;
                        break;
                    }
                    memcpy(copyBoard2, potMoveBoard, sizeof(struct Piece) * 64);
                }
                memcpy(copyBoard2, copyBoard, sizeof(struct Piece) * 64);
                free(possibleLocations);
                break;
            } else {
                memcpy(copyBoard2, copyBoard, sizeof(struct Piece) * 64);
                break;
            }
        }
        case pieceQueen: {
            struct Piece potMoveBoard[boardWidth][boardHeight];
            int possibleMoveCount;
            if ((possibleMoveCount = queenPotentialMoves(copyBoard2, allPieces[i].position.index1, allPieces[i].position.index2)) != 0) {
                memcpy(potMoveBoard, copyBoard2, sizeof(struct Piece) * 64); // save potential moves board
                const int count = possibleMoveCount;
                //struct Piece possibleLocations[count];
                struct Piece* possibleLocations = malloc(sizeof(struct Piece) * count);
                int z = 0;
                for (int x = 0; x != 8; x++) {
                    for (int y = 0; y != 8; y++) {
                        if (copyBoard2[x][y].pieceID == piecePotentialCapture || copyBoard2[x][y].pieceID == piecePotentialMove) {
                            possibleLocations[z].pieceID = copyBoard2[x][y].pieceID;
                            possibleLocations[z].pieceColorID = copyBoard2[x][y].pieceColorID;
                            strcpy(possibleLocations[z].pieceName, copyBoard2[x][y].pieceName);
                            possibleLocations[z].position.index1 = copyBoard2[x][y].position.index1;
                            possibleLocations[z].position.index2 = copyBoard2[x][y].position.index2;
                            z++;
                        }
                    }
                }

                for (int j = 0; j != z; j++) {
                    movePiece(copyBoard, copyBoard2, allPieces[i].position.index1, allPieces[i].position.index2, possibleLocations[j].position.index1, possibleLocations[j].position.index2);
                    if (checkCheck(copyBoard2, checkedPlayer,enPassantFlag) == 0) {
                        notMate = 1;
                        break;
                    }
                    memcpy(copyBoard2, potMoveBoard, sizeof(struct Piece) * 64);
                }
                memcpy(copyBoard2, copyBoard, sizeof(struct Piece) * 64);
                free(possibleLocations);
                break;
            } else {
                memcpy(copyBoard2, copyBoard, sizeof(struct Piece) * 64);
                break;
            }
        }
        case piecePawn: {
            struct Piece potMoveBoard[boardWidth][boardHeight];
            int possibleMoveCount;
            if ((possibleMoveCount = pawnPotentialMoves(copyBoard2, allPieces[i].position.index1, allPieces[i].position.index2,enPassantFlag)) != 0) {
                memcpy(potMoveBoard, copyBoard2, sizeof(struct Piece) * 64); // save potential moves board
                const int count = possibleMoveCount;
                //struct Piece possibleLocations[count];
                struct Piece* possibleLocations = malloc(sizeof(struct Piece) * count);
                int z = 0;
                for (int x = 0; x != 8; x++) {
                    for (int y = 0; y != 8; y++) {
                        if (copyBoard2[x][y].pieceID == piecePotentialCapture || copyBoard2[x][y].pieceID == piecePotentialMove) {
                            possibleLocations[z].pieceID = copyBoard2[x][y].pieceID;
                            possibleLocations[z].pieceColorID = copyBoard2[x][y].pieceColorID;
                            strcpy(possibleLocations[z].pieceName, copyBoard2[x][y].pieceName);
                            possibleLocations[z].position.index1 = copyBoard2[x][y].position.index1;
                            possibleLocations[z].position.index2 = copyBoard2[x][y].position.index2;
                            z++;
                        }
                    }
                }

                for (int j = 0; j != z; j++) {
                    movePiece(copyBoard, copyBoard2, allPieces[i].position.index1, allPieces[i].position.index2, possibleLocations[j].position.index1, possibleLocations[j].position.index2);
                    if (checkCheck(copyBoard2, checkedPlayer,enPassantFlag) == 0) {
                        notMate = 1;
                        break;
                    }
                    memcpy(copyBoard2, potMoveBoard, sizeof(struct Piece) * 64);
                }
                memcpy(copyBoard2, copyBoard, sizeof(struct Piece) * 64);
                free(possibleLocations);
                break;
            } else {
                memcpy(copyBoard2, copyBoard, sizeof(struct Piece) * 64);
                break;
            }
        }
        default:
            break;
        }
    }
    if (notMate == 1) {
        return 0;
    } else return 1;
}

void endGame(struct Piece( * board)[boardWidth], int turn) {
    MessageBox(NULL, "Mate!", "Game over!", MB_OK);
    system("cls");
    printf("Game Finished!\n");
    for (int i = 0; i != 8; i++) {
        printf("\n%d", 8 - i);
        for (int j = 0; j != 8; j++) {
            printf(" %-2s", board[i][j].pieceName);
        }
    }
    if (turn == turnBlack) printf("\nWhite has won!\n");
    else printf("\nBlack has won!!\n");
}

char* convertArrayIndexToChar(int index1,int index2){
char * buffer = malloc(2);
switch (index2) {
    case 0:
        buffer[0] = 'A';
        break;
    case 1:
        buffer[0] = 'B';
        break;
    case 2:
        buffer[0] = 'C';
        break;
    case 3:
        buffer[0] = 'D';
        break;
    case 4:
        buffer[0] = 'E';
        break;
    case 5:
        buffer[0] = 'F';
        break;
    case 6:
        buffer[0] = 'G';
        break;
    case 7:
        buffer[0] = 'H';
        break;
    default:
        break;
    }
    switch (index1) {
    case 0:
        buffer[1] = '8';
        break;
    case 1:
        buffer[1] = '7';
        break;
    case 2:
        buffer[1] = '6';
        break;
    case 3:
        buffer[1] = '5';
        break;
    case 4:
        buffer[1] = '4';
        break;
    case 5:
        buffer[1] = '3';
        break;
    case 6:
        buffer[1] = '2';
        break;
    case 7:
        buffer[1] = '1';
        break;
    default:
        break;
    }
    return buffer;
}

void printPotentialMoves(struct Piece (*board)[boardHeight]){
//    int totalMoves =0;
//    for(int i=0;i!=8;i++){
//        for(int j =0;j!=8;j++){
//if(board[i][j].pieceID == piecePotentialCapture || board[i][j].pieceID == piecePotentialMove ){
//            totalMoves += 1;
//        }
//    }
//}
int k=1;

printf("\n");
for(int i=0;i!=8;i++){
        for(int j =0;j!=8;j++){
if(board[i][j].pieceID == piecePotentialCapture || board[i][j].pieceID == piecePotentialMove ){
            char* convertedPos = convertArrayIndexToChar(board[i][j].position.index1,board[i][j].position.index2);
            printf("%d)%c%c ",k,convertedPos[0],convertedPos[1]);
            free(convertedPos);
            k++;
        }
    }
}
printf(":\n");
}
