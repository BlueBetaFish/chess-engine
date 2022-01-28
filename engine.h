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

struct MinimaxReturn
{
    int bestScore;
    Move bestMove;
};

class Engine : public Board
{
    //*materialValue[piece]
    static int materialValue[12];

    //*positionalScore[piece][squareIndex]
    static int positionalScore[12][64];

    //*mirrorIndex[squareIndex]
    static int mirrorIndex[128];

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
    //*TODO: score is positive if white has advantage, and score is neg if black has advantage( different from BBC, it returened score as positive if current player had advantage)
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

        return score;
    }

    //*TODO: ply
    MinimaxReturn minimax(int depthLimit, int alpha, int beta)
    {
        if (depthLimit == 0)
            return {this->staticEvaluation(), Move::INVALID_MOVE};

        long long legalMoves = 0;
        int ply = 0;

        bool inCheck = this->isCurrentPlayerKingInCheck();

        Move bestMove = Move::INVALID_MOVE;

        if (this->currentPlayer == WHITE)
        {
            int maxScore = INT_MIN;

            //*generate all pseudo legal moves
            MoveList moveList;
            this->generateAllPseudoLegalMovesOfGivenPlayer(this->currentPlayer, moveList);

            Engine backUpCopyOfBoard = *this;

            int moveListSize = moveList.size();

            for (int i = 0; i < moveListSize; i++)
            {
                Move move = moveList[i];
                //*if making this move lets opponent capture our king , dont consider it
                if (!this->makeMove(move))
                    continue;

                legalMoves++;
                ply++;

                MinimaxReturn currReturnedVal = this->minimax(depthLimit - 1, alpha, beta);

                //*restore board
                *this = backUpCopyOfBoard;

                if (currReturnedVal.bestScore > maxScore)
                {
                    maxScore = alpha = currReturnedVal.bestScore;
                    bestMove = move; //*current move is the best move
                }

                if (alpha >= beta)
                    return {maxScore, bestMove};
            }

            //*if current player doesnt have any legal moves
            if (legalMoves == 0)
            {
                //*if king is in check, then it is checkmate
                if (inCheck)
                {
                    //*TODO:
                    return {-49000 + ply, Move::INVALID_MOVE};
                }
                //*stalemate
                else
                {
                    return {0, Move::INVALID_MOVE};
                }
            }

            //*return best move
            return {maxScore, bestMove};
        }

        //*for black
        else
        {
            int minScore = INT_MAX;

            //*generate all pseudo legal moves
            MoveList moveList;
            this->generateAllPseudoLegalMovesOfGivenPlayer(this->currentPlayer, moveList);

            Engine backUpCopyOfBoard = *this;

            int moveListSize = moveList.size();
            for (int i = 0; i < moveListSize; i++)
            {
                Move move = moveList[i];

                //*if making this move lets opponent capture our king , dont consider it
                if (!this->makeMove(move))
                    continue;

                legalMoves++;
                ply++;

                MinimaxReturn currReturnedVal = this->minimax(depthLimit - 1, alpha, beta);

                //*restore board
                *this = backUpCopyOfBoard;

                if (currReturnedVal.bestScore < minScore)
                {
                    minScore = beta = currReturnedVal.bestScore;
                    bestMove = move; //*current move is the best move
                }

                if (alpha >= beta)
                    return {minScore, bestMove};
            }

            //*if current player doesnt have any legal moves
            if (legalMoves == 0)
            {
                //*if king is in check, then it is checkmate
                if (inCheck)
                {
                    //*TODO:
                    return {49000 - ply, Move::INVALID_MOVE};
                }
                //*stalemate
                else
                {
                    return {0, Move::INVALID_MOVE};
                }
            }

            //*return best move
            return {minScore, bestMove};
        }
    }

    //*TODO:
    //*find the best move upto given depth
    void inline searchPosition(int depth)
    {

        MinimaxReturn res = minimax(depth, -50000, 50000);

        if (res.bestMove == Move::INVALID_MOVE)
        {
            cout << "\n\nProblem searchPostion : INVALID_MOVE returned\n\n";
            return;
        }
        else
        {
            cout << "info score cp " << res.bestScore << " depth " << depth << " nodes "
                 << "-1" << endl;
            cout << "bestmove " << res.bestMove.getUCIMove() << endl;
        }
    }
};

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