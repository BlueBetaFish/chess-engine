// Name: SOURASHIS MONDAL
#include "neededHeaders.h"

using namespace std;

#include "definitions.h"

#include "BitBoard.h"
#include "PieceClass.h"
#include "Board.h"

//*helper functions
#include "helperFunctions.h"

#define MAX_DEPTH 64 //*we assume we will search upto max 64 depth

struct MinimaxReturn
{
    int bestScore;
    long long nodeCount;
};

class Engine : public Board
{
    //*bestMove after a search
    static Move BEST_MOVE;

    //*materialValue[piece]
    static int materialValue[12];

    //*positionalScore[piece][squareIndex]
    static int positionalScore[12][64];

    //*mirrorIndex[squareIndex]
    static int mirrorIndex[128];

    //*Most valuable victim , less valuable attacker score table for move ordering (stores the score of a move to order them from best to worst)
    static int MVV_LVA_MOVE_SCORE[12][12];

    //*Killer move is a dangerous quiet move which kills opponent's most of the other moves  to be expanded (example link : https://rustic-chess.org/search/ordering/killers.html)
    //*KILLER_MOVE[id][ply] ---> most of the engines store 2 killer moves for each ply for efficiency
    static Move KILLER_MOVES[2][MAX_DEPTH];

    //*SCORE OF HISTORY MOVES : //*TODO: gotta comment properly later
    //*HISTORY_MOVES[piece][square]
    static int HISTORY_MOVE_SCORE[12][64];

public:
    Engine()
    {
    }

    Engine(string fen)
    {
        this->initializeFromFenString(fen);
    }

    void inline printBoard()
    {
        this->print();
    }

    void printPositionalScores()
    {
        for (int piece = Piece::P; piece <= Piece::k; piece++)
        {
            cout << "\n\npositional score of : " << Piece::getASCIICodeOfPiece(piece) << "  :   \n";

            int k = 0;

            cout << "{\n    ";
            for (int i = 0; i < 63; i++, k++)
            {

                cout << positionalScore[piece][i];
                if (i != 63)
                    cout << ", ";

                if (k % 8 == 7)
                    cout << endl
                         << "    ";
            }

            cout << "\n}";
        }
    }

    // get time in milliseconds
    static int inline getTimeInMilliSeconds()
    {
#ifdef WIN64
        return GetTickCount();
#else
        struct timeval time_value;
        gettimeofday(&time_value, NULL);
        return time_value.tv_sec * 1000 + time_value.tv_usec / 1000;
#endif
    }

    //*retruns the relative score of a move so that we can order them from good to bad
    int inline getMoveScore(const Move &move, int ply)
    {
        //*if move is capture move , then find the MVV_LVA_MOVE_SCORE value
        if (move.captureFlag)
        {
            //*TODO: also include the capturedPiece in move class, so that we dont need to call this function to get the captured piece here
            int capturedPiece = Piece::EMPTY_PIECE;

            //*if the move was enpassant , then toSquare would be empty, so assign pawn
            if (move.enPassantFlag)
            {
                capturedPiece = this->currentPlayer == WHITE ? Piece::p : Piece::P;
            }
            else
            {
                int startPiece = -1, endPiece = -1;
                if (this->currentPlayer == WHITE)
                {
                    startPiece = Piece::p;
                    endPiece = Piece::k;
                }
                else
                {
                    startPiece = Piece::P;
                    endPiece = Piece::K;
                }

                for (int piece = startPiece; piece <= endPiece; piece++)
                {
                    if (this->pieceBitBoards[piece].getBitAt(move.toSquare) == 1)
                    {
                        capturedPiece = piece;
                        break;
                    }
                }
            }

            // cout << "\nattacker : " << Piece::getASCIICodeOfPiece(move.pieceMoved) << ", captured Piece : " << Piece::getASCIICodeOfPiece(capturedPiece);

            //*because quiet killer move gets score of 9000, so to assign captures  greater weight, add 10,000
            return 10000 + Engine::MVV_LVA_MOVE_SCORE[move.pieceMoved][capturedPiece];
        }
        //*TODO:for quiet move
        else
        {
            //*get score of 1st killer move, if exists
            if (Engine::KILLER_MOVES[0][ply] == move)
            {
                return 9000;
            }

            //*get score of 1st killer move, if exists
            if (Engine::KILLER_MOVES[1][ply] == move)
            {
                return 8000;
            }

            //*get score of history move
            return Engine::HISTORY_MOVE_SCORE[move.pieceMoved][move.toSquare];
        }

        return 0;
    }

    inline void sortMoveList(MoveList &moveList, int ply)
    {
        int moveListSize = moveList.size();

        //*store the scores of all moves
        int moveScores[moveListSize];
        for (int i = 0; i < moveListSize; i++)
            moveScores[i] = this->getMoveScore(moveList[i], ply);

        //*-----------------INSERTION SORT-------------------------------//
        for (int i = 1; i < moveListSize; i++)
        {
            int j = i - 1;
            int x = moveScores[i];
            Move m = moveList[i];

            //*shifting
            while (j >= 0 && moveScores[j] < x)
            {
                moveScores[j + 1] = moveScores[j];
                moveList[j + 1] = moveList[j];

                j--;
            }

            moveScores[j + 1] = x;
            moveList[j + 1] = m;
        }
    }

    /*
     *   returns the number of leaf nodes upto given depth limit
     */
    long long inline perft_driver(int depthLimit)
    {
        return Board::perft_driver(depthLimit);
    }

    /*
     *   prints the number of leaf nodes upto given depth limit after the corresponding move
     */
    long long inline perft_test(int depthLimit)
    {
        long long startTime = Engine::getTimeInMilliSeconds();

        long long res = Board::perft_test(depthLimit);

        long long endTime = Engine::getTimeInMilliSeconds();
        cout << "\nTotal Number of leaves upto depth = " << depthLimit << "  =  " << res << endl;
        cout << "\n\nExecution time = " << (endTime - startTime) << " milliseconds ." << endl;

        return res;
    }

    //*TODO: to be improved
    //*TODO: score is positive if current has advantage
    inline int staticEvaluation()
    {
        int score = 0;
        int ls1bIndex = -1;
        int squareIndex = -1;
        BitBoard pieceBitBoard;

        //*iterate all pieces on the board
        for (int piece = Piece::P; piece <= Piece::k; piece++)
        {

            pieceBitBoard = this->pieceBitBoards[piece];

            while (pieceBitBoard.getDecimalValue())
            {
                ls1bIndex = pieceBitBoard.getFirstLeastSignificantBitIndexFromRight();
                squareIndex = ls1bIndex;

                //*material value
                score += Engine::materialValue[piece];

                //*positional score of the piece
                if (Piece::colorOfGivenPiece(piece) == WHITE)
                    score += Engine::positionalScore[piece][squareIndex];
                else
                    score -= Engine::positionalScore[piece][squareIndex];

                //*pop the least significant 1 bit
                pieceBitBoard.unsetBitAt(ls1bIndex);
            }
        }

        return this->currentPlayer == WHITE ? score : -score;
    }

    /*
     *   Returns the max static evaluation score from the initial position until the position is quiet (i.e, there is no more captures)
     */
    MinimaxReturn inline quiescenceSearch(int alpha, int beta, int ply)
    {
        int currScore = this->staticEvaluation();

        // cout << "\nBoard pos:\n";
        // this->printBoard();

        //*TODO: codeMonkeyKing was returning beta here dunno why , check later
        if (currScore >= beta)
            return {beta, 1};

        alpha = max(alpha, currScore);

        long long nodeCount = 0;

        //*generate all pseudo legal moves
        MoveList moveList;
        this->generateAllPseudoLegalMovesOfGivenPlayer(this->currentPlayer, moveList);

        //*sort moves from good to bad for more alpha beta pruning
        this->sortMoveList(moveList, ply);

        Engine backUpCopyOfBoard = *this;

        int moveListSize = moveList.size();

        int maxScore = currScore;

        for (int i = 0; i < moveListSize; i++)
        {
            Move move = moveList[i];
            //*if making this move lets opponent capture our king , dont consider it

            //*--------------------------------------------------IMPORTANT: only make the capture moves in QUIESCENCE SEARCH--------------------------------------------------
            if (!this->makeMove(move, true))
                continue;

            MinimaxReturn currReturnedVal = this->quiescenceSearch(-beta, -alpha, ply + 1);
            int currScore = -currReturnedVal.bestScore;

            //*restore board
            *this = backUpCopyOfBoard;

            //*add new nodeCoutns
            nodeCount += currReturnedVal.nodeCount;

            if (currScore > maxScore)
            {
                maxScore = alpha = currScore;
            }

            //*TODO: codeMonkeyKing is returning beta here dunno why check alter
            if (alpha >= beta)
                return {beta, nodeCount};
        }

        //*return best score
        return {alpha, nodeCount};
    }

    MinimaxReturn inline negamax(int depthLimit, int alpha, int beta, int ply)
    {
        if (depthLimit == 0)
        {
            // return {this->staticEvaluation(), Move::INVALID_MOVE, 1};

            MinimaxReturn quiescenceSearchResult = this->quiescenceSearch(alpha, beta, ply);
            return {quiescenceSearchResult.bestScore, quiescenceSearchResult.nodeCount};
        }

        long long legalMoves = 0;

        //*TODO: move it to the section where inCheck is used, otherwise bestMove might be returned before reaching that section , and i am calling this function unnecessarily here
        bool inCheck = this->isCurrentPlayerKingInCheck();

        //*TODO: didnt understand properly , research later

        //*if current player is in check(it is an interesting position and cant be ignored) , increase search depth if the king has been exposed into a check to avoid being mated
        if (inCheck)
            depthLimit++;

        long long nodeCount = 0;

        int maxScore = INT_MIN;

        //*generate all pseudo legal moves
        MoveList moveList;
        this->generateAllPseudoLegalMovesOfGivenPlayer(this->currentPlayer, moveList);

        //*sort moves from good to bad for more alpha beta pruning
        this->sortMoveList(moveList, ply);

        Engine backUpCopyOfBoard = *this;

        int moveListSize = moveList.size();

        for (int i = 0; i < moveListSize; i++)
        {
            Move move = moveList[i];
            //*if making this move lets opponent capture our king , dont consider it
            if (!this->makeMove(move))
                continue;

            legalMoves++;

            MinimaxReturn currReturnedVal = this->negamax(depthLimit - 1, -beta, -alpha, ply + 1);
            int currScore = -currReturnedVal.bestScore;

            //*restore board
            *this = backUpCopyOfBoard;

            //*add new nodeCoutns
            nodeCount += currReturnedVal.nodeCount;

            if (currScore > maxScore)
            {
                //*if it is a quiet move, consider its HISTORY_MOVE_SCORE
                if (!move.captureFlag)
                {
                    //*store history move score
                    Engine::HISTORY_MOVE_SCORE[move.pieceMoved][move.toSquare] += depthLimit;
                }

                maxScore = alpha = currScore;

                if (ply == 0)
                    Engine::BEST_MOVE = move; //*current move is the best move
            }

            //*beta cutoff
            if (alpha >= beta)
            {
                //*for quiet move, store it as killer move
                if (!move.captureFlag)
                {
                    //*store this move as killer move, since it caused the beta cut off , and also make the prev 1st killer move as 2nd killer move
                    Engine::KILLER_MOVES[1][ply] = Engine::KILLER_MOVES[0][ply];
                    Engine::KILLER_MOVES[0][ply] = move;
                }

                return {beta, nodeCount};
            }
        }

        //*if current player doesnt have any legal moves
        if (legalMoves == 0)
        {
            //*if king is in check, then it is checkmate
            if (inCheck)
            {
                //*Important : "- depthLimit" is needed to find the nearest checkmate , becuase if there are 2 checkmates at depth 3 and depth 7 , we need to return specifically the checkmate at depth 3 , and at depth 3 depthLimit is higher than at depth 7
                return {-49000 - depthLimit, nodeCount};
            }
            //*stalemate
            else
            {
                return {0, nodeCount};
            }
        }

        //*return best move
        return {alpha, nodeCount};
    }

    static void resetTablesOfSearch()
    {
        Engine::BEST_MOVE = Move::INVALID_MOVE;

        //*reset KILLER_MOVE and HISTORY_MOVE_SCORE tables
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < MAX_DEPTH; j++)
                Engine::KILLER_MOVES[i][j] = Move::INVALID_MOVE;
        }

        for (int i = 0; i < 12; i++)
        {
            for (int j = 0; j < 64; j++)
                Engine::HISTORY_MOVE_SCORE[i][j] = 0;
        }
    }

    //*TODO:
    //*find the best move upto given depth
    void inline searchPosition(int depth)
    {
        //*IMPORTANT: Dont forget to reset the tables, otherwise middle game performance will be so much dropped
        Engine::resetTablesOfSearch();

        MinimaxReturn res = negamax(depth, -50000, 50000, 0);

        if (Engine::BEST_MOVE == Move::INVALID_MOVE)
        {
            cout << "\n\nProblem searchPostion : INVALID_MOVE returned\n\n";
            return;
        }
        else
        {
            cout << "info score cp " << res.bestScore << " depth " << depth << " nodes " << res.nodeCount << endl;
            cout << "bestmove " << Engine::BEST_MOVE.getUCIMove() << endl;
        }
    }
};

Move Engine::BEST_MOVE = Move::INVALID_MOVE;

int Engine::materialValue[12] = {
    100,    // white pawn value(P)
    300,    // white knight value(N)
    350,    // white bishop value(B)
    500,    // white rook value(R)
    1000,   // white queen value(Q)
    10000,  // white king value(K)
    -100,   // black pawn value(p)
    -300,   // black knight value(n)
    -350,   // black bishop value(b)
    -500,   // black rook value(r)
    -1000,  // black queen value(q)
    -10000, // black king value(k)
};

int Engine::positionalScore[12][64] =
    {
        //*white pawn (P) Positional score
        {
            90, 90, 90, 90, 90, 90, 90, 90,
            30, 30, 30, 40, 40, 30, 30, 30,
            20, 20, 20, 30, 30, 30, 20, 20,
            10, 10, 10, 20, 20, 10, 10, 10,
            5, 5, 10, 20, 20, 5, 5, 5,
            0, 0, 0, 5, 5, 0, 0, 0,
            0, 0, 0, -10, -10, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0

        },

        //*white knight (N) Positional score
        {
            -5, 0, 0, 0, 0, 0, 0, -5,
            -5, 0, 0, 10, 10, 0, 0, -5,
            -5, 5, 20, 20, 20, 20, 5, -5,
            -5, 10, 20, 30, 30, 20, 10, -5,
            -5, 10, 20, 30, 30, 20, 10, -5,
            -5, 5, 20, 10, 10, 20, 5, -5,
            -5, 0, 0, 0, 0, 0, 0, -5,
            -5, -10, 0, 0, 0, 0, -10, -5

        },

        //*white bishop (B) Positional score
        {
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 10, 10, 0, 0, 0,
            0, 0, 10, 20, 20, 10, 0, 0,
            0, 0, 10, 20, 20, 10, 0, 0,
            0, 10, 0, 0, 0, 0, 10, 0,
            0, 30, 0, 0, 0, 0, 30, 0,
            0, 0, -10, 0, 0, -10, 0, 0

        },

        //*white rook (R) positional score
        {
            50, 50, 50, 50, 50, 50, 50, 50,
            50, 50, 50, 50, 50, 50, 50, 50,
            0, 0, 10, 20, 20, 10, 0, 0,
            0, 0, 10, 20, 20, 10, 0, 0,
            0, 0, 10, 20, 20, 10, 0, 0,
            0, 0, 10, 20, 20, 10, 0, 0,
            0, 0, 10, 20, 20, 10, 0, 0,
            0, 0, 0, 20, 20, 0, 0, 0

        },

        //*white queen (Q) positional score
        {
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0

        },

        //*white king (K) positional score
        {
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 5, 5, 5, 5, 0, 0,
            0, 5, 5, 10, 10, 5, 5, 0,
            0, 5, 10, 20, 20, 10, 5, 0,
            0, 5, 10, 20, 20, 10, 5, 0,
            0, 0, 5, 10, 10, 5, 0, 0,
            0, 5, 5, -5, -5, 0, 5, 0,
            0, 0, 5, 0, -15, 0, 10, 0

        },

        //*black pawn (p) positional score
        {

            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, -10, -10, 0, 0, 0,
            0, 0, 0, 5, 5, 0, 0, 0,
            5, 5, 10, 20, 20, 5, 5, 5,
            10, 10, 10, 20, 20, 10, 10, 10,
            20, 20, 20, 30, 30, 30, 20, 20,
            30, 30, 30, 40, 40, 30, 30, 30,
            90, 90, 90, 90, 90, 90, 90, 90

        },

        //*black knight (n) positional score
        {

            -5, -10, 0, 0, 0, 0, -10, -5,
            -5, 0, 0, 0, 0, 0, 0, -5,
            -5, 5, 20, 10, 10, 20, 5, -5,
            -5, 10, 20, 30, 30, 20, 10, -5,
            -5, 10, 20, 30, 30, 20, 10, -5,
            -5, 5, 20, 20, 20, 20, 5, -5,
            -5, 0, 0, 10, 10, 0, 0, -5,
            -5, 0, 0, 0, 0, 0, 0, -5

        },

        //*black bishop (b) positional score
        {

            0, 0, -10, 0, 0, -10, 0, 0,
            0, 30, 0, 0, 0, 0, 30, 0,
            0, 10, 0, 0, 0, 0, 10, 0,
            0, 0, 10, 20, 20, 10, 0, 0,
            0, 0, 10, 20, 20, 10, 0, 0,
            0, 0, 0, 10, 10, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0

        },

        //*black rook (r) positional score
        {

            0, 0, 0, 20, 20, 0, 0, 0,
            0, 0, 10, 20, 20, 10, 0, 0,
            0, 0, 10, 20, 20, 10, 0, 0,
            0, 0, 10, 20, 20, 10, 0, 0,
            0, 0, 10, 20, 20, 10, 0, 0,
            0, 0, 10, 20, 20, 10, 0, 0,
            50, 50, 50, 50, 50, 50, 50, 50,
            50, 50, 50, 50, 50, 50, 50, 50

        },

        //*black queen (q) positional score
        {

            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0

        },

        //*black king (k) positonal score
        {

            0, 0, 5, 0, -15, 0, 10, 0,
            0, 5, 5, -5, -5, 0, 5, 0,
            0, 0, 5, 10, 10, 5, 0, 0,
            0, 5, 10, 20, 20, 10, 5, 0,
            0, 5, 10, 20, 20, 10, 5, 0,
            0, 5, 5, 10, 10, 5, 5, 0,
            0, 0, 5, 5, 5, 5, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0

        }

};

int Engine::mirrorIndex[128] = {
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8

};

/*
                Pawn Knight Bishop   Rook  Queen   King (Victims)

    Pawn        105    205    305    405    505    605
    Knight      104    204    304    404    504    604
    Bishop      103    203    303    403    503    603
    Rook        102    202    302    402    502    602
    Queen       101    201    301    401    501    601
    King        100    200    300    400    500    600
(Attackers)

*/
//* MVV_LVA_MOVE_SCORE[attacker_piece][victim_piece] = returns the relative score of a move which captures the victim_piece using attacker_piece
int Engine::MVV_LVA_MOVE_SCORE[12][12] =
    {

        105, 205, 305, 405, 505, 605, 105, 205, 305, 405, 505, 605,
        104, 204, 304, 404, 504, 604, 104, 204, 304, 404, 504, 604,
        103, 203, 303, 403, 503, 603, 103, 203, 303, 403, 503, 603,
        102, 202, 302, 402, 502, 602, 102, 202, 302, 402, 502, 602,
        101, 201, 301, 401, 501, 601, 101, 201, 301, 401, 501, 601,
        100, 200, 300, 400, 500, 600, 100, 200, 300, 400, 500, 600,

        105, 205, 305, 405, 505, 605, 105, 205, 305, 405, 505, 605,
        104, 204, 304, 404, 504, 604, 104, 204, 304, 404, 504, 604,
        103, 203, 303, 403, 503, 603, 103, 203, 303, 403, 503, 603,
        102, 202, 302, 402, 502, 602, 102, 202, 302, 402, 502, 602,
        101, 201, 301, 401, 501, 601, 101, 201, 301, 401, 501, 601,
        100, 200, 300, 400, 500, 600, 100, 200, 300, 400, 500, 600

};

//*KILLER_MOVE[id][ply] ---> most of the engines store 2 killer moves for each ply for efficiency
Move Engine::KILLER_MOVES[2][MAX_DEPTH];

//*HISTORY_MOVES[piece][square]
int Engine::HISTORY_MOVE_SCORE[12][64];
