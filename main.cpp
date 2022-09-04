// Name: SOURASHIS MONDAL
#include "neededHeaders.h"

using namespace std;

#include "definitions.h"

#include "BitBoard.h"
#include "PieceClass.h"
#include "Board.h"
#include "engine.h"
#include "uci.h"
#include "debug.h"

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
    // cout << "\n\nHello earth\n\n\n";

    //*---------------IMPORTANT----------------------------------------*//
    initializeTables();
    //*---------------IMPORTANT----------------------------------------*//

    bool debugMode = true;

    if (debugMode)
    {
        /*
Input:
position fen qrb5/rk1p2K1/p2P4/Pp6/1N2n3/6p1/5nB1/6b1 w - - 0 1
go depth 5
Output:
bestmove b4d3

Input:
position fen r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1
go depth 5
Output:
bestmove c4c5


Input:
position fen 5r2/8/1R6/ppk3p1/2N3P1/P4b2/1K6/5B2 w - - 0 1
go depth 5
Output:
bestmove c4e5


Input:
position fen 4kb1r/p2n1ppp/4q3/4p1B1/4P3/1Q6/PPP2PPP/2KR4 w k - 1 0
go depth 5
Output:
bestmove b3b8


Input:
position fen 2r2rk1/pp1bqpp1/2nppn1p/2p3N1/1bP5/1PN3P1/PBQPPPBP/3R1RK1 w - - 0 1
go depth 7
Output:
bestmove g5f3

        */
        // define user / GUI input buffer
        char input[200];

        Debug uci;
        // *reset STDIN & STDOUT buffers
        setbuf(stdin, NULL);
        setbuf(stdout, NULL);
        fgets(input, 2000, stdin);
        uci.parsePosition(input);

        // *reset STDIN & STDOUT buffers
        setbuf(stdin, NULL);
        setbuf(stdout, NULL);
        fgets(input, 2000, stdin);
        uci.parseGo(input);
     
    }
    else
    {
        UCI uciEngine;
        uciEngine.uciLoop();
    }

    return 0;
}
