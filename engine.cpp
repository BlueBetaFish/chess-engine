// Name: SOURASHIS MONDAL

#include <bits/stdc++.h>

using namespace std;
class BitBoard;

#include "definitions.h"

#include "BitBoard.h"
#include "PieceClass.h"

#include "tables.h"

//*helper functions
#include "helperFunctions.h"

//*Constants

//*_____________________________ATTACK TABLES____________________________________________________________________

void initializeSliderPieceAttackTables()
{
    for (int squareIndex = 0; squareIndex < 64; squareIndex++)
    {
        //*for bishop_____________________________________________________________________________
        int relevantBitCount = BISHOP_ATTACK_MASK_SET_BIT_COUNT[squareIndex];
        // initialize occupancyIndex
        int occupancyIndex = (1 << relevantBitCount);

        for (int index = 0; index < occupancyIndex; index++)
        {
            // initilalize current occupancy variation
            BitBoard occupancy = setOccupancy(index, BISHOP_ATTACK_MASK[squareIndex]);

            // initialize magic index
            U64 magicIndex = (occupancy.getDecimalValue() * BISHOP_MAGIC_NUMBER[squareIndex]) >> (64 - relevantBitCount);

            // initialize bishop attacks
            BISHOP_ATTACK_TABLE[squareIndex][magicIndex] = generateBishopAttacksOnTheFly(squareIndex, occupancy);
        }

        //*for rook_____________________________________________________________________________
        relevantBitCount = ROOK_ATTACK_MASK_SET_BIT_COUNT[squareIndex];
        // initialize occupancyIndex
        occupancyIndex = (1 << relevantBitCount);

        for (int index = 0; index < occupancyIndex; index++)
        {
            // initilalize current occupancy variation
            BitBoard occupancy = setOccupancy(index, ROOK_ATTACK_MASK[squareIndex]);

            // initialize magic index
            U64 magicIndex = (occupancy.getDecimalValue() * ROOK_MAGIC_NUMBER[squareIndex]) >> (64 - relevantBitCount);

            // initialize bishop attacks
            ROOK_ATTACK_TABLE[squareIndex][magicIndex] = generateRookAttacksOnTheFly(squareIndex, occupancy);
        }
    }
}

//*Finds bishop attacks assuming current board occupancy as "blockers"
BitBoard inline getBishopAttacks(int squareIndex, const BitBoard &blockers)
{
    U64 occupancyValue = blockers.getDecimalValue();

    occupancyValue &= BISHOP_ATTACK_MASK[squareIndex].getDecimalValue();
    occupancyValue *= BISHOP_MAGIC_NUMBER[squareIndex];
    occupancyValue >>= (64 - BISHOP_ATTACK_MASK_SET_BIT_COUNT[squareIndex]);

    U64 index = occupancyValue;
    return BISHOP_ATTACK_TABLE[squareIndex][index];
}

//*Finds rook attacks assuming current board occupancy as "blockers"
BitBoard inline getRookAttacks(int squareIndex, const BitBoard &blockers)
{
    U64 occupancyValue = blockers.getDecimalValue();

    occupancyValue &= ROOK_ATTACK_MASK[squareIndex].getDecimalValue();
    occupancyValue *= ROOK_MAGIC_NUMBER[squareIndex];
    occupancyValue >>= (64 - ROOK_ATTACK_MASK_SET_BIT_COUNT[squareIndex]);

    U64 index = occupancyValue;
    return ROOK_ATTACK_TABLE[squareIndex][index];
}

BitBoard inline getQueenAttacks(int squareIndex, const BitBoard &blockers)
{
    U64 res = getBishopAttacks(squareIndex, blockers).getDecimalValue() | getRookAttacks(squareIndex, blockers).getDecimalValue();
    return BitBoard(res);
}

class Board
{
    //*bitboards for each piece of each color
    // BitBoard whitePawn, whiteKnight, whiteBishop, whiteRook, whiteQueen, whiteKing;
    // BitBoard blackPawn, blackKnight, blackBishop, blackRook, blackQueen, blackKing;
    //*Pieces enum : {P = 0, N = 1, B = 2, R = 3, Q = 4, K = 5, p = 6, n = 7, b = 8, r = 9, q = 10, k = 11, EMPTY_PIECE = 12}
    vector<BitBoard> pieceBitBoards; // size 12

    //*side to move
    COLOR currentPlayer;

    //*en passant square (-1 means no enpassant square available)
    int enPassantSquareIndex;

    //*castling rights : [whiteKingSide, whiteQueenSide, blackKingSide, blackQueenSide]
    vector<bool> castlingRights;

public:
    Board()
    {
        for (int i = 0; i < 12; i++)
            this->pieceBitBoards.push_back(BitBoard(0ULL));

        this->currentPlayer = WHITE;
        this->enPassantSquareIndex = -1;
        this->castlingRights = {true, true, true, true};
    }

    // get the piece at given square
    int inline getPieceAtSquare(int squareIndex)
    {
        for (int piece = P; piece <= k; piece++)
        {
            if (this->pieceBitBoards[piece].getBitAt(squareIndex) == 1)
                return piece;
        }
        return EMPTY_PIECE;
    }

    BitBoard inline getWhiteOccupancyBitBoard()
    {
        U64 res = 0;
        for (int piece = Piece::whitePawn; piece <= Piece::whiteKing; piece++)
            res |= this->pieceBitBoards[piece].getDecimalValue();

        return BitBoard(res);
    }

    BitBoard inline getBlackOccupancyBitBoard()
    {
        U64 res = 0;
        for (int piece = Piece::blackPawn; piece <= Piece::blackKing; piece++)
            res |= this->pieceBitBoards[piece].getDecimalValue();

        return BitBoard(res);
    }

    BitBoard inline getAllOccupancyBitBoard()
    {
        U64 res = 0;
        for (int piece = Piece::whitePawn; piece <= Piece::blackKing; piece++)
            res |= this->pieceBitBoards[piece].getDecimalValue();

        return BitBoard(res);
    }

    void inline print(bool printPieceBitBoards = false)
    {
        //*print piece board
        cout << "Piece Board: " << endl;
        for (int i = 0; i < 8; i++)
        {
            // print rank number
            int rank = 8 - i;
            // cout << rank << "    ";

            cout << "+---+---+---+---+---+---+---+---+" << endl;

            for (int j = 0; j < 8; j++)
            {
                int file = j;
                int index = i * 8 + j;

                char piece = Piece::getASCIICodeOfPiece(this->getPieceAtSquare(index));

                cout << "| ";
                cout << (piece != '.' ? piece : ' ');
                cout << " ";
            }
            cout << "|";
            cout << "   " << rank << endl;
        }
        cout << "+---+---+---+---+---+---+---+---+---+---+" << endl;
        cout << "  a   b   c   d   e   f   g   h   --------> File";

        //*print other board states
        cout << "\n\nCurrentPlayer = " << (this->currentPlayer == WHITE ? "WHITE" : "BLACK") << endl;
        cout << "EnPassant square = " << this->enPassantSquareIndex << (this->enPassantSquareIndex != -1 ? " (" + getAlgebraicCoordinateFromIndex(this->enPassantSquareIndex) + ")" : "") << endl;

        string castlingRightsTemp = string((this->castlingRights[0] ? "K" : "-")) + (this->castlingRights[1] ? "Q" : "-") + (this->castlingRights[2] ? "k" : "-") + (this->castlingRights[3] ? "q" : "-");
        cout << "Castling Rights = " << castlingRightsTemp << endl;

        cout << endl
             << endl
             << endl;

        if (printPieceBitBoards)
        {
            for (int piece = Piece::whitePawn; piece <= Piece::blackKing; piece++)
            {
                cout << Piece::getASCIICodeOfPiece(piece) << " ---- BitBoard :" << endl;
                this->pieceBitBoards[piece].print();
            }

            cout << endl
                 << endl;
        }
    }

    // *changes the board object and assigns the new board from fen
    inline void initializeFromFenString(const string &fen)
    {
        //* reset member fields ______________________________________________________________

        for (int i = 0; i < this->pieceBitBoards.size(); i++)
            this->pieceBitBoards[i] = 0ULL;

        for (int i = 0; i < this->castlingRights.size(); i++)
            this->castlingRights[i] = false;

        this->currentPlayer = WHITE;
        this->enPassantSquareIndex = -1;

        //*update the positions of pieces according to fen string________________________________
        int fenSize = fen.size();
        int k = 0;

        int rank = 8, file = 1;
        while (k < fenSize && fen[k] != ' ')
        {
            if (isdigit(fen[k]))
            {
                file += (fen[k] - '0');
                // make these squares empty

                if (file > 9)
                    throw runtime_error("file > 9 inside initializeFromFenString() function");
            }
            else if (fen[k] == '/')
            {
                if (file != 9)
                    throw runtime_error("file!=9 inside initializeFromFenString() function");
                if (--rank < 1)
                    throw runtime_error("rank<1 inside initializeFromFenString() function");

                file = 1;
            }
            else
            {
                int pieceVal = Piece::getPieceValueFromASCII(fen[k]);

                // if invalid piece character inside fen
                if (pieceVal == -1)
                    throw runtime_error("invalid piece character of fen inside initializeFromFenString() function");

                //*set the piece
                this->pieceBitBoards[pieceVal].setBitAt((8 - rank) * 8 + (file - 1));

                if (++file > 9)
                    throw runtime_error("file > 9 initializeFromFenString() function");
            }

            //*increment k
            k++;
        }

        if (rank != 1 || file != 9)
            throw runtime_error("rank!=1 || file!=9   inside initializeFromFenString() function");

        //*update other board state variables __________________________________________________________________
        k++;

        if (k >= fenSize || !(fen[k] == 'w' || fen[k] == 'b'))
            throw runtime_error("invalid player color inside initializeFromFenString() function");

        this->currentPlayer = fen[k] == 'w' ? WHITE : BLACK;

        //*increment k
        k++;

        //*if next character is not white space
        if (k >= fenSize || fen[k] != ' ')
            throw runtime_error("white space missing after currentPlayer in FEN inside initializeFromFenString() function");

        //*ignore white space
        k++;

        //*castling availability
        if (k >= fenSize)
            throw runtime_error("no castling availability inside initializeFromFenString() function");

        if (fen[k] == '-')
        {
            for (int i = 0; i < this->castlingRights.size(); i++)
                this->castlingRights[i] = false;

            // *skip the castling string
            k++;
        }
        else
        {
            vector<char> foundChars = {};
            while (k < fenSize && fen[k] != ' ')
            {
                if (!(fen[k] == 'K' || fen[k] == 'Q' || fen[k] == 'k' || fen[k] == 'q'))
                    throw runtime_error("invalid castling info inside initializeFromFenString() function");

                //*if one castling info is repeated
                for (char c : foundChars)
                    if (c == fen[k])
                        throw runtime_error("one castling info repeated inside initializeFromFenString() function");

                foundChars.push_back(fen[k]);

                switch (fen[k])
                {
                case 'K':
                    this->castlingRights[0] = true;
                    break;
                case 'Q':
                    this->castlingRights[1] = true;
                    break;
                case 'k':
                    this->castlingRights[2] = true;
                    break;
                case 'q':
                    this->castlingRights[3] = true;
                    break;

                default:
                    throw runtime_error("invalid castling info inside initializeFromFenString() function");
                }

                //*increment k
                k++;
            }

            if (k >= fenSize)
                throw runtime_error("problem on or after castling rights inside initializeFromFenString() function");
        }

        //*ignore the white space
        if (k >= fenSize || fen[k] != ' ')
            throw runtime_error(" problem initializeFromFenString() function");
        //*increment
        k++;

        //*check enPassant_________________________________________________________
        if (k >= fenSize)
            throw runtime_error(" no enpassanti inside initializeFromFenString() function");

        if (fen[k] == '-')
        {
            this->enPassantSquareIndex = -1;
            k++;
        }
        else
        {
            char enPassantFile = fen[k];
            k++;
            if (k >= fenSize)
                throw runtime_error(" no enpassanti rank inside initializeFromFenString() function");

            int enPassantRank = fen[k] - '0';
            this->enPassantSquareIndex = (8 - enPassantRank) * 8 + (enPassantFile - 'a');
            if (this->enPassantSquareIndex < 0 || this->enPassantSquareIndex >= 64)
                throw runtime_error(" invalid enpassanti coordinate inside initializeFromFenString() function");

            k++;
        }

        //*TODO: HAlf move and full move remaining
    }

    //*---------------------------------friend functions------------------------------------
};

// void initializeLeaperPieceAttackTables()
// {
//     initializePawnAttackTables();
// }

//*must be called at begining
void initializeTables()
{
    initializeSliderPieceAttackTables();
}

int main()
{
    cout << "\n\nHello earth\n\n\n";

    //*---------------IMPORTANT----------------------------------------*//
    initializeTables();
    //*---------------IMPORTANT----------------------------------------*//

    // BitBoard blockers;
    // blockers.setBitAt(a1);
    // blockers.setBitAt(c3);
    // blockers.setBitAt(d6);
    // blockers.setBitAt(d8);
    // blockers.setBitAt(g2);
    // blockers.setBitAt(g7);
    // blockers.setBitAt(h1);
    // blockers.setBitAt(h2);
    // blockers.setBitAt(f6);
    // blockers.setBitAt(d2);
    // blockers.setBitAt(d8);
    // blockers.setBitAt(b4);
    // blockers.setBitAt(a4);
    // blockers.setBitAt(d3);
    // blockers.setBitAt(a7);

    // blockers.print();

    // // getBishopAttacks(d4, blockers).print();
    // // getRookAttacks(d4, blockers).print();
    // getQueenAttacks(d4, blockers).print();

    Board b;

    b.initializeFromFenString("rn2kbnr/ppp1pppp/3q4/3p4/3P2b1/3Q1N1P/PPP1PPP1/RNB1KB1R b Kq - 0 1");
    b.print();

    // cout << "\n\nwhite occupancy :\n";
    // b.getWhiteOccupancyBitBoard().print();

    // cout << "\n\nBlack occupancy :\n";
    // b.getBlackOccupancyBitBoard().print();

    // cout << "\n\nAll occupancy :\n";
    // b.getAllOccupancyBitBoard().print();

    return 0;
}
