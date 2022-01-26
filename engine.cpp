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

    b.initializeFromFenString("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10   ");
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

    // vector<Move> moveList = b.generateAllPseudoLegalMovesOfGivenPlayer(b.getCurrentPlayer());
    // Move::printMoveList(moveList);

    // //*TODO: WRONG RESULT FOR DEPTHLIMIT 2 : DEBUG
    for (int i = 1; i <= 5; i++)
    {
        int depthLimit = i;
        cout << "\n\nTotal No of leaf nodes upto depth : " << depthLimit << "  =  " << b.perft_driver(depthLimit);
    }

    // int depthLimit = 5;
    // b.perft_test(depthLimit);

    return 0;
}
