#ifndef CHESS_H
#define CHESS_H

#define pieceSpace 0x0
#define pieceRook 0x1
#define pieceKnight 0x2
#define pieceBishop 0x3
#define pieceQueen 0x4
#define pieceKing 0x5
#define piecePawn 0x6
#define piecePotentialMove 0x7
#define piecePotentialCapture 0x8

#define colorWhite 0x1
#define colorBlack 0x2
#define colorSpace 0x0

#define boardWidth 8
#define boardHeight 8

#define turnWhite 0x1
#define turnBlack 0x2

extern int capturedFlag; // A flag to use if a piece gets captured

struct Coords { // simple struct for x,y positions(array index board[x][y])
    int index1;
    int index2;
};

struct enPassantInfo{
int flag;
struct Coords position;
};

struct Piece { // A piece structure to represent pieces on the board
    struct Coords position;
    int pieceColorID;
    int pieceID;
    char pieceName[3];
};

extern int * convertToArrayIndex(char * userInputPos); // A function to convert input position like A3 to an array index like board[5][0]

extern char* convertArrayIndexToChar(int index1,int index2); // Reverse of the ConvertToArrayIndex function

extern void initiateBoard(struct Piece board[boardHeight][boardWidth]); // A function to initiate the board and put the pieces on correct locations

extern int render(struct Piece board[boardHeight][boardWidth], int turn); // A function to render the chess board[][]

extern int isSameColor(struct Piece piece1, struct Piece piece2); // A function to check whether two pieces have the same color

extern void promotePawn(struct Piece( * board)[boardWidth], int posIndex1, int posIndex2); // rank up a pawn

extern int pawnPotentialMoves(struct Piece( * board)[boardWidth], int posIndex1, int posIndex2,struct enPassantInfo enPassantFlag); // A function to calculate potential locations on the board for a pawn to move

extern int rookPotentialMoves(struct Piece( * board)[boardHeight], int posIndex1, int posIndex2); // A function to calculate potential locations on the board for a rook to move

extern int knightPotentialMoves(struct Piece( * board)[boardHeight], int posIndex1, int posIndex2);

extern int bishopPotentialMoves(struct Piece( * board)[boardHeight], int posIndex1, int posIndex2);

extern int queenPotentialMoves(struct Piece( * board)[boardHeight], int posIndex1, int posIndex2);

extern int kingPotentialMoves(struct Piece( * board)[boardHeight], int posIndex1, int posIndex2);

extern int checkCheck(struct Piece( * board)[boardWidth], int turn,struct enPassantInfo enPassantFlag); // A function to check if there is a check on current turn

extern int evaluateCheck(struct Piece copyBoard[boardHeight][boardWidth], struct Piece( * board)[boardWidth], int pos1, int pos2, int * turn, char * userInput,struct enPassantInfo enPassantFlag); // A function to evaluate the inputs when a player is in check

extern int checkMate(struct Piece( * board)[boardWidth], int turn,struct enPassantInfo enPassantFlag); // A function to check mate

extern int movePiece(struct Piece( * copyBoard)[boardWidth], struct Piece( * board)[boardWidth], int piecePos1, int piecePos2, int targetPos1, int targetPos2); // A function to move pieces

extern void printPotentialMoves(struct Piece (*board)[boardHeight]);

void endGame(struct Piece( * board)[boardWidth], int turn); // display end game message
#endif // CHESS_H
