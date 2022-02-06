// Name: SOURASHIS MONDAL
#include "neededHeaders.h"

using namespace std;

#include "definitions.h"

#include "BitBoard.h"
#include "PieceClass.h"
#include "Board.h"
#include "engine.h"

//*helper functions
#include "helperFunctions.h"

#define START_POSITION_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0"
#define PRUNING_TEST_POSITION "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 "
#define TRICKY_POSITION_FEN "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
#define KILLER_MOVE_TEST_POSITION "6k1/3q1pp1/pp5p/1r5n/8/1P3PP1/PQ4BP/2R3K1 w - - 0 1"
#define KILLER_MOVE_TEST_POSITION1 "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"

//*must be called at begining
void initializeTables()
{
    // initializeSliderPieceAttackTables();
    BitBoard tempBitBoardToInitializeLookupTables;
}

//*----------------------------------------------------------GLOBAL VARIABLE--------------------------------------------------------------------------*//
//*----------------------------------------------------------GLOBAL VARIABLE--------------------------------------------------------------------------*//
//*----------------------------------------------------------GLOBAL VARIABLE--------------------------------------------------------------------------*//
Engine engine(START_POSITION_FEN);
//*----------------------------------------------------------GLOBAL VARIABLE--------------------------------------------------------------------------*//
//*----------------------------------------------------------GLOBAL VARIABLE--------------------------------------------------------------------------*//

//*----------------------------------------FUNCTIONS FOR UCI----------------------------------------------------------------------------------*//

// //* returns move object from moveString(algebric notation) (e.g. "e7e8q")
Move parseMove(const string &moveString)
{
    MoveList moveList;

    int currPlayer = engine.getCurrentPlayer();

    // generate moves
    engine.generateAllPseudoLegalMovesOfGivenPlayer(currPlayer, moveList);

    int fromSquare = (moveString[0] - 'a') + (8 - (moveString[1] - '0')) * 8;
    int toSquare = (moveString[2] - 'a') + (8 - (moveString[3] - '0')) * 8;

    //*check promotion
    int promotedPiece = -1;
    if (moveString.size() > 4 && moveString[4] != ' ')
    {
        switch (moveString[4])
        {
        case 'q':
            promotedPiece = currPlayer == WHITE ? Piece::Q : Piece::q;
            break;

        case 'r':
            promotedPiece = currPlayer == WHITE ? Piece::R : Piece::r;
            break;

        case 'b':
            promotedPiece = currPlayer == WHITE ? Piece::B : Piece::b;
            break;

        case 'n':
            promotedPiece = currPlayer == WHITE ? Piece::N : Piece::n;
            break;

        default:
            break;
        }
    }

    int moveListSize = moveList.size();
    //*check if the move is valid
    for (int i = 0; i < moveListSize; i++)
    {
        Move move = moveList[i];

        if (move.fromSquare == fromSquare && move.toSquare == toSquare && move.promotedPiece == promotedPiece)
            return move;
    }

    // *illegal move
    // throw runtime_error("\nillegal move inside parseMove() function\n\n");
    return Move::INVALID_MOVE;
}

/*
    Example UCI commands to init position on chess board

    -- init start position
    position startpos

    -- init start position and make the moves on chess board
    position startpos moves e2e4 e7e5

    -- init position from FEN string
    position fen r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1

    -- init position from fen string and make moves on chess board
    position fen r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 moves e2a6 e8g8
*/

// *parse UCI "position" command
void parsePosition(const string &command)
{
    //* shift pointer to the right where next token begins after "position "
    int i = 9;

    // *parse UCI "startpos" command
    if (command.substr(i, 9) == "startpos ")
    {
        engine.initializeFromFenString(START_POSITION_FEN);
    }
    //* parse UCI "fen" command
    else
    {
        int fenPos = command.find("fen");

        //* there is no fen token
        if (fenPos == string::npos)
        {
            engine.initializeFromFenString(START_POSITION_FEN);
        }
        //* found fen
        else
        {
            engine.initializeFromFenString(command.substr(fenPos + 4, command.size() - fenPos - 4));
        }
    }

    // *parse moves after position
    int movePos = command.find("moves");

    // moves available
    if (movePos != string::npos)
    {
        // *shift pointer to the right where next token begins
        int nextMovePos = movePos + 6;

        // loop over moves within a move string
        while (nextMovePos < command.size())
        {
            // *parse next move
            Move move = parseMove(command.substr(nextMovePos, command.size() - nextMovePos));

            // *if no move
            if (move == Move::INVALID_MOVE)
                break;

            // *make move on the chess board
            engine.makeMove(move);

            // *move current character mointer to the end of current move
            while (nextMovePos < command.size() && command[nextMovePos] != ' ')
                nextMovePos++;

            // *go to the next move
            nextMovePos++;
        }
    }

    engine.print();
}

/*
    Example UCI commands to make engine search for the best move

    --fixed depth search
    go depth 6
*/

//* parse UCI "go" command
void parseGo(const string &command)
{
    //*default
    int depth = 5;

    int depthPos = command.find("depth ");

    if (depthPos != string::npos)
        depth = stoi(command.substr(depthPos + 6, command.size() - depthPos - 6));

    // *search position for best move
    engine.searchPosition(depth);
}

/*
 *GUI -> isready
 *Engine -> readyok
 *GUI -> ucinewgame
 */

//*TODO: refactoring
// main UCI loop
void uciLoop()
{
    // *reset STDIN & STDOUT buffers
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    // define user / GUI input buffer
    char input[200];

    // print engine info
    cout << "id name BlueBetaFish\n";
    cout << "id name Sourashis Mondal\n";
    cout << "uciok\n";

    // main loop
    while (1)
    {
        // reset user /GUI input
        memset(input, 0, sizeof(input));

        // make sure output reaches the GUI
        fflush(stdout);

        // get user / GUI input
        if (!fgets(input, 2000, stdin))
            // continue the loop
            continue;

        // make sure input is available
        if (input[0] == '\n')
            // continue the loop
            continue;

        // parse UCI "isready" command
        if (strncmp(input, "isready", 7) == 0)
        {
            cout << "readyok\n";
            continue;
        }

        // parse UCI "position" command
        else if (strncmp(input, "position", 8) == 0)
            // call parse position function
            parsePosition(input);

        // parse UCI "ucinewgame" command
        else if (strncmp(input, "ucinewgame", 10) == 0)
            // call parse position function
            parsePosition("position startpos");

        // parse UCI "go" command
        else if (strncmp(input, "go", 2) == 0)
            // call parse go function
            parseGo(input);

        // parse UCI "quit" command
        else if (strncmp(input, "quit", 4) == 0)
            // quit from the chess engine program execution
            break;

        // parse UCI "uci" command
        else if (strncmp(input, "uci", 3) == 0)
        {
            // print engine info
            cout << "id name BlueBetaFish\n";
            cout << "id name Sourashis Mondal\n";
            cout << "uciok\n";
        }
    }
}

int main()
{
    cout << "\n\nHello earth\n\n\n";

    //*---------------IMPORTANT----------------------------------------*//
    initializeTables();
    //*---------------IMPORTANT----------------------------------------*//

    bool debugMode = true;

    if (debugMode)
    {

        Engine engine(TRICKY_POSITION_FEN);

        cout << "\n\nBoard : \n";
        engine.printBoard();

        int depthLimit = 7;
        // cout << "\nEnter depthLimit : ";
        // cin >> depthLimit;

        // long long numberOfLeaves = engine.perft_test(depthLimit);
        cout << "\n\n----------------------------------------------------------------------------\n\n";

        // {
        //     long startTime = Engine::getTimeInMilliSeconds();
        //     engine.searchPosition(depthLimit);
        //     long endTime = Engine::getTimeInMilliSeconds();
        //     cout << "\n\nTime required for normal search:       " << (endTime - startTime) << " ms";
        //     cout << "\n\n----------------------------------------------------------------------------\n\n";
        // }

        {
            long long startTime = Engine::getTimeInMilliSeconds();
            engine.searchPositionIterativeDeepening(depthLimit);
            long long endTime = Engine::getTimeInMilliSeconds();
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
    }
    else
    {
        uciLoop();
    }

    return 0;
}
