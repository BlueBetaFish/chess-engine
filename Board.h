
#pragma once

#include <bits/stdc++.h>
using namespace std;

#include "definitions.h"
#include "BitBoard.h"
#include "PieceClass.h"
#include "Move.h"

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

    int getCurrentPlayer()
    {
        return this->currentPlayer;
    }

    // get the piece at given square
    int inline getPieceAtSquare(int squareIndex)
    {
        for (int piece = Piece::P; piece <= Piece::k; piece++)
        {
            if (this->pieceBitBoards[piece].getBitAt(squareIndex) == 1)
                return piece;
        }
        return Piece::EMPTY_PIECE;
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
        cout << "EnPassant square = " << this->enPassantSquareIndex << (this->enPassantSquareIndex != -1 ? " (" + BitBoard::getAlgebraicCoordinateFromIndex(this->enPassantSquareIndex) + ")" : "") << endl;

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

        //*check enPassant square validity
        if (this->enPassantSquareIndex != -1)
        {
            if (this->currentPlayer == BLACK)
            {
                if (BitBoard::getRankOfSquareIndex(this->enPassantSquareIndex) != 3)
                    throw runtime_error("\n\ninvalid enPassant square given in fen string -- from Board.initializeFromFenString()\n\n");

                //*if there is no pawn in front of enPassant square
                if (this->pieceBitBoards[Piece::P].getBitAt(this->enPassantSquareIndex - 8) == 0)
                    throw runtime_error("\n\ninvalid enPassant square given in fen string -- from Board.initializeFromFenString()\n\n");
            }
            else if (this->currentPlayer == WHITE)
            {
                if (BitBoard::getRankOfSquareIndex(this->enPassantSquareIndex) != 6)
                    throw runtime_error("\n\ninvalid enPassant square given in fen string -- from Board.initializeFromFenString()\n\n");

                //*if there is no pawn in front of enPassant square
                if (this->pieceBitBoards[Piece::p].getBitAt(this->enPassantSquareIndex + 8) == 0)
                    throw runtime_error("\n\ninvalid enPassant square given in fen string -- from Board.initializeFromFenString()\n\n");
            }
        }

        //*TODO: HAlf move and full move remaining
    }

    //*checks if given square is attacked by given player(pseudo legal moves)
    /*
     *Efficient Method to check :---------- if a square is attacked by a white bishop , then if a black bishop is on the square, it can also attack the white bishop
     *So intuition is to get attack bitboards of all pieces at the given square of the given player and check if the same type of opposite player's
     *piece is on any of the attackecd square, if yes, then that opposite piece can attack the square :)
     */
    bool inline isGivenSquareAttackedByGivenPlayer(int squareIndex, int attackerPlayer)
    {
        int oppositePlayer = Piece::getOppositeColor(attackerPlayer);

        //* check pawn attacks
        //*if there is any opposite color pawn on the square where currrent color pawn can attack, then that opposite pawn can also attack this current color pawn at given square
        if (BitBoard::getPawnAttacks(squareIndex, oppositePlayer).getDecimalValue() & (attackerPlayer == WHITE ? this->pieceBitBoards[Piece::P].getDecimalValue() : this->pieceBitBoards[Piece::p].getDecimalValue()))
            return true;

        //*check knight attacks
        if (BitBoard::getKnightAttacks(squareIndex).getDecimalValue() & (attackerPlayer == WHITE ? this->pieceBitBoards[Piece::N].getDecimalValue() : this->pieceBitBoards[Piece::n].getDecimalValue()))
            return true;

        //*check king attacks
        if (BitBoard::getKingAttacks(squareIndex).getDecimalValue() & (attackerPlayer == WHITE ? this->pieceBitBoards[Piece::K].getDecimalValue() : this->pieceBitBoards[Piece::k].getDecimalValue()))
            return true;

        BitBoard allBlockers = this->getAllOccupancyBitBoard();

        //*check bishop attacks
        if (BitBoard::getBishopAttacks(squareIndex, allBlockers).getDecimalValue() & (attackerPlayer == WHITE ? this->pieceBitBoards[Piece::B].getDecimalValue() : this->pieceBitBoards[Piece::b].getDecimalValue()))
            return true;

        //*check rook attacks
        if (BitBoard::getRookAttacks(squareIndex, allBlockers).getDecimalValue() & (attackerPlayer == WHITE ? this->pieceBitBoards[Piece::R].getDecimalValue() : this->pieceBitBoards[Piece::r].getDecimalValue()))
            return true;

        //*TODO: no need to check queen attacks , as they are covered within rook and bishop . Comment it later
        //*check queen attacks
        if (BitBoard::getQueenAttacks(squareIndex, allBlockers).getDecimalValue() & (attackerPlayer == WHITE ? this->pieceBitBoards[Piece::Q].getDecimalValue() : this->pieceBitBoards[Piece::q].getDecimalValue()))
            return true;

        //*default
        return false;
    }

    //*unoptimized
    BitBoard getAllAttackedSquaresByGivenPlayer(int attackerPlayer)
    {
        BitBoard res;
        for (int squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            if (this->isGivenSquareAttackedByGivenPlayer(squareIndex, attackerPlayer))
                res.setBitAt(squareIndex);
        }

        return res;
    }

    //*TODO: handle return of Move object list , and create Move class later
    vector<Move> generateAllPseudoLegalMovesOfGivenPlayer(int playerColor)
    {

        vector<Move> generatedMoves;

        if (playerColor != WHITE && playerColor != BLACK)
            throw runtime_error("wrong color inside generateAllMovesOfGivenPlayer() function\n");

        int fromSquare, toSquare;

        int oppositeColor = Piece::getOppositeColor(playerColor);
        BitBoard currPlayerOccupancy = playerColor == WHITE ? this->getWhiteOccupancyBitBoard() : this->getBlackOccupancyBitBoard();
        BitBoard oppositePlayerOccupancy = oppositeColor == WHITE ? this->getWhiteOccupancyBitBoard() : this->getBlackOccupancyBitBoard();
        BitBoard allOccupancy = this->getAllOccupancyBitBoard();

        //*define a copy of the piece BitBoard, because it will be modified throughout this function
        BitBoard pieceBitBoard;

        //*Define a BitBoard for attacked squares
        BitBoard attackBitBoard;

        //*-------------------------------PAWN Moves------------------------------------------------//
        if (playerColor == WHITE)
        {
            pieceBitBoard = this->pieceBitBoards[Piece::P];

            //*extract the bits of pawns, and unset them one by one to iterate all pawn positions
            while (pieceBitBoard.getDecimalValue())
            {
                int leastSignificantSetBitIndex = pieceBitBoard.getFirstLeastSignificantBitIndexFromRight();
                fromSquare = leastSignificantSetBitIndex;

                //*---------------------------------generate quiet pawn moves (pawn pushes)---------------------------------------------//
                if (BitBoard::getRankOfSquareIndex(fromSquare) == 8)
                    throw runtime_error("\nwhite pawn cant be at 8th rank---from generateALlMovesOfGivenPlayer() function\n");

                toSquare = fromSquare - 8;

                //*if to square is valid and toSquare is empty
                if (!(toSquare < a8) && allOccupancy.getBitAt(toSquare) == 0)
                {

                    //*-----------------handle promotions------------------------------------
                    if (BitBoard::getRankOfSquareIndex(fromSquare) == 7)
                    {

                        // cout << BitBoard::getAlgebraicCoordinateFromIndex(fromSquare) << BitBoard::getAlgebraicCoordinateFromIndex(toSquare) << "q ----> pawn promotion" << endl;
                        // cout << BitBoard::getAlgebraicCoordinateFromIndex(fromSquare) << BitBoard::getAlgebraicCoordinateFromIndex(toSquare) << "r ----> pawn promotion" << endl;
                        // cout << BitBoard::getAlgebraicCoordinateFromIndex(fromSquare) << BitBoard::getAlgebraicCoordinateFromIndex(toSquare) << "b ----> pawn promotion" << endl;
                        // cout << BitBoard::getAlgebraicCoordinateFromIndex(fromSquare) << BitBoard::getAlgebraicCoordinateFromIndex(toSquare) << "n ----> pawn promotion" << endl;

                        generatedMoves.push_back(Move(fromSquare, toSquare, Piece::P, Piece::Q, false, false, false, false));
                        generatedMoves.push_back(Move(fromSquare, toSquare, Piece::P, Piece::R, false, false, false, false));
                        generatedMoves.push_back(Move(fromSquare, toSquare, Piece::P, Piece::B, false, false, false, false));
                        generatedMoves.push_back(Move(fromSquare, toSquare, Piece::P, Piece::N, false, false, false, false));
                    }
                    else
                    {
                        //*--------------pawn push of one square-------------------------//
                        // cout << BitBoard::getAlgebraicCoordinateFromIndex(fromSquare) << BitBoard::getAlgebraicCoordinateFromIndex(toSquare) << " ----> pawn push" << endl;
                        generatedMoves.push_back(Move(fromSquare, toSquare, Piece::P, -1, false, false, false, false));

                        //*------------double pawn push--------------------------------------//
                        //*dont forget to update enPassant square

                        if (BitBoard::getRankOfSquareIndex(fromSquare) == 2 && allOccupancy.getBitAt(toSquare - 8) == 0)
                        {
                            // cout << BitBoard::getAlgebraicCoordinateFromIndex(fromSquare) << BitBoard::getAlgebraicCoordinateFromIndex(toSquare - 8) << " ----> double pawn push" << endl;
                            generatedMoves.push_back(Move(fromSquare, toSquare - 8, Piece::P, -1, false, true, false, false));
                        }
                    }
                }

                //*---------------------------------generate pawn capture moves---------------------------------------------//

                //*only take the square if there is a opposite color piece
                attackBitBoard = BitBoard::getPawnAttacks(fromSquare, playerColor).getDecimalValue() & oppositePlayerOccupancy.getDecimalValue();

                //*iterate pawn attack squares
                while (attackBitBoard.getDecimalValue())
                {
                    int lestSignificantSetBitIndexOfAttackBitBoard = attackBitBoard.getFirstLeastSignificantBitIndexFromRight();
                    toSquare = lestSignificantSetBitIndexOfAttackBitBoard;

                    //*-------------------handle promotion---------------------------------------//
                    if (BitBoard::getRankOfSquareIndex(fromSquare) == 7)
                    {
                        // cout << BitBoard::getAlgebraicCoordinateFromIndex(fromSquare) << BitBoard::getAlgebraicCoordinateFromIndex(toSquare) << "q ----> pawn promotion capture" << endl;
                        // cout << BitBoard::getAlgebraicCoordinateFromIndex(fromSquare) << BitBoard::getAlgebraicCoordinateFromIndex(toSquare) << "r ----> pawn promotion capture" << endl;
                        // cout << BitBoard::getAlgebraicCoordinateFromIndex(fromSquare) << BitBoard::getAlgebraicCoordinateFromIndex(toSquare) << "b ----> pawn promotion capture" << endl;
                        // cout << BitBoard::getAlgebraicCoordinateFromIndex(fromSquare) << BitBoard::getAlgebraicCoordinateFromIndex(toSquare) << "n ----> pawn promotion capture" << endl;

                        generatedMoves.push_back(Move(fromSquare, toSquare, Piece::P, Piece::Q, true, false, false, false));
                        generatedMoves.push_back(Move(fromSquare, toSquare, Piece::P, Piece::R, true, false, false, false));
                        generatedMoves.push_back(Move(fromSquare, toSquare, Piece::P, Piece::B, true, false, false, false));
                        generatedMoves.push_back(Move(fromSquare, toSquare, Piece::P, Piece::N, true, false, false, false));
                    }
                    else
                    {
                        //*normal capture without promotion
                        // cout << BitBoard::getAlgebraicCoordinateFromIndex(fromSquare) << BitBoard::getAlgebraicCoordinateFromIndex(toSquare) << "  ----> pawn capture" << endl;
                        generatedMoves.push_back(Move(fromSquare, toSquare, Piece::P, -1, true, false, false, false));
                    }

                    //*pop the bit
                    attackBitBoard.unsetBitAt(lestSignificantSetBitIndexOfAttackBitBoard);
                }

                //*----------handle enPassant Capture------------------------------
                //*also make sure that the enPassant square was created by other player
                if (this->enPassantSquareIndex != -1 && this->currentPlayer == playerColor)
                {
                    BitBoard attackBitBoard = BitBoard::getPawnAttacks(fromSquare, playerColor);

                    if (attackBitBoard.getBitAt(this->enPassantSquareIndex) == 1)
                    {
                        toSquare = this->enPassantSquareIndex;
                        // cout << BitBoard::getAlgebraicCoordinateFromIndex(fromSquare) << BitBoard::getAlgebraicCoordinateFromIndex(toSquare) << "  ----> pawn enPassant capture" << endl;
                        generatedMoves.push_back(Move(fromSquare, toSquare, Piece::P, -1, true, false, true, false));
                    }
                }

                //*Pop the lease significant set bit , to get the next set bit in next iteration
                pieceBitBoard.unsetBitAt(leastSignificantSetBitIndex);
            }
        }
        else if (playerColor == BLACK)
        {
            pieceBitBoard = this->pieceBitBoards[Piece::p];

            //*extract the bits of pawns, and unset them one by one to iterate all pawn positions
            while (pieceBitBoard.getDecimalValue())
            {
                int leastSignificantSetBitIndex = pieceBitBoard.getFirstLeastSignificantBitIndexFromRight();
                fromSquare = leastSignificantSetBitIndex;

                //*---------------------------------generate quiet pawn moves (pawn pushes)---------------------------------------------//
                if (BitBoard::getRankOfSquareIndex(fromSquare) == 1)
                    throw runtime_error("\nblack pawn cant be at 1st rank---from generateALlMovesOfGivenPlayer() function\n");

                toSquare = fromSquare + 8;

                //*if to square is valid and toSquare is empty
                if (!(toSquare > h1) && allOccupancy.getBitAt(toSquare) == 0)
                {

                    //*-----------------handle promotions------------------------------------
                    if (BitBoard::getRankOfSquareIndex(fromSquare) == 2)
                    {
                        // cout << BitBoard::getAlgebraicCoordinateFromIndex(fromSquare) << BitBoard::getAlgebraicCoordinateFromIndex(toSquare) << "q ----> pawn promotion" << endl;
                        // cout << BitBoard::getAlgebraicCoordinateFromIndex(fromSquare) << BitBoard::getAlgebraicCoordinateFromIndex(toSquare) << "r ----> pawn promotion" << endl;
                        // cout << BitBoard::getAlgebraicCoordinateFromIndex(fromSquare) << BitBoard::getAlgebraicCoordinateFromIndex(toSquare) << "b ----> pawn promotion" << endl;
                        // cout << BitBoard::getAlgebraicCoordinateFromIndex(fromSquare) << BitBoard::getAlgebraicCoordinateFromIndex(toSquare) << "n ----> pawn promotion" << endl;

                        generatedMoves.push_back(Move(fromSquare, toSquare, Piece::p, Piece::q, false, false, false, false));
                        generatedMoves.push_back(Move(fromSquare, toSquare, Piece::p, Piece::r, false, false, false, false));
                        generatedMoves.push_back(Move(fromSquare, toSquare, Piece::p, Piece::b, false, false, false, false));
                        generatedMoves.push_back(Move(fromSquare, toSquare, Piece::p, Piece::n, false, false, false, false));
                    }
                    else
                    {
                        //*--------------pawn push of one square-------------------------//
                        // cout << BitBoard::getAlgebraicCoordinateFromIndex(fromSquare) << BitBoard::getAlgebraicCoordinateFromIndex(toSquare) << " ----> pawn push" << endl;
                        generatedMoves.push_back(Move(fromSquare, toSquare, Piece::p, -1, false, false, false, false));

                        //*------------double pawn push--------------------------------------//
                        //*dont forget to update enPassant square
                        if (BitBoard::getRankOfSquareIndex(fromSquare) == 7 && allOccupancy.getBitAt(toSquare + 8) == 0)
                        {

                            // cout << BitBoard::getAlgebraicCoordinateFromIndex(fromSquare) << BitBoard::getAlgebraicCoordinateFromIndex(toSquare + 8) << " ----> double pawn push" << endl;
                            generatedMoves.push_back(Move(fromSquare, toSquare + 8, Piece::p, -1, false, true, false, false));
                        }
                    }
                }

                //*---------------------------------generate pawn capture moves---------------------------------------------//

                //*only take the square if there is a opposite color piece
                attackBitBoard = BitBoard(BitBoard::getPawnAttacks(fromSquare, playerColor).getDecimalValue() & oppositePlayerOccupancy.getDecimalValue());

                //*iterate pawn attack squares
                while (attackBitBoard.getDecimalValue())
                {
                    int lestSignificantSetBitIndexOfAttackBitBoard = attackBitBoard.getFirstLeastSignificantBitIndexFromRight();
                    toSquare = lestSignificantSetBitIndexOfAttackBitBoard;

                    //*-------------------handle promotion---------------------------------------//
                    if (BitBoard::getRankOfSquareIndex(fromSquare) == 2)
                    {

                        // cout << BitBoard::getAlgebraicCoordinateFromIndex(fromSquare) << BitBoard::getAlgebraicCoordinateFromIndex(toSquare) << "q ----> pawn promotion capture" << endl;
                        // cout << BitBoard::getAlgebraicCoordinateFromIndex(fromSquare) << BitBoard::getAlgebraicCoordinateFromIndex(toSquare) << "r ----> pawn promotion capture" << endl;
                        // cout << BitBoard::getAlgebraicCoordinateFromIndex(fromSquare) << BitBoard::getAlgebraicCoordinateFromIndex(toSquare) << "b ----> pawn promotion capture" << endl;
                        // cout << BitBoard::getAlgebraicCoordinateFromIndex(fromSquare) << BitBoard::getAlgebraicCoordinateFromIndex(toSquare) << "n ----> pawn promotion capture" << endl;

                        generatedMoves.push_back(Move(fromSquare, toSquare, Piece::p, Piece::q, true, false, false, false));
                        generatedMoves.push_back(Move(fromSquare, toSquare, Piece::p, Piece::r, true, false, false, false));
                        generatedMoves.push_back(Move(fromSquare, toSquare, Piece::p, Piece::b, true, false, false, false));
                        generatedMoves.push_back(Move(fromSquare, toSquare, Piece::p, Piece::n, true, false, false, false));
                    }
                    else
                    {
                        //*normal capture without promotion
                        // cout << BitBoard::getAlgebraicCoordinateFromIndex(fromSquare) << BitBoard::getAlgebraicCoordinateFromIndex(toSquare) << "  ----> pawn capture" << endl;
                        generatedMoves.push_back(Move(fromSquare, toSquare, Piece::p, -1, true, false, false, false));
                    }

                    //*pop the bit
                    attackBitBoard.unsetBitAt(lestSignificantSetBitIndexOfAttackBitBoard);
                }

                //*----------handle enPassant Capture------------------------------
                //*also make sure that the enPassant square was created by other player
                if (this->enPassantSquareIndex != -1 && this->currentPlayer == playerColor)
                {
                    BitBoard attackBitBoard = BitBoard::getPawnAttacks(fromSquare, playerColor);

                    if (attackBitBoard.getBitAt(this->enPassantSquareIndex) == 1)
                    {
                        toSquare = this->enPassantSquareIndex;
                        // cout << BitBoard::getAlgebraicCoordinateFromIndex(fromSquare) << BitBoard::getAlgebraicCoordinateFromIndex(toSquare) << "  ----> pawn enPassant capture" << endl;
                        generatedMoves.push_back(Move(fromSquare, toSquare, Piece::p, -1, true, false, true, false));
                    }
                }

                //*Pop the lease significant set bit , to get the next set bit in next iteration
                pieceBitBoard.unsetBitAt(leastSignificantSetBitIndex);
            }
        }

        //*-----------------------------------------------Castling Moves---------------------------------------------------------------------*//
        if (playerColor == WHITE)
        {
            //*whtie king position
            fromSquare = e1;

            //*king side castling
            if (this->castlingRights[0])
            {
                //* if squares between king and king side rook are empty
                if (allOccupancy.getBitAt(f1) == 0 && allOccupancy.getBitAt(g1) == 0)
                {
                    //*if king is not in check and squares between rook and king are not attacked by enemy pieces
                    if (!this->isGivenSquareAttackedByGivenPlayer(e1, oppositeColor) && !this->isGivenSquareAttackedByGivenPlayer(f1, oppositeColor) && !this->isGivenSquareAttackedByGivenPlayer(g1, oppositeColor))
                    {
                        toSquare = g1;

                        // cout << BitBoard::getAlgebraicCoordinateFromIndex(fromSquare) << BitBoard::getAlgebraicCoordinateFromIndex(toSquare) << "  ----> castling move" << endl;
                        generatedMoves.push_back(Move(fromSquare, toSquare, Piece::K, -1, false, false, false, true));
                    }
                }
            }

            //*queen side castling
            if (this->castlingRights[1])
            {
                //* if squares between king and queen side rook are empty
                if (allOccupancy.getBitAt(d1) == 0 && allOccupancy.getBitAt(c1) == 0 && allOccupancy.getBitAt(b1) == 0)
                {
                    //*if king is not in check and squares between rook and king are not attacked by enemy pieces
                    if (!this->isGivenSquareAttackedByGivenPlayer(e1, oppositeColor) && !this->isGivenSquareAttackedByGivenPlayer(d1, oppositeColor) && !this->isGivenSquareAttackedByGivenPlayer(c1, oppositeColor))
                    {
                        toSquare = c1;

                        // cout << BitBoard::getAlgebraicCoordinateFromIndex(fromSquare) << BitBoard::getAlgebraicCoordinateFromIndex(toSquare) << "  ----> castling move" << endl;
                        generatedMoves.push_back(Move(fromSquare, toSquare, Piece::K, -1, false, false, false, true));
                    }
                }
            }
        }
        else if (playerColor == BLACK)
        {
            //*black king position
            fromSquare = e8;

            //*king side castling
            if (this->castlingRights[2])
            {
                //* if squares between king and king side rook are empty
                if (allOccupancy.getBitAt(f8) == 0 && allOccupancy.getBitAt(g8) == 0)
                {
                    //*if king is not in check and squares between rook and king are not attacked by enemy pieces
                    if (!this->isGivenSquareAttackedByGivenPlayer(e8, oppositeColor) && !this->isGivenSquareAttackedByGivenPlayer(f8, oppositeColor) && !this->isGivenSquareAttackedByGivenPlayer(g8, oppositeColor))
                    {
                        toSquare = g8;

                        // cout << BitBoard::getAlgebraicCoordinateFromIndex(fromSquare) << BitBoard::getAlgebraicCoordinateFromIndex(toSquare) << "  ----> castling move" << endl;
                        generatedMoves.push_back(Move(fromSquare, toSquare, Piece::k, -1, false, false, false, true));
                    }
                }
            }

            //*queen side castling
            if (this->castlingRights[3])
            {
                //* if squares between king and queen side rook are empty
                if (allOccupancy.getBitAt(d8) == 0 && allOccupancy.getBitAt(c8) == 0 && allOccupancy.getBitAt(b8) == 0)
                {
                    //*if king is not in check and squares between rook and king are not attacked by enemy pieces
                    if (!this->isGivenSquareAttackedByGivenPlayer(e8, oppositeColor) && !this->isGivenSquareAttackedByGivenPlayer(d8, oppositeColor) && !this->isGivenSquareAttackedByGivenPlayer(c8, oppositeColor))
                    {
                        toSquare = c8;

                        // cout << BitBoard::getAlgebraicCoordinateFromIndex(fromSquare) << BitBoard::getAlgebraicCoordinateFromIndex(toSquare) << "  ----> castling move" << endl;
                        generatedMoves.push_back(Move(fromSquare, toSquare, Piece::k, -1, false, false, false, true));
                    }
                }
            }
        }

        //*----------------------------------------------------KNIGHT, BISHOP, ROOK, QUEEN, KING  MOVES--------------------------------------------------------------*//
        vector<int> tempPieces = {
            Piece::N,
            Piece::B,
            Piece::R,
            Piece::Q,
            Piece::K,
        };
        for (int i = 0; i < tempPieces.size(); i++)
        {
            int currPiece = -1;
            if (playerColor == WHITE)
            {
                currPiece = tempPieces[i];
            }
            else // if black
            {
                switch (tempPieces[i])
                {
                case Piece::N:
                    currPiece = Piece::n;
                    break;
                case Piece::B:
                    currPiece = Piece::b;
                    break;
                case Piece::R:
                    currPiece = Piece::r;
                    break;
                case Piece::Q:
                    currPiece = Piece::q;
                    break;
                case Piece::K:
                    currPiece = Piece::k;
                    break;
                default:
                    break;
                }
            }

            pieceBitBoard = this->pieceBitBoards[currPiece];

            while (pieceBitBoard.getDecimalValue())
            {
                int leastSignificantSetBitIndex = pieceBitBoard.getFirstLeastSignificantBitIndexFromRight();
                fromSquare = leastSignificantSetBitIndex;

                //*make sure the toSquare of attackBitBoard does not contain a same color piece
                //*for slider pieces
                if (tempPieces[i] == Piece::B || tempPieces[i] == Piece::R || tempPieces[i] == Piece::Q)
                    attackBitBoard = BitBoard(BitBoard::getPieceAttacks(fromSquare, tempPieces[i], allOccupancy).getDecimalValue() & ~currPlayerOccupancy.getDecimalValue());
                else
                    attackBitBoard = BitBoard(BitBoard::getPieceAttacks(fromSquare, tempPieces[i]).getDecimalValue() & ~currPlayerOccupancy.getDecimalValue());

                //*iterate the tooSquares of attackBitBoard
                while (attackBitBoard.getDecimalValue())
                {
                    int leastSignificantSetBitIndexOfAttackBitBoard = attackBitBoard.getFirstLeastSignificantBitIndexFromRight();
                    toSquare = leastSignificantSetBitIndexOfAttackBitBoard;

                    //*quiet moves
                    if (oppositePlayerOccupancy.getBitAt(toSquare) == 0)
                    {
                        // cout << BitBoard::getAlgebraicCoordinateFromIndex(fromSquare) << BitBoard::getAlgebraicCoordinateFromIndex(toSquare) << "  ----> piece quiet move" << endl;
                        generatedMoves.push_back(Move(fromSquare, toSquare, currPiece, -1, false, false, false, false));
                    }
                    //*capture
                    else
                    {
                        // cout << BitBoard::getAlgebraicCoordinateFromIndex(fromSquare) << BitBoard::getAlgebraicCoordinateFromIndex(toSquare) << "  ----> piece capture move" << endl;
                        generatedMoves.push_back(Move(fromSquare, toSquare, currPiece, -1, true, false, false, false));
                    }

                    //*POP BIT
                    attackBitBoard.unsetBitAt(leastSignificantSetBitIndexOfAttackBitBoard);
                }

                //*POP THE BIT
                pieceBitBoard.unsetBitAt(leastSignificantSetBitIndex);
            }
        }

        //*return move list
        return generatedMoves;
    }

    //*---------------------------------friend functions------------------------------------
};