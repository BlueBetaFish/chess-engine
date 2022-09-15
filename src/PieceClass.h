#pragma once

#include "definitions.h"

class Piece
{
public:
    enum PIECE_TYPE
    {

        P,
        N,
        B,
        R,
        Q,
        K,
        p,
        n,
        b,
        r,
        q,
        k,
        EMPTY_PIECE

    };

    static int whitePawn;
    static int whiteKnight;
    static int whiteBishop;
    static int whiteRook;
    static int whiteQueen;
    static int whiteKing;

    static int blackPawn;
    static int blackKnight;
    static int blackBishop;
    static int blackRook;
    static int blackQueen;
    static int blackKing;

    static int emptyPiece;

    static inline char getASCIICodeOfPiece(int piece)
    {
        switch (piece)
        {
        case P:
            return 'P';
        case N:
            return 'N';
        case B:
            return 'B';
        case R:
            return 'R';
        case Q:
            return 'Q';
        case K:
            return 'K';

        case p:
            return 'p';
        case n:
            return 'n';
        case b:
            return 'b';
        case r:
            return 'r';
        case q:
            return 'q';
        case k:
            return 'k';

        case EMPTY_PIECE:
            return '.';

        default:
            throw runtime_error("\n\ninvalid piece type\n");
            return '!';
        }
    }

    static inline char getPieceValueFromASCII(char piece)
    {
        switch (piece)
        {
        case 'P':
            return P;
        case 'N':
            return N;
        case 'B':
            return B;
        case 'R':
            return R;
        case 'Q':
            return Q;
        case 'K':
            return K;

        case 'p':
            return p;
        case 'n':
            return n;
        case 'b':
            return b;
        case 'r':
            return r;
        case 'q':
            return q;
        case 'k':
            return k;

        case '.':
            return EMPTY_PIECE;

        default:
            throw runtime_error("\n\ninvalid piece type\n");

            return -1;
        }
    }

    static int inline colorOfGivenPiece(int pieceValue)
    {
        switch (pieceValue)
        {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            return WHITE;

        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            return BLACK;

        case 12: // empty piece
            return NO_COLOR;

        //*invalid
        default:
            throw runtime_error("\n\ninvalid piece type\n");

            return -1;
        }
    }

    static int inline colorOfGivenPiece(char pieceASCIICode)
    {
        switch (pieceASCIICode)
        {
        case 'P':
        case 'N':
        case 'B':
        case 'R':
        case 'Q':
        case 'K':
            return WHITE;

        case 'p':
        case 'n':
        case 'b':
        case 'r':
        case 'q':
        case 'k':
            return BLACK;

        case '.':
            return NO_COLOR;

        //*invalid
        default:
            throw runtime_error("\n\ninvalid piece type\n");

            return -1;
        }
    }

    static int inline getOppositeColor(int color)
    {
        if (color == WHITE)
            return BLACK;
        if (color == BLACK)
            return WHITE;

        return -1;
    }
};

int Piece::whitePawn = P;
int Piece::whiteKnight = N;
int Piece::whiteBishop = B;
int Piece::whiteRook = R;
int Piece::whiteQueen = Q;
int Piece::whiteKing = K;

int Piece::blackPawn = p;
int Piece::blackKnight = n;
int Piece::blackBishop = b;
int Piece::blackRook = r;
int Piece::blackQueen = q;
int Piece::blackKing = k;

int Piece::emptyPiece = EMPTY_PIECE;