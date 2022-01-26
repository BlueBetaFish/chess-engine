// Name: SOURASHIS MONDAL
#include "neededHeaders.h"

using namespace std;

#include "definitions.h"

#include "BitBoard.h"
#include "PieceClass.h"
#include "Board.h"

//*helper functions
#include "helperFunctions.h"

// get time in milliseconds
int getTimeInMilliSeconds()
{
#ifdef WIN64
    return GetTickCount();
#else
    struct timeval time_value;
    gettimeofday(&time_value, NULL);
    return time_value.tv_sec * 1000 + time_value.tv_usec / 1000;
#endif
}

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

    b.initializeFromFenString("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");
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

    // // //*TODO: WRONG RESULT FOR DEPTHLIMIT 2 : DEBUG
    // for (int i = 1; i <= 5; i++)
    // {
    //     int depthLimit = i;
    //     cout << "\n\nTotal No of leaf nodes upto depth : " << depthLimit << "  =  " << b.perft_driver(depthLimit);
    // }

    long long startTime = getTimeInMilliSeconds();

    int depthLimit = 6;
    b.perft_test(depthLimit);

    long long endTime = getTimeInMilliSeconds();

    cout << "\n\nExecution time = " << (endTime - startTime) << " milliseconds ." << endl;

    return 0;
}
