// Name: SOURASHIS MONDAL

#include <bits/stdc++.h>

using namespace std;

#include "definitions.h"

#include "BitBoard.h"
#include "PieceClass.h"
#include "Board.h"

//*helper functions
#include "helperFunctions.h"

//*must be called at begining
void initializeTables()
{

    // initializeSliderPieceAttackTables();
    BitBoard tempBitBoardToInitializeLookupTables;
}

int main()
{
    cout << "\n\nHello earth\n\n\n";

    //*---------------IMPORTANT----------------------------------------*//
    initializeTables();
    //*---------------IMPORTANT----------------------------------------*//

    BitBoard blockers;
    blockers.setBitAt(a1);
    blockers.setBitAt(c3);
    blockers.setBitAt(d6);
    blockers.setBitAt(d8);
    blockers.setBitAt(g2);
    blockers.setBitAt(g7);
    blockers.setBitAt(h1);
    blockers.setBitAt(h2);
    blockers.setBitAt(f6);
    blockers.setBitAt(d2);
    blockers.setBitAt(d8);
    blockers.setBitAt(b4);
    blockers.setBitAt(a4);
    blockers.setBitAt(d3);
    blockers.setBitAt(a7);

    // blockers.print();

    // int squareIndex = d4;
    // cout << "\nBishop moves : \n";
    // BitBoard::getBishopAttacks(squareIndex, blockers).print();
    // cout << "\nRook moves : \n";
    // BitBoard::getRookAttacks(squareIndex, blockers).print();
    // cout << "\nqueen moves : \n";
    // BitBoard::getQueenAttacks(squareIndex, blockers).print();

    Board b;

    b.initializeFromFenString("r3k2r/p2pqpb1/bn2pnp1/2pPN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq c6 0 1 ");
    cout << "\n\nBoard : \n";
    b.print();

    // cout << "\n\nwhite occupancy :\n";
    // b.getWhiteOccupancyBitBoard().print();

    // cout << "\n\nBlack occupancy :\n";
    // b.getBlackOccupancyBitBoard().print();

    // cout << "\n\nAll occupancy :\n";
    // b.getAllOccupancyBitBoard().print();

    // cout << "\n\n\nAll attacked squares :\n";
    // b.getAllAttackedSquaresByGivenPlayer(BLACK).print();

    vector<Move> moveList = b.generateAllPseudoLegalMovesOfGivenPlayer(b.getCurrentPlayer());
    Move::printMoveList(moveList);

    return 0;
}
