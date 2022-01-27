// Name: SOURASHIS MONDAL
#include "neededHeaders.h"

using namespace std;

#include "definitions.h"

#include "BitBoard.h"
#include "PieceClass.h"
#include "Board.h"

//*helper functions
#include "helperFunctions.h"

#define START_POSITION_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - "

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

class Engine : public Board
{

public:
    Engine(string fen = START_POSITION_FEN)
    {
        this->initializeFromFenString(fen);
    }

    void printBoard()
    {
        this->print();
    }

    /*
     *   returns the number of leaf nodes upto given depth limit
     */
    long long perft_driver(int depthLimit)
    {
        return Board::perft_driver(depthLimit);
    }

    /*
     *   prints the number of leaf nodes upto given depth limit after the corresponding move
     */
    long long perft_test(int depthLimit)
    {
        long long startTime = getTimeInMilliSeconds();

        long long res = Board::perft_test(depthLimit);

        long long endTime = getTimeInMilliSeconds();
        cout << "\nTotal Number of leaves upto depth = " << depthLimit << "  =  " << res << endl;
        cout << "\n\nExecution time = " << (endTime - startTime) << " milliseconds ." << endl;

        return res;
    }

    //*TODO:
    //*find the best move upto given depth
    void searchPosition(int depth)
    {
        // this->perft_test(depth);

        vector<Move> moveList;
        this->generateAllPseudoLegalMovesOfGivenPlayer(this->getCurrentPlayer(), moveList);

        cout << "bestmove " << moveList[0].getUCIMove() << endl;
    }
};