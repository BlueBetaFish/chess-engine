#pragma once

#include "neededHeaders.h"

using namespace std;

#include "engine.h"


//*----------------------------------------------------------GLOBAL VARIABLE--------------------------------------------------------------------------*//
//*----------------------------------------------------------GLOBAL VARIABLE--------------------------------------------------------------------------*//
//*----------------------------------------------------------GLOBAL VARIABLE--------------------------------------------------------------------------*//
// Engine engine(START_POSITION_FEN);
//*----------------------------------------------------------GLOBAL VARIABLE--------------------------------------------------------------------------*//
//*----------------------------------------------------------GLOBAL VARIABLE--------------------------------------------------------------------------*//

//*----------------------------------------FUNCTIONS FOR UCI----------------------------------------------------------------------------------*//

class UCI
{
    Engine engine;

public:
    UCI()
    {
        engine.initializeFromFenString(START_POSITION_FEN);
    }

private:
    // //* returns move object from moveString(algebric notation) (e.g. "e7e8q")
    Move inline parseMove(const string &moveString)
    {
        MoveList moveList;

        int currPlayer = this->engine.getCurrentPlayer();

        // generate moves
        this->engine.generateAllPseudoLegalMovesOfGivenPlayer(currPlayer, moveList);

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
        cout<<"\n\n-------------Illegal move inside parseMove() function--------------\n\n";
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
    void inline parsePosition(const string &command)
    {
        //* shift pointer to the right where next token begins after "position "
        int i = 9;

        // *parse UCI "startpos" command
        if (command.substr(i, 9) == "startpos ")
        {
            this->engine.initializeFromFenString(START_POSITION_FEN);
        }
        //* parse UCI "fen" command
        else
        {
            int fenPos = command.find("fen");

            //* there is no fen token
            if (fenPos == string::npos)
            {
                this->engine.initializeFromFenString(START_POSITION_FEN);
            }
            //* found fen
            else
            {
                this->engine.initializeFromFenString(command.substr(fenPos + 4, command.size() - fenPos - 4));
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
                this->engine.makeMove(move);

                // *move current character mointer to the end of current move
                while (nextMovePos < command.size() && command[nextMovePos] != ' ')
                    nextMovePos++;

                // *go to the next move
                nextMovePos++;
            }
        }

        this->engine.print();
    }

    /*
        Example UCI commands to make this->engine search for the best move

        --fixed depth search
        go depth 6
    */

    // //* parse UCI "go" command
    // void inline parseGo(const string &command)
    // {
    //     int depth = -1;

    //     int depthPos = command.find("depth ");

    //     if (depthPos != string::npos)
    //         depth = stoi(command.substr(depthPos + 6, command.size() - depthPos - 6));

    //     // *search position for best move
    //     this->engine.searchPositionIterativeDeepening(depth);
    // }

    //*Copied from BlueFeverSoftware's youtube channel
    // parse UCI command "go"
    void parseGo(char *command)
    {
        

        //*Important
        engine.resetUCISearchInfo();


        // init parameters
        int depth = -1;

        // init argument
        char *argument = NULL;

        // infinite search
        if ((argument = strstr(command, "infinite")))
        {
        }

        // match UCI "binc" command
        if ((argument = strstr(command, "binc")) && engine.getCurrentPlayer() == BLACK)
            // parse black time increment
            engine.inc = atoi(argument + 5);

        // match UCI "winc" command
        if ((argument = strstr(command, "winc")) && engine.getCurrentPlayer() == WHITE)
            // parse white time increment
            engine.inc = atoi(argument + 5);

        // match UCI "wtime" command
        if ((argument = strstr(command, "wtime")) && engine.getCurrentPlayer() == WHITE)
            // parse white time limit
            engine.time = atoi(argument + 6);

        // match UCI "btime" command
        if ((argument = strstr(command, "btime")) && engine.getCurrentPlayer() == BLACK)
            // parse black time limit
            engine.time = atoi(argument + 6);

        // match UCI "movestogo" command
        if ((argument = strstr(command, "movestogo")))
            // parse number of moves to go
            engine.movestogo = atoi(argument + 10);

        // match UCI "movetime" command
        if ((argument = strstr(command, "movetime")))
            // parse amount of time allowed to spend to make a move
            engine.movetime = atoi(argument + 9);

        // match UCI "depth" command
        if ((argument = strstr(command, "depth")))
            // parse search depth
            depth = atoi(argument + 6);

        // if move time is available
        if (engine.movetime != -1)
        {
            // set time equal to move time
            engine.time = engine.movetime;

            // set moves to go to 1
            engine.movestogo = 1;
        }

        engine.starttime = getTimeInMilliSeconds();

        // if time control is available
        if (engine.time != -1)
        {
            // flag we're playing with time control
            engine.timeset = 1;

            // set up timing
            engine.time /= engine.movestogo;
            engine.time -= 50;
            engine.stoptime = engine.starttime + engine.time + engine.inc;
        }

        // if depth is not available
        if (depth == -1)
            // set depth to 64 plies (takes ages to complete...)
            depth = 64;

        /*
        *   time : time limit for the search
        *   start : start time in millisecond
        *   stop : stop time in millisecond
        *   depth : upto which depth the engine will search
        *   timeset : flag to denote if the engine is searching for specific time limit or depth limit
        */
        // print debug info
        printf("time:%d start:%d stop:%d depth:%d timeset:%d\n",
               engine.time, engine.starttime, engine.stoptime, depth, engine.timeset);


        //*error
        if(engine.timeset && engine.starttime > engine.stoptime)
        {
            cout<<"\n\n----------------------------engine.starttime > engine.stoptime-----------------------\n\n";
            return;
        }               

        // *search position for best move
        this->engine.searchPositionIterativeDeepening(depth);
    }

    /*
     *GUI -> isready
     *this->Engine -> readyok
     *GUI -> ucinewgame
     */

public:
    //*TODO: refactoring
    // main UCI loop
    void inline uciLoop()
    {
        // *reset STDIN & STDOUT buffers
        setbuf(stdin, NULL);
        setbuf(stdout, NULL);

        // define user / GUI input buffer
        char input[200];

        // print this->engine info
        cout << "id name BlueBetaFish\n";
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
                // quit from the chess this->engine program execution
                break;

            // parse UCI "uci" command
            else if (strncmp(input, "uci", 3) == 0)
            {
                // print this->engine info
                cout << "id name BlueBetaFish\n";
                cout << "uciok\n";
            }
        }
    }
};