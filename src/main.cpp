// Name: SOURASHIS MONDAL
#include "neededHeaders.h"

using namespace std;

#include "definitions.h"

#include "BitBoard.h"
#include "PieceClass.h"
#include "Board.h"
#include "engine.h"
#include "uci.h"

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

    bool debugMode = false;

    if (debugMode)
    {

        Engine engine(TRICKY_POSITION_FEN);

        cout << "\n\nBoard : \n";
        engine.print();


        int depthLimit = 7;
        // cout << "\nEnter depthLimit : ";
        // cin >> depthLimit;

        // long long numberOfLeaves = engine.perft_test(depthLimit);
        cout << "\n\n----------------------------------------------------------------------------\n\n";

        {
            long startTime = getTimeInMilliSeconds();
            engine.searchPosition(depthLimit);
            long endTime = getTimeInMilliSeconds();
            cout << "\n\nTime required for normal search:       " << (endTime - startTime) << " ms";
            cout << "\n\n----------------------------------------------------------------------------\n\n";
        }

        {
            long long startTime = getTimeInMilliSeconds();
            engine.searchPositionIterativeDeepening(depthLimit);
            long long endTime = getTimeInMilliSeconds();
            cout << "\n\nTime required for iterative deepening: " << (endTime - startTime) << " ms";
        }
        cout << "\n\n----------------------------------------------------------------------------\n\n";

        // MoveList moveList;
        // engine.generateAllPseudoLegalMovesOfGivenPlayer(engine.getCurrentPlayer(), moveList);
        // engine.sortMoveList(moveList);

        // for (int i = 0; i < moveList.size(); i++)
        // {
        //     int moveScore = engine.getMoveScore(moveList[i]);
        //     moveList[i].print();
        //     cout << "MoveScore : " << moveScore << endl;
        // }

        cout << endl
             << endl
             << endl;

        cout << "\n=================================================END================================================================\n\n";
    }
    else
    {
        UCI uciEngine;
        uciEngine.uciLoop();
    }

    return 0;
}
