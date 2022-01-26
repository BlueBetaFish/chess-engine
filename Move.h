
#pragma once
#include <iostream>
#include <vector>

#include "BitBoard.h"
#include "PieceClass.h"

using namespace std;

class Move
{
public:
    int fromSquare;
    int toSquare;
    int pieceMoved;
    int promotedPiece;

    bool captureFlag;
    bool doublePawnPushFlag;
    bool enPassantFlag;
    bool castleFlag;

    /*
        if there is no promoted piece, then promotedPiece = -1
    */
    Move(int fromSquare, int toSquare, int pieceMoved, int promotedPiece, bool captureFlag, bool doublePawnPushFlag, bool enPassantFlag, bool castleFlag)
    {
        this->fromSquare = fromSquare;
        this->toSquare = toSquare;
        this->pieceMoved = pieceMoved;
        this->promotedPiece = promotedPiece;
        this->captureFlag = captureFlag;
        this->doublePawnPushFlag = doublePawnPushFlag;
        this->enPassantFlag = enPassantFlag;
        this->castleFlag = castleFlag;
    }

    void inline print()
    {
        cout << BitBoard::getAlgebraicCoordinateFromIndex(this->fromSquare) << BitBoard::getAlgebraicCoordinateFromIndex(this->toSquare);
        cout << "   ";
        cout << Piece::getASCIICodeOfPiece(this->pieceMoved);
        cout << "       ";

        if (this->promotedPiece == -1)
            cout << "-1";
        else
            cout << " " << Piece::getASCIICodeOfPiece(this->promotedPiece);

        cout << "         " << this->captureFlag;
        cout << "         ";
        cout << this->doublePawnPushFlag;
        cout << "            ";
        cout << this->enPassantFlag;
        cout << "           ";
        cout << this->castleFlag;
        cout << endl;
    }

    static void inline printMoveList(const vector<Move> &moveList)
    {
        cout << "\n     Move list :\n";
        cout << "     Total No. of Moves : " << moveList.size() << endl;

        cout << "---------------------------------------------------------------------" << endl;
        cout << "Move---Piece---Promoted---Capture---DoublePush---EnPassant---Castle" << endl;
        cout << "---------------------------------------------------------------------" << endl;

        for (auto move : moveList)
            move.print();
    }
};