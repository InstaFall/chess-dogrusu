#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "chess.h"

// Function to validate the chess board coordinates
int validate_coordinates(char input[]) {
    if (strlen(input) != 2) {
        return 0;
    }

    // Check if the first character is between A-H (or a-h) and the second character is between 1-8
    if ((input[0] >= 'A' && input[0] <= 'H') || (input[0] >= 'a' && input[0] <= 'h')) {
        if (input[1] >= '1' && input[1] <= '8') {
            return 1;
        }
    }

    return 0;
}

void clear_input_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

void getCoordInput(char* userInput) {
    int valid = 0;
    do {
        if (fgets(userInput, 3, stdin) != NULL)
        {
            // fgets also reads the newline character, so we need to remove it
            if ((strlen(userInput) > 0) && (userInput[strlen(userInput) - 1] == '\n')) {
                userInput[strlen(userInput) - 1] = '\0';
            }
            // Now clear the input buffer
            clear_input_buffer();
        }

        valid = validate_coordinates(userInput);
        if (!valid) {
            printf("ENTER A VALID SQUARE!\n");
        }
        printf("getcoord loop\n");
    } while (!valid);
}

int main()
{
    struct Piece board[boardHeight][boardWidth];
    struct Piece copyBoard[boardWidth][boardHeight]; // A copy of the board to use in movePiece functions
    int turn = turnWhite;
    char userInputPos[3];
    char userInputTar[3];
    int checkFlag = 0;
    int mateFlag = 0;
    struct enPassantInfo enPassantFlag;
    enPassantFlag.flag = 0;
    capturedFlag = 0;

    initiateBoard(board);
    render(board, turn);

    do
    {
        checkFlag = checkCheck(board, turn, enPassantFlag);
        if (checkFlag == 0)
        {
            int valid = 0;
            printf("Type the piece you want to play: ");
            getCoordInput(userInputPos);

            int *convertedPos = convertToArrayIndex(userInputPos); // Convert the input coordinate(letter + number) to array index([num][num])
            if (board[convertedPos[0]][convertedPos[1]].pieceColorID != turn) {
                 // If player selects a piece of the opponent continue the loop to ask input again
                    if (turn == turnBlack) printf("YOU ARE PLAYING AS BLACK. CHOOSE A BLACK PIECE!\n");
                    else printf("YOU ARE PLAYING AS WHITE. CHOOSE A WHITE PIECE!\n");
                    free(convertedPos);
                    printf("Continue is ran\n'");
                    continue;
            }

            switch (board[convertedPos[0]][convertedPos[1]].pieceID)
            { // Check and evaluate the chosen square by  its pieceID
            case pieceSpace:
            { // If player chooses an empty square
                printf("YOU CAN'T SELECT AN EMPTY SQUARE\n");
                free(convertedPos);
                break;
            }

            case piecePawn:
            {
                memcpy(copyBoard, board, sizeof(board)); // save the boards current state
                if (pawnPotentialMoves(board, convertedPos[0], convertedPos[1], enPassantFlag) == 0)
                {
                    printf("Choose a valid piece! ");
                    free(convertedPos);
                    break;
                }
                else
                {
                    int failed = 0;
                    char movingPieceName[2] = {board[convertedPos[0]][convertedPos[1]].pieceName[0], board[convertedPos[0]][convertedPos[1]].pieceName[1]};
                    render(board, turn);
                    do
                    {
                        printf("Moving %s Type where you want to move: ", userInputPos);
                        printPotentialMoves(board);
                        getCoordInput(userInputTar);
                        int *convertedTargetPos = convertToArrayIndex(userInputTar);
                        char targetPieceName[3] = {copyBoard[convertedTargetPos[0]][convertedTargetPos[1]].pieceName[0], copyBoard[convertedTargetPos[0]][convertedTargetPos[1]].pieceName[1]};
                        if ((movePiece(copyBoard, board, convertedPos[0], convertedPos[1], convertedTargetPos[0], convertedTargetPos[1]) == 0))
                        {
                            if (checkCheck(board, turn, enPassantFlag) == 0)
                            {
                                render(board, turn);
                                if (enPassantFlag.flag == 1 && enPassantFlag.position.index2 == convertedTargetPos[1])
                                { // if enPassant occured, remove the passed pawn from the board
                                    if (turn == turnWhite)
                                    {
                                        board[enPassantFlag.position.index1 + 1][enPassantFlag.position.index2].pieceID = pieceSpace;
                                        board[enPassantFlag.position.index1 + 1][enPassantFlag.position.index2].pieceColorID = colorSpace;
                                        strcpy(board[enPassantFlag.position.index1 + 1][enPassantFlag.position.index2].pieceName, "0");
                                    }
                                    else
                                    {
                                        board[enPassantFlag.position.index1 - 1][enPassantFlag.position.index2].pieceID = pieceSpace;
                                        board[enPassantFlag.position.index1 - 1][enPassantFlag.position.index2].pieceColorID = colorSpace;
                                        strcpy(board[enPassantFlag.position.index1 - 1][enPassantFlag.position.index2].pieceName, "0");
                                    }
                                }
                                enPassantFlag.flag = 0;
                                if (abs(convertedTargetPos[0] - convertedPos[0]) == 2)
                                {
                                    if ((board[convertedTargetPos[0]][convertedTargetPos[1] + 1].pieceID == piecePawn && isSameColor(board[convertedTargetPos[0]][convertedTargetPos[1] + 1], board[convertedTargetPos[0]][convertedTargetPos[1]]) != 0) || (board[convertedTargetPos[0]][convertedTargetPos[1] - 1].pieceID == piecePawn && isSameColor(board[convertedTargetPos[0]][convertedTargetPos[1] - 1], board[convertedTargetPos[0]][convertedTargetPos[1]]) != 0))
                                    {
                                        enPassantFlag.flag = 1;
                                        enPassantFlag.position.index2 = convertedTargetPos[1];
                                        enPassantFlag.position.index1 = (turn == turnBlack) ? convertedTargetPos[0] - 1 : convertedTargetPos[0] + 1;
                                    }
                                }
                                if (convertedTargetPos[0] == 0 || convertedTargetPos[0] == 7)
                                {
                                    promotePawn(board, convertedTargetPos[0], convertedTargetPos[1]);
                                    turn = (turn == turnWhite) ? turnBlack : turnWhite;
                                    free(convertedTargetPos);
                                    break;
                                }
                                if (capturedFlag != 0)
                                {
                                    printf("Piece captured: %s %c%c -> %c%c %c%c", userInputPos, movingPieceName[0], movingPieceName[1], userInputTar[0], userInputTar[1], targetPieceName[0], targetPieceName[1]);
                                    Sleep(1200);
                                }
                                capturedFlag = 0;
                                turn = (turn == turnWhite) ? turnBlack : turnWhite;
                                render(board, turn);
                                free(convertedTargetPos);
                                break;
                            }
                            else if (checkCheck(board, turn, enPassantFlag) != 0)
                            { // If the move puts the player in a check break the loop and ask for input again
                                memcpy(board, copyBoard, sizeof(copyBoard));
                                free(convertedTargetPos);
                                printf("Illegal move!!\n");
                                Sleep(1000);
                                render(board, turn);
                                break;
                            }
                        }
                        else
                        {
                            free(convertedTargetPos);
                            failed = 1;
                        }
                        free(convertedTargetPos);
                    } while (failed == 1);
                    render(board, turn);
                    free(convertedPos);
                    break;
                }
            }
            case pieceRook:
            {
                memcpy(copyBoard, board, sizeof(board));
                if ((rookPotentialMoves(board, convertedPos[0], convertedPos[1])) == 0)
                {                                     // mark the potential squares that the piece can go
                    printf("Choose a valid piece! "); // if the selected piece hasn't any potential moves break the loop and ask again
                    free(convertedPos);
                    break;
                }
                else
                {
                    int failed = 0;
                    char movingPieceName[3] = {board[convertedPos[0]][convertedPos[1]].pieceName[0], board[convertedPos[0]][convertedPos[1]].pieceName[1]};
                    render(board, turn); // Display the board with marked squares of the potential moves
                    do
                    {
                        printf("Moving %s Type where you want to move: ", userInputPos);
                        printPotentialMoves(board);
                        getCoordInput(userInputTar);
                        int *convertedTargetPos = convertToArrayIndex(userInputTar);
                        char targetPieceName[3] = {copyBoard[convertedTargetPos[0]][convertedTargetPos[1]].pieceName[0], copyBoard[convertedTargetPos[0]][convertedTargetPos[1]].pieceName[1]};
                        if ((movePiece(copyBoard, board, convertedPos[0], convertedPos[1], convertedTargetPos[0], convertedTargetPos[1])) == 0)
                        {
                            if (checkCheck(board, turn, enPassantFlag) == 0)
                            {
                                enPassantFlag.flag = 0;
                                render(board, turn);
                                if (capturedFlag != 0)
                                {
                                    printf("Piece captured: %s %c%c -> %c%c %c%c", userInputPos, movingPieceName[0], movingPieceName[1], userInputTar[0], userInputTar[1], targetPieceName[0], targetPieceName[1]);
                                    Sleep(1200);
                                }
                                capturedFlag = 0;
                                turn = (turn == turnWhite) ? turnBlack : turnWhite;
                                free(convertedTargetPos);
                                render(board, turn);
                                break;
                            }
                            else if (checkCheck(board, turn, enPassantFlag) != 0)
                            {
                                free(convertedTargetPos);
                                memcpy(board, copyBoard, sizeof(copyBoard));
                                printf("Illegal move!!\n");
                                Sleep(1000);
                                render(board, turn);
                                break;
                            }
                        }
                        else
                        {
                            free(convertedTargetPos);
                            failed = 1;
                        }
                    } while (failed == 1);
                    free(convertedPos);
                    break;
                }
            }
            case pieceKnight:
            {
                memcpy(copyBoard, board, sizeof(board));
                if ((knightPotentialMoves(board, convertedPos[0], convertedPos[1])) == 0)
                {                                     // mark the potential squares that the piece can go
                    printf("Choose a valid piece! "); // if the selected piece hasn't any potential moves break the loop and ask again
                    free(convertedPos);
                    break;
                }
                else
                {
                    int failed = 0;
                    char movingPieceName[2] = {board[convertedPos[0]][convertedPos[1]].pieceName[0], board[convertedPos[0]][convertedPos[1]].pieceName[1]};
                    render(board, turn); // Display the board with marked squares of the potential moves
                    do
                    {
                        printf("Moving %s Type where you want to move: ", userInputPos);
                        printPotentialMoves(board);
                        getCoordInput(userInputTar);
                        int *convertedTargetPos = convertToArrayIndex(userInputTar);
                        char targetPieceName[3] = {copyBoard[convertedTargetPos[0]][convertedTargetPos[1]].pieceName[0], copyBoard[convertedTargetPos[0]][convertedTargetPos[1]].pieceName[1]};
                        if ((movePiece(copyBoard, board, convertedPos[0], convertedPos[1], convertedTargetPos[0], convertedTargetPos[1])) == 0)
                        {
                            if (checkCheck(board, turn, enPassantFlag) == 0)
                            {
                                enPassantFlag.flag = 0;
                                render(board, turn);
                                if (capturedFlag != 0)
                                {
                                    printf("Piece captured: %s %c%c -> %c%c %c%c", userInputPos, movingPieceName[0], movingPieceName[1], userInputTar[0], userInputTar[1], targetPieceName[0], targetPieceName[1]);
                                    Sleep(1200);
                                }
                                capturedFlag = 0;
                                turn = (turn == turnWhite) ? turnBlack : turnWhite;
                                free(convertedTargetPos);
                                //     checkFlag = checkCheck(board,turn);
                                render(board, turn);
                                break;
                            }
                            else if (checkCheck(board, turn, enPassantFlag) != 0)
                            {
                                free(convertedTargetPos);
                                memcpy(board, copyBoard, sizeof(copyBoard));
                                printf("Illegal move!!\n");
                                Sleep(1000);
                                render(board, turn);
                                break;
                            }
                        }
                        else
                        {
                            free(convertedTargetPos);
                            failed = 1;
                        }
                    } while (failed == 1);
                    free(convertedPos);
                    break;
                }
            }
            case pieceBishop:
            {
                memcpy(copyBoard, board, sizeof(board));
                if (bishopPotentialMoves(board, convertedPos[0], convertedPos[1]) == 0)
                {                                     // mark the potential squares that the piece can go
                    printf("Choose a valid piece! "); // if the selected piece hasn't any potential moves break the loop and ask again
                    free(convertedPos);
                    break;
                }
                else
                {
                    int failed = 0;
                    char movingPieceName[2] = {board[convertedPos[0]][convertedPos[1]].pieceName[0], board[convertedPos[0]][convertedPos[1]].pieceName[1]};
                    render(board, turn); // Display the board with marked squares of the potential moves
                    do
                    {
                        printf("Moving %s Type where you want to move: ", userInputPos);
                        printPotentialMoves(board);
                        getCoordInput(userInputTar);
                        int *convertedTargetPos = convertToArrayIndex(userInputTar);
                        char targetPieceName[3] = {copyBoard[convertedTargetPos[0]][convertedTargetPos[1]].pieceName[0], copyBoard[convertedTargetPos[0]][convertedTargetPos[1]].pieceName[1]};
                        if ((movePiece(copyBoard, board, convertedPos[0], convertedPos[1], convertedTargetPos[0], convertedTargetPos[1])) == 0)
                        {
                            if (checkCheck(board, turn, enPassantFlag) == 0)
                            {
                                enPassantFlag.flag = 0;
                                render(board, turn);
                                if (capturedFlag != 0)
                                {
                                    printf("Piece captured: %s %c%c -> %c%c %c%c", userInputPos, movingPieceName[0], movingPieceName[1], userInputTar[0], userInputTar[1], targetPieceName[0], targetPieceName[1]);
                                    Sleep(1200);
                                }
                                capturedFlag = 0;
                                turn = (turn == turnWhite) ? turnBlack : turnWhite;
                                free(convertedTargetPos);
                                render(board, turn);
                                break;
                            }
                            else if (checkCheck(board, turn, enPassantFlag) != 0)
                            {
                                free(convertedTargetPos);
                                memcpy(board, copyBoard, sizeof(copyBoard));
                                printf("Illegal move!!\n");
                                Sleep(1000);
                                render(board, turn);
                                break;
                            }
                        }
                        else
                        {
                            free(convertedTargetPos);
                            failed = 1;
                        }
                    } while (failed == 1);
                    free(convertedPos);
                    break;
                }
            }
            case pieceQueen:
            {
                memcpy(copyBoard, board, sizeof(board));
                if (queenPotentialMoves(board, convertedPos[0], convertedPos[1]) == 0)
                {                                     // mark the potential squares that the piece can go
                    printf("Choose a valid piece! "); // if the selected piece hasn't any potential moves break the loop and ask again
                    free(convertedPos);
                    break;
                }
                else
                {
                    int failed = 0;
                    char movingPieceName[2] = {board[convertedPos[0]][convertedPos[1]].pieceName[0], board[convertedPos[0]][convertedPos[1]].pieceName[1]};
                    render(board, turn); // Display the board with marked squares of the potential moves
                    do
                    {
                        printf("Moving %s Type where you want to move: ", userInputPos);
                            printPotentialMoves(board);
                            if (fgets(userInputTar, 16, stdin) != NULL)
                            getCoordInput(userInputTar);
                       
                        int *convertedTargetPos = convertToArrayIndex(userInputTar);
                        char targetPieceName[3] = {copyBoard[convertedTargetPos[0]][convertedTargetPos[1]].pieceName[0], copyBoard[convertedTargetPos[0]][convertedTargetPos[1]].pieceName[1]};
                        if ((movePiece(copyBoard, board, convertedPos[0], convertedPos[1], convertedTargetPos[0], convertedTargetPos[1])) == 0)
                        {
                            if (checkCheck(board, turn, enPassantFlag) == 0)
                            {
                                enPassantFlag.flag = 0;
                                render(board, turn);
                                if (capturedFlag != 0)
                                {
                                    printf("Piece captured: %s %c%c -> %c%c %c%c", userInputPos, movingPieceName[0], movingPieceName[1], userInputTar[0], userInputTar[1], targetPieceName[0], targetPieceName[1]);
                                    Sleep(1200);
                                }
                                capturedFlag = 0;
                                turn = (turn == turnWhite) ? turnBlack : turnWhite;
                                checkFlag = checkCheck(board, turn, enPassantFlag);
                                free(convertedTargetPos);
                                render(board, turn);
                                break;
                            }
                            else if (checkCheck(board, turn, enPassantFlag) != 0)
                            {
                                free(convertedTargetPos);
                                memcpy(board, copyBoard, sizeof(copyBoard));
                                printf("Illegal move!!\n");
                                Sleep(1000);
                                render(board, turn);
                                break;
                            }
                        }
                        else
                        {
                            free(convertedTargetPos);
                            failed = 1;
                        }
                    } while (failed == 1);
                    free(convertedPos);
                    break;
                }
            }
            case pieceKing:
            {
                memcpy(copyBoard, board, sizeof(board));
                if (kingPotentialMoves(board, convertedPos[0], convertedPos[1]) == 0)
                {                                     // mark the potential squares that the piece can go
                    printf("Choose a valid piece! "); // if the selected piece hasn't any potential moves break the loop and ask again
                    free(convertedPos);
                    break;
                }
                else
                {
                    int failed = 0;
                    char movingPieceName[2] = {board[convertedPos[0]][convertedPos[1]].pieceName[0], board[convertedPos[0]][convertedPos[1]].pieceName[1]};
                    render(board, turn); // Display the board with marked squares of the potential moves
                    do
                    {
                            printf("Moving %s Type where you want to move: ", userInputPos);
                            printPotentialMoves(board);
                            getCoordInput(userInputTar);
                        int *convertedTargetPos = convertToArrayIndex(userInputTar);
                        char targetPieceName[3] = {copyBoard[convertedTargetPos[0]][convertedTargetPos[1]].pieceName[0], copyBoard[convertedTargetPos[0]][convertedTargetPos[1]].pieceName[1]};
                        if ((movePiece(copyBoard, board, convertedPos[0], convertedPos[1], convertedTargetPos[0], convertedTargetPos[1])) == 0)
                        {

                            if (checkCheck(board, turn, enPassantFlag) == 0)
                            {
                                enPassantFlag.flag = 0;
                                render(board, turn);
                                if (capturedFlag != 0)
                                {
                                    printf("Piece captured: %s %c%c -> %c%c %c%c", userInputPos, movingPieceName[0], movingPieceName[1], userInputTar[0], userInputTar[1], targetPieceName[0], targetPieceName[1]);
                                    Sleep(1200);
                                }
                                capturedFlag = 0;
                                turn = (turn == turnWhite) ? turnBlack : turnWhite;
                                free(convertedTargetPos);
                                //       checkFlag = checkCheck(board,turn);
                                render(board, turn);
                                break;
                            }
                            else if (checkCheck(board, turn, enPassantFlag) != 0)
                            {
                                memcpy(board, copyBoard, sizeof(copyBoard));
                                printf("Illegal move!!\n");
                                Sleep(1000);
                                render(board, turn);
                                break;
                            }
                        }
                        else
                        {
                            free(convertedTargetPos);
                            failed = 1;
                        }
                    } while (failed == 1);
                    free(convertedPos);
                    break;
                }
            }
            default:
                printf("Error");
            }
        }
            else if (checkFlag != 0)
            {
                memcpy(copyBoard, board, sizeof(board)); // save the check position, if player moves illegal revert to this,
                if ((mateFlag = checkMate(copyBoard, turn, enPassantFlag)) == 0)
                { // check mate
                    do
                    {
                        render(board, turn);
                        printf("You are in Check!! Type the piece you want to play: ");
                        getCoordInput(userInputTar);

                        int* convertedPos = convertToArrayIndex(userInputPos);
                        checkFlag = evaluateCheck(copyBoard, board, convertedPos[0], convertedPos[1], &turn, userInputPos, enPassantFlag);
                        free(convertedPos);
                    } while (checkFlag != 0);
                }
                if (mateFlag != 0)
                {
                    endGame(board, turn);
                }
            }
    } while (mateFlag != 1);
    printf("GAME OVER");
    Sleep(3000);
    return 0;
}
