#pragma once

// Name: SOURASHIS MONDAL
#include "neededHeaders.h"

using namespace std;

#include "definitions.h"

#include "BitBoard.h"
#include "PieceClass.h"
#include "Board.h"
#include "UCISearchInfo.h"

//*helper functions
#include "helperFunctions.h"

//*we assume we will search upto max 64 depth
#define MAX_DEPTH 64

//*Minimum number of moves ,after which other moves can be reduced
#define FULL_DEPTH_MOVES 4

//*min depth after which moves can be reduced
#define REDUCTION_DEPTH_LIMIT 3





class Engine : public UCISearchInfo
{
    //*current baord position
    Board boardPosition;

    //*bestMove after a search
    Move BEST_MOVE;
    
    //*Number of nodes searched in a search
    long long nodeCount;

    //*Killer move is a dangerous quiet move which kills opponent's most of the other moves  to be expanded (example link : https://rustic-chess.org/search/ordering/killers.html)
    //*KILLER_MOVE[id][ply] ---> most of the engines store 2 killer moves for each ply for efficiency
    Move KILLER_MOVES[2][MAX_DEPTH];

    //*SCORE OF HISTORY MOVES : //*TODO: gotta comment properly later
    //*HISTORY_MOVES[piece][square]
    int HISTORY_MOVE_SCORE[12][64];


    //*Principal variation table
    /*
    * For example : for depth 4 , PV_TABLE:
    *
    *    ply
    *    |
    *    |
    *    V
    *    0  ---> m4 m3 m2 m1
    *    1  ---> m3 m2 m1
    *    2  ---> m2 m1
    *    3  ---> m1
    */
    //*Principal variation table : PV_TABLE[ply] = move list(principal vaiation line moves at ply )
    //* PV_TABLE[0][depth] = best move of the player at depth after completing the search
    MoveList PV_TABLE[MAX_DEPTH];


    //*-----------------------------------------------------------------------------------------------------------------------------------------------------------------*//
    //*-----------------------------------------------------------_________________________-----------------------------------------------------------------------------*//
    //*----------------------------------------------------------|                         |----------------------------------------------------------------------------*//
    //*----------------------------------------------------------|         TABLES          |----------------------------------------------------------------------------*//
    //*----------------------------------------------------------|    (STATIC VARIABLES)   |----------------------------------------------------------------------------*//
    //*----------------------------------------------------------|_________________________|----------------------------------------------------------------------------*//
    //*-----------------------------------------------------------------------------------------------------------------------------------------------------------------*//

    //*materialValue[piece] = value of a piece
    static int materialValue[12];

    //*positionalScore[piece][squareIndex] = positional score of piece at squareIndex
    static int positionalScore[12][64];

    //*mirrorIndex[squareIndex]
    static int mirrorIndex[128];

    //*Most valuable victim , less valuable attacker score table for move ordering (stores the score of a move to order them from best to worst)
    static int MVV_LVA_MOVE_SCORE[12][12];



public:

    //*Constructor
    Engine(string fen = START_POSITION_FEN)
    {
        this->boardPosition.initializeFromFenString(fen);
        resetTablesOfSearch();
    }
    
    //*initializes the boardposition from given fen string
    void initializeFromFenString(const string &fen)
    {
        boardPosition.initializeFromFenString(fen);
    }

    //*returns the current player to move
    int getCurrentPlayer()
    {
        return this->boardPosition.currentPlayer;
    }

    //*returns all pseudo legal moves of given player through the passed argument "generatedMoves"
    void generateAllPseudoLegalMovesOfGivenPlayer(int playerColor, MoveList &generatedMoves)
    {
        boardPosition.generateAllPseudoLegalMovesOfGivenPlayer(playerColor, generatedMoves);    
    }
    
    //*returns the number of leaf nodes upto given depth limit
    long long inline perft_driver(int depthLimit)
    {
        return boardPosition.perft_driver(depthLimit);
    }

    //* prints the number of leaf nodes upto given depth limit after the corresponding move
    long long inline perft_test(int depthLimit)
    {
        // long long startTime = Engine::getTimeInMilliSeconds();
        long long res = boardPosition.perft_test(depthLimit);
        // long long endTime = Engine::getTimeInMilliSeconds();
        cout << "\nTotal Number of leaves upto depth = " << depthLimit << "  =  " << res << endl;
        // cout << "\n\nExecution time = " << (endTime - startTime) << " milliseconds ." << endl;

        return res;
    }
    


    /*
    * Executes the given move on the Board Object
    *
    *   If you want to execute the move, if it is only capture move (needed in Quiescence Search) , pass true as the 2nd argument (default value false)
    * 
    *   Returns true if the move is legal ,
    *                else returns false if the move is pseudo legal (i.e, current player's king can be captured after the move) and also undo the move
    */
    bool makeMove(const Move &move, bool onlyCaptureMove = false)
    {
        return boardPosition.makeMove(move, onlyCaptureMove);
    }

    //*prints the baord position
    void inline print()
    {
        this->boardPosition.print();
    }

    //*Debugging function: prints the positional scores of pieces
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



    //*-----------------------------------------------------------------------------------------------------------------------------------------------------------------*//
    //*-----------------------------------------------------------_________________________-----------------------------------------------------------------------------*//
    //*----------------------------------------------------------|                         |----------------------------------------------------------------------------*//
    //*----------------------------------------------------------|     MOVE ORDERING       |----------------------------------------------------------------------------*//
    //*----------------------------------------------------------|_________________________|----------------------------------------------------------------------------*//
    //*-----------------------------------------------------------------------------------------------------------------------------------------------------------------*//



    /*
    *   Move ordering is needed to prune more branches using alpha-beta search.
    *   
    *   While doing the minimax/negamax search, while generating the search tree, if the expand 
    * the best move first for all nodes, then the pruning is maximum. Thus good move ordering 
    * increases the efficiency of alpha-beta pruning. 
    *
    *   We use heuristics to order the moves of a position from good to bad(estimated).
    *   We will sort the moves before searching, using the score of the move obtained from : "int getMove(Move move)" --> Heuristic function
    *
    */
    //*-----------------------------------------------------------------------------------------------------------------------------------------------------------------*//

    /*
    *   Move Ordering:
    *   ---------------------------------------------------------------------
    *       1. PV move
    *       2. Captures in MVV_LVA
    *       3. 1st Killer move
    *       4. 2nd Killer move
    *       5. History move
    *       6. Rest of the moves
    *
    * 
    */
    //*-----------------------------------------------------------------------------------------------------------------------------------------------------------------*//

    /*
    *    DESCRIPTION:
    *    =================================================================================
    * 
    *   1. PV moves:
    *   -------------------------------------------------------------------------------------------------------------------------------------
    *            We will store Principal variation for a search upto a certain depth during iterative deepening.
    *            And in the next search of the iterative deepening, we will give the principal variation line 
    *            moves the max priority to be searched first. 
    * 
    *   2. Capture Heuristic(MVV_LVA):
    *   -------------------------------------------------------------------------------------------------------------------------------------
    *            It stands for most valuable victim, least valuable attacker.
    *            We will give more priority to a move if it is a capture of a bigger piece using a smaller valued piece.
    *            We are storing the move score of MVV_LVA using : int MVV_LVA_MOVE_SCORE[12][12]; // use as MVV_LVA_MOVE_SCORE[attackerPiece][capturedPiece]
    * 
    *   3. Killer Move Heuristic: (Example URL : https://rustic-chess.org/search/ordering/killers.html)
    *   -------------------------------------------------------------------------------------------------------------------------------------
    *           * Killer move is a dangerous quiet move which kills most of the other moves at the same ply(level).
    *           * So when a non-capture move causes a beta cutoff at a ply, we store it as a killer move for that ply. 
    *       While checking other positions at that ply, if that killer move for that ply is a valid move of that position,
    *       then we give higher priority to that move and check that move first to get more cutoffs.
    *           * The concept is if a non-capture move is a great move(causes beta cutoff)  for a position at a given ply, 
    *       then for other positions of the same ply, it can be a great move(that is, it can cause cutoffs).
    *           * Usually we give capture moves most priority, then killer moves.
    * 
    *           * We will store 2 killer moves for each plies using the table : Move KILLER_MOVES[2][MAX_DEPTH];
    *           * Use it as : KILLER_MOVE[moveNumber][ply]
    *           * While getting the score of a move using "int getMove(Move move, int ply)" function, we will check
    *       if the move is equal to any of the killer moves. If yes, then we will return a big score for this move.
    *           * First killer move is scored more than the second killer move.
    *           * For storing the killer moves, inside alpha-beta function, if a quiet move causes a beta cutoff(that is score >= beta), then we will store it as a killer move:
    *           * This heuristic has a huge impact on pruning. It’s very important.
    *                       
    *                       CHANGE IN CODE(TO STORE KILLER MOVES):
    *                   -------------------------------------------------
    *                  ________________________                            _____________________________________________________________________
    *                 |                        |                          |                                                                     |
    *                 |  if(score >= beta)     |                          |     if(score >= beta)                                               |         
    *                 |  {                     |   =============>>>>      |     {                                                               |
    *                 |    return beta;        |                          |         if(move is a quiet move)                                    |
    *                 |  }                     |                          |         {                                                           |
    *                 |________________________|                          |             KILLER_MOVE[1][ply] = KILLER_MOVE[0][ply]; //shift      |
    *                                                                     |             KILLER_MOVE[0][ply] = move;                             |
    *                                                                     |         }                                                           |
    *                                                                     |                                                                     |
    *                                                                     |         return beta;                                                |
    *                                                                     |     }                                                               |
    *                                                                     |_____________________________________________________________________|
    * 
    *   4. History heuristic:
    *   -------------------------------------------------------------------------------------------------------------------------------------
    *       * History moves work much the same as killer moves, but here the engine saves quiet moves that improves the alpha of the search.
    *       * History heuristic is often presented as depth-independent generalization of the killer moves. 
    *       * We store the history score of a move in a table : "int HISTORY_SCORE[TOTAL_NO_OF_PIECES][MAX_DEPTH] "
    *       * If a non-capture, and non killer move improves the alpha score, then we increase the history score of that move( HISTORY_MOVE_SCORE[piece][toSquare] += depth) .
    *       * The added value is typically depth, based on the assumption that otherwise moves from the plies
    *   near the leaves would have too much impact on the result. Values retrieved from that table are used to order non-capturing moves.      
    * 
    *                       CHANGE IN CODE(TO STORE History scores):
    *                   -------------------------------------------------
    *                  ________________________                            ________________________________________________________________________
    *                 |                        |                          |                                                                        |
    *                 |  if(score > alpha)     |                          |     if(score > alpha)                                                  |         
    *                 |  {                     |   =============>>>>      |     {                                                                  |
    *                 |     score = alpha;     |                          |         score = alpha;                                                 |
    *                 |  }                     |                          |         if(move is a quiet move)                                       |
    *                 |________________________|                          |         {                                                              |
    *                                                                     |             HISTORY_MOVE_SCORE[move.pieceMoved][move.toSquare] += ply; |
    *                                                                     |         }                                                              |
    *                                                                     |     }                                                                  |
    *                                                                     |________________________________________________________________________|
    * 
    * 
    * 
    * 
    * 
    *    
    */


    //*retruns the relative score of a move so that we can order them from good to bad 
    //*(isCurrNodeFollowingPVLine is the flag if the current node is following the pv line)
    int inline getMoveScore(const Move &move, int ply, bool isCurrNodeFollowingPVLine)
    {
        //*================================ PV MOVE ============================================*/

        //*if the position is following pv line then check if the move is equal to PV move at given ply or not
        //*if the move is a PV move at the given ply , then return max score for this move
        if (isCurrNodeFollowingPVLine)
        {
            //*if the PV_LINE upto given ply was found
            if (ply < PV_TABLE[0].size())
            {
                //*if the move matches the PV move at the given ply
                if (move == PV_TABLE[0][ply])
                {
                    // //*TODO:Printing For debugging purpose:
                    // cout << "current PV move : ";
                    // cout << move.getUCIMove();
                    // cout << " , at ply : " << ply;
                    // cout << endl;

                    //*return the max score for this PV move
                    return 20000;
                }
            }
        }

        //*================================ CAPTURE HEURISTIC (MVV_LVA) ============================================*/

        //*if move is capture move , then find the MVV_LVA_MOVE_SCORE value
        if (move.captureFlag)
        {
            //*TODO: also include the capturedPiece in move class, so that we dont need to call this function to get the captured piece here
            int capturedPiece = Piece::EMPTY_PIECE;

            //*if the move was enpassant , then toSquare would be empty, so assign pawn
            if (move.enPassantFlag)
            {
                capturedPiece = this->boardPosition.currentPlayer == WHITE ? Piece::p : Piece::P;
            }
            else
            {
                int startPiece = -1, endPiece = -1;
                if (this->boardPosition.currentPlayer == WHITE)
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
                    if (this->boardPosition.pieceBitBoards[piece].getBitAt(move.toSquare) == 1)
                    {
                        capturedPiece = piece;
                        break;
                    }
                }
            }

            // cout << "\nattacker : " << Piece::getASCIICodeOfPiece(move.pieceMoved) << ", captured Piece : " << Piece::getASCIICodeOfPiece(capturedPiece);

            //*because quiet killer move gets score of 9000, so to assign captures of greater weight, add 10,000
            return 10000 + Engine::MVV_LVA_MOVE_SCORE[move.pieceMoved][capturedPiece];
        }

        //*================================ FOR QUIET MOVES (KILLER MOVE AND HISTORY HEURISTICS) ============================================*/
        else
        {
            //*get score of 1st killer move, if exists
            if (KILLER_MOVES[0][ply] == move)
                return 9000;

            //*get score of 2nd killer move, if exists
            if (KILLER_MOVES[1][ply] == move)
                return 8000;

            //*get score of history move
            return HISTORY_MOVE_SCORE[move.pieceMoved][move.toSquare];
        }

        //*for rest of the moves, return 0
        return 0;
    }


    //*Sorts move from best to worst(according to the heuristic function : getMoveScore(move) )
    //*isCurrNodeFollowingPVLine is a flag indicating whether the current node(position) is following the PV line or not
    inline void sortMoveList(MoveList &moveList, int ply, bool isCurrNodeFollowingPVLine)
    {
        int moveListSize = moveList.size();

        //*store the scores of all moves
        int moveScores[moveListSize];
        for (int i = 0; i < moveListSize; i++)
            moveScores[i] = this->getMoveScore(moveList[i], ply, isCurrNodeFollowingPVLine);

        //*----------------- INSERTION SORT -------------------------------//
        //*as the average number of moves is 35-40, so insertion sort will have less overhead than mergesort and quicksort
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

    

    //*checks if the depth for the given move can be reduced
    //*Needed in Late Move Reduction (Reduce depth limit of searching for potentially bad moves)
    inline bool moveCanBeReduced(const Move &move, bool inCheck) const
    {
        /*
         *            Common Conditions : (Reference https://www.chessprogramming.org/Late_Move_Reductions)
         *
         *            Most programs do not reduce these types of moves:
         *           --------------------------------------------------------------
         *                1. Tactical Moves (captures and promotions) (IMPLEMENTED)
         *                2. Moves while in check  (IMPLEMENTED)
         *                3. Moves which give check
         *                4. Moves that cause a search extension
         *                5. Anytime in a PV-Node in a PVS search
         *                6. Depth < 3 (sometimes depth < 2)
         */

        return (!inCheck && !move.captureFlag && move.promotedPiece == -1);
    }

    

    //*TODO: to be improved
    //*Static Evaluation of a position is the score of the position without searching that position dynamically
    /*
    *   In normal minimax algorithm we consider static evaluation as the score of the position with respect to a particular player(usually white : positive for white and negative for black),
    *   but in negamax we consider static evaluation as the score w.r.t to the current player to move (positive if the current player has advantage, else negative)
    */
    inline int staticEvaluation()
    {
        int score = 0;
        int ls1bIndex = -1;
        int squareIndex = -1;
        BitBoard pieceBitBoard;

        //*iterate all pieces on the board
        for (int piece = Piece::P; piece <= Piece::k; piece++)
        {
            pieceBitBoard = this->boardPosition.pieceBitBoards[piece];

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

        return this->boardPosition.currentPlayer == WHITE ? score : -score;
    }













    //*-----------------------------------------------------------------------------------------------------------------------------------------------------------------*//
    //*-----------------------------------------------------------_________________________-----------------------------------------------------------------------------*//
    //*----------------------------------------------------------|                         |----------------------------------------------------------------------------*//
    //*----------------------------------------------------------|     HORIZON EFFECT      |----------------------------------------------------------------------------*//
    //*----------------------------------------------------------|          AND            |----------------------------------------------------------------------------*//
    //*----------------------------------------------------------|   QUIESCENCE SEARCH     |----------------------------------------------------------------------------*//
    //*----------------------------------------------------------|_________________________|----------------------------------------------------------------------------*//
    //*-----------------------------------------------------------------------------------------------------------------------------------------------------------------*//
    /*
    *   Since the alpha beta algorithm searches to one specific depth it is prone to be a victim of what 
    *   is popularly named the "Horizon effect". The evaluation function does not make any moves and can
    *   not tell if a piece is hanging or not. Say the engine has searched to a depth of 7 and finds out
    *   it's a whole Knight ahead of the opposition and happily makes the move. What if it were to search
    *   to a depth of 8 and find out that it loses a Queen in the next move? Not such a good move to make
    *   after all. To limit the horizon effect the engine therefore should only evaluate quiet positions,
    *   positions where no captures can be made. Quiescence search simply makes all captures possible in 
    *   a position before it is evaluated. The ordering of the moves are same as the nagamax algorithm.
    * 
    * 
    *                       CHANGE IN CODE(Replace StatiEvaluation() with QuiescenceSearch() insdie Minimax/Negamax() function):
    *                   ---------------------------------------------------------------------------------------------------------------
    *    ________________________________________________________                        _______________________________________________________
    *   |                                                        |                      |                                                       |
    *   |  if( depth == 0)                                       |                      |     if( depth == 0)                                   |         
    *   |     return StaticEvaluationOfPosition(boardPosition);  |   =============>>>>  |       return QuiscenceSearch(alpha, beta, ply);       |
    *   |________________________________________________________|                      |_______________________________________________________|
    *                                                                                       
    * 
    */

    
    //* returns the max static evaluation score from the initial position until the position is quiet (i.e, there is no more captures)
    int inline quiescenceSearch(int alpha, int beta, int ply)
    {
        
        //* FOR TIME CONTROL: for every 2048 nodes check if there is any interrupt(or input from user/GUI) or time has ended
        if ((nodeCount & 2047) == 0)
            communicateWithGUI();
        
        //*if time is up
        if(stopped)
            return 0;

            
        //*increment searched nodes counter
        nodeCount++;

        int currScore = this->staticEvaluation();

        // cout << "\nBoard pos:\n";
        // this->printBoard();

        if (currScore >= beta)
            return beta;

        alpha = max(alpha, currScore);

        // long long nodeCount = 0;

        //*generate all pseudo legal moves
        MoveList moveList;
        this->boardPosition.generateAllPseudoLegalMovesOfGivenPlayer(this->boardPosition.currentPlayer, moveList);

        //*sort moves from good to bad for more alpha beta pruning
        this->sortMoveList(moveList, ply, false); //*quiscence search is not following PV line

        //*store backUpCopy of the current board position for taking back the executed move later
        Board backUpCopyOfBoard = boardPosition;

        int moveListSize = moveList.size();
        int maxScore = currScore;

        for (int i = 0; i < moveListSize; i++)
        {
            Move move = moveList[i];

            //*if making this move lets opponent capture our king , dont consider it
            //* ----------- IMPORTANT: only make the capture moves in QUIESCENCE SEARCH -------------*/
            if (!this->boardPosition.makeMove(move, true))
                continue;

            int currScore = -quiescenceSearch(-beta, -alpha, ply + 1);

            //*restore board
            boardPosition = backUpCopyOfBoard;

            //*return 0 if time is up
            if (stopped)
                return 0;
            
            //*if better score is found
            if (currScore > maxScore)
            {
                maxScore = alpha = currScore;
            }

            //*fail hard beta-cutoff
            if (alpha >= beta)
                return beta;
        }

        //*return best score
        return alpha;
    }









    //*-----------------------------------------------------------------------------------------------------------------------------------------------------------------*//
    //*-----------------------------------------------------------_________________________-----------------------------------------------------------------------------*//
    //*----------------------------------------------------------|                         |----------------------------------------------------------------------------*//
    //*----------------------------------------------------------|    NEGAMAX / MINIMAX    |----------------------------------------------------------------------------*//
    //*----------------------------------------------------------|_________________________|----------------------------------------------------------------------------*//
    //*-----------------------------------------------------------------------------------------------------------------------------------------------------------------*//



    /*
    *   ====================================================================================================  
    *                                         ALPHA-BETA SEARCH (NEGAMAX):
    *   ====================================================================================================
    *   
    *    ______________________________________________________________
    *   |   int alphaBeta(int depthLeft, int alpha, int beta)          |
    *   |   {                                                          |
    *   |      if( depthleft == 0 )                                    |
    *   |         return quiescenceSearch( alpha, beta );              |
    *   |                                                              |
    *   |      for ( all moves)                                        |
    *   |      {                                                       |                                              
    *   |         score = -alphaBeta(depthLeft - 1, -beta, -alpha);    |
    *   |                                                              |
    *   |         if( score >= beta )                                  |
    *   |            return beta;   //  fail hard beta-cutoff          |
    *   |                                                              |
    *   |         if( score > alpha )                                  |
    *   |            alpha = score; // alpha acts like max in MiniMax  |
    *   |      }                                                       |
    *   |      return alpha;                                           |
    *   |   }                                                          |
    *   |______________________________________________________________|  
    * 
    * 
    * 
    * 
    * 
    *   ====================================================================================================  
    *                                   PRINCIPAL VARIATION SEARCH (IMPROVEMENT OF NEGAMAX):
    *   ====================================================================================================
    * 
    *   Reference: https://web.archive.org/web/20071030220825/http://www.brucemo.com/compchess/programming/pvs.htm
    *  
    *   -> PVS is a means of getting a small performance improvement out of the alpha-beta algorithm. Performance gain is almost 10%.
    *   -> Any node in alpha-beta search belongs to one of the 3 types:
    * 
    *   -> ALPHA NODE: every move of the node will have a score <= alpha, meaning that no moves of this node will be any good.
    *     -------------
    *   -> BETA NODE: at least one of the moves of this node will be >= beta, that is, this branch can be pruned.
    *     --------------
    *   -> Principal Variation(PV) Node: one or more moves of this node will have a score > alpha, but none of the moves will have score >= beta
    *     --------------
    * 
    *   
    *   -> For a node, if any move fails high(that is score of the move >= beta), then it is surely beta node.
    *   -> If the first move of that node fails low(that is, score of the move <= alpha), then it may be an Alpha node, or a PV node.
    * To make sure it is not a PV node, we can search this move with shallower bound (alpha, alpha+1) and see if it returns a score > alpha or not. 
    * If yes then it is a PV node, and we should search this move again with full bound (alpha,beta) to get the actual score. Else(if score of the 
    * move with bound (alpha,alpha+1) is <= alpha) the move can not contribute to better scoring. This shallower bound search to prove that the move
    * is surely not going to be a better move, reduces the time in practical games. If the node is a PV node, then research is needed to be done with
    * actual (alpha,beta) bound, that is the extra search will cost some extra time. But in real scenario the researches needed to be done are less 
    * assuming the move ordering is good(that is, assuming we already searched the best move first) compared to the improvement due to narrower bound search.
    * 
    * 
    *   -> Usually we search the first move of a node with actual (alph , beta) bound. Then for the rest of the moves, 
    * we first search with (alpha, alpha + 1) bound to check if it is an alpha node or PV node. If PV node, then we
    * research the move with actual (alpha, beta) bound.
    * 
    * 
    * 
    *  
    *                       CHANGE IN CODE(Include Principal Variation Search insde Negamax Search):
    *                   ------------------------------------------------------------------------------------
    * 
    * 
    *           NEGAMAX (ALPHA-BETA) SEARCH:
    *   =============================================== 
    *    ______________________________________________________________
    *   |   int alphaBeta(int depthLeft, int alpha, int beta)          |
    *   |   {                                                          |
    *   |      if( depthleft == 0 )                                    |
    *   |         return quiescenceSearch( alpha, beta );              |
    *   |                                                              |
    *   |      for ( all moves)                                        |
    *   |      {                                                       |                                              
    *   |         score = -alphaBeta(depthLeft - 1, -beta, -alpha);    |
    *   |                                                              |
    *   |         if( score >= beta )                                  |
    *   |            return beta;   //  fail hard beta-cutoff          |
    *   |                                                              |
    *   |         if( score > alpha )                                  |
    *   |            alpha = score; // alpha acts like max in MiniMax  |
    *   |      }                                                       |
    *   |      return alpha;                                           |
    *   |   }                                                          |
    *   |______________________________________________________________|  
    * 
    *                              ||
    *                              ||
    *                              \/ 
    * 
    *           NEGAMAX (ALPHA-BETA) SEARCH WITH PRINCIPAL VARATION:
    *   ============================================================================
    *    _______________________________________________________________________________
    *   |   int alphaBeta(int depthLeft, int alpha, int beta)                           |
    *   |   {                                                                           |
    *   |      if( depthleft == 0 )                                                     |
    *   |         return quiescenceSearch( alpha, beta );                               |
    *   |                                                                               |
    *   |      for ( all moves)                                                         |
    *   |      {                                                                        |                                              
    *   |         if (the current move is the first move of this node to be searched)   |
    *   |         {                                                                     |
    *   |               //search with full bound                                        |
    *   |               score = -alphaBeta(depthLeft - 1, -beta, -alpha);               |
    *   |                                                                               |
    *   |               if( score >= beta )                                             |
    *   |                   return beta;   //  fail hard beta-cutoff                    |
    *   |                                                                               |
    *   |               if( score > alpha )                                             |
    *   |                   alpha = score; // alpha acts like max in MiniMax            |
    *   |         }                                                                     |
    *   |         else                                                                  |
    *   |         {                                                                     |
    *   |               //search with narrower bound :  alpha: alpha, beta: (alpha + 1) |  
    *   |               score = -alphaBeta(depthLeft - 1, -alpha - 1, -alpha);          |
    *   |                                                                               |
    *   |               //check if it is a PV node                                      |
    *   |               if ((score > alpha) && (score < beta))                          |
    *   |                   score = -alphaBeta(depthLeft - 1, -beta, -alpha);           |
    *   |         }                                                                     |
    *   |      }                                                                        |
    *   |                                                                               |
    *   |      return alpha;                                                            |
    *   |   }                                                                           |
    *   |_______________________________________________________________________________|  
    * 
    * 
    */

    //* returns the best score for the current player from current boardPosition
    //*  searches the current board position upto given depthLimit (or until time runs out)
    //*  [alpha,beta] denotes the bound for best score of current position (Alpha-Beta pruning)
    //* "isCurrNodeFollowingPVLine" is a flag indicating whether the current node(position) is following the PV line or not (needed in PRINCIPAL VARIATION SEARCH)
    int inline negamax(int depthLimit, int alpha, int beta, int ply, bool isCurrNodeFollowingPVLine)
    {
        //* FOR TIME CONTROL: for every 2048 nodes check if there is any interrupt(or input from user/GUI) or time has ended
        if ((nodeCount & 2047) == 0)
            communicateWithGUI();

        //*if time is up
        if(stopped)
            return 0;

        //*if the depthLimit is exhausted, do Quiescence Search
        if (depthLimit <= 0)
        {
            // return {this->staticEvaluation(), Move::INVALID_MOVE, 1};
            return quiescenceSearch(alpha, beta, ply);
        }

        //*increment nodes count
        nodeCount++;

        long long legalMoves = 0;

        bool inCheck = this->boardPosition.isCurrentPlayerKingInCheck();

        //*if current player is in check(it is an interesting position and cant be ignored) ,
        //*increase search depth if the king has been exposed into a check to avoid being mated
        int oldDepthLimit = depthLimit;
        if (inCheck)
            depthLimit++;

        // long long nodeCount = 0;

        int maxScore = INT_MIN;

        //*number of searched moves of the current node
        int searchedMoveCount = 0;

        //*preserve board state to restore the board after executing each move
        Board backUpCopyOfBoard = boardPosition;


        // TODO: uncomment NULL MOVE PRUNING , when you understand and implement properly
        //*----------------------------------NULL MOVE PRUNING START-------------------------------------------------------------//
        //* Resource : https://web.archive.org/web/20071031095933/http://www.brucemo.com/compchess/programming/nullmove.htm

        //*if there are >= 3 depths to be searched and it is not the root node, and king is not in check , then do null move pruning
        if (depthLimit >= 3 && !inCheck && ply != 0)
        {
            //*make null move( Null Move : you do not play any move and let the opponent play her move)
            this->boardPosition.currentPlayer = Piece::getOppositeColor(this->boardPosition.currentPlayer);
            this->boardPosition.enPassantSquareIndex = -1; //*IMPORTANT: reset enPassant square

            //*search null move with reduced depthLimit to find beta cut offs
            //*here alpha = beta - 1
            int currScore = -negamax(depthLimit - 1 - 2, -beta, -(beta - 1), ply + 1, false);

            //*restore board
            boardPosition = backUpCopyOfBoard;

            //*return 0 if time is up
            if (stopped)
                return 0;
            

            if (currScore >= beta)
                return beta;
        }
        // //*----------------------------------NULL MOVE PRUNING END-------------------------------------------------------------//



        //*generate all pseudo legal moves
        MoveList moveList;
        this->boardPosition.generateAllPseudoLegalMovesOfGivenPlayer(this->boardPosition.currentPlayer, moveList);

        //*sort moves from good to bad for more alpha beta pruning
        this->sortMoveList(moveList, ply, isCurrNodeFollowingPVLine);

        int moveListSize = moveList.size();

        //*if the current node is not following the pv line, then its children nodes will not follow pv line,
        //*but if the current node is following the pv line, then that children which is obtained by executing the PV move, will follow pv line
        bool isChildNodeFollowingPVLine = isCurrNodeFollowingPVLine;

        //*variable to keep track of whether the  PV node of "Principal Variation Search" is found or not
        //*Learn about Principal Variation Search : https://web.archive.org/web/20071030220825/http://www.brucemo.com/compchess/programming/pvs.htm
        // bool foundPVNode = false;

        for (int i = 0; i < moveListSize; i++)
        {
            Move move = moveList[i];

            //*if making this move lets opponent capture our king , dont consider it
            if (!this->boardPosition.makeMove(move))
                continue;

            //*if the node(position) was following PV line , and the move is pv move then 
            //*enable isCurrNodeFollowingPVLine for the children nodes of this move,
            //*else disable isCurrNodeFollowingPVLine since other nodes are not following the pv line
            if (isCurrNodeFollowingPVLine)
            {
                //*if the PV move upto given ply was found previously then check if the current move
                //*is equal to the PV move of the given ply level, if true, then the child node will follow PV line
                if (ply < PV_TABLE[0].size() && PV_TABLE[0][ply] == move)
                    isChildNodeFollowingPVLine = true;
                else
                    isChildNodeFollowingPVLine = false;
            }

            legalMoves++;

            //*TODO: comment this normal negamax() call , because PrincipalVariationSearch() is implemented
            // MinimaxReturn currReturnedVal = this->negamax(depthLimit - 1, -beta, -alpha, ply + 1, isChildNodeFollowingPVLine); //*normal negamax search
            // int currScore = -currReturnedVal.bestScore;


            //*-----------------------------------------------------------------------------------------------------------------------------------------------------------------*//
            //*-----------------------------------------------------------_________________________-----------------------------------------------------------------------------*//
            //*----------------------------------------------------------|   PRINCIPAL VARIATION   |----------------------------------------------------------------------------*//
            //*----------------------------------------------------------|        SEARCH           |----------------------------------------------------------------------------*//
            //*----------------------------------------------------------| ======= (START) ======= |----------------------------------------------------------------------------*//
            //*----------------------------------------------------------|_________________________|----------------------------------------------------------------------------*//
            //*-----------------------------------------------------------------------------------------------------------------------------------------------------------------*//

            //*if we have already found that this node is PV node , then for other nodes (either alpha node or beta node) 
            //*try to prove that this node will fail to be a PV node (i.e, PV node : this move's score is better than alpha)
            /*
             *PVS :  If no PV move has been found, "AlphaBeta()" is recursed normally.  If one has been found,
             *everything changes.  Rather than searching with a normal (alpha, beta) window, we search with (alpha, alpha + 1).  The premise is that all of the searches are going to come
             *back with a score less than or equal to alpha, and if this is going to happen, the elimination of the top end of the window results in more cutoffs.  Of course, if the premise
             *is wrong, the score comes back at alpha + 1 or higher, and the search  must be re-done with the wider window.
             */

            int currScore;

            //*if the node is not PV node proved yet , then search with full alpha,beta bandwidth
            if (searchedMoveCount == 0)
            {
                currScore = -negamax(depthLimit - 1, -beta, -alpha, ply + 1, isChildNodeFollowingPVLine);
            }
            //*if principal variation node was found previously , then search the rest of nodes with narrower alpha, beta bound to try to prove that this move is bad,
            //* if it is indeed not a bad node, then research with full alpha,beta bandwidth
            else
            {
                //*-------------------------------------------------------------------------------------------------------------------------------------------
                //*Late Move Reduction (source  : https://web.archive.org/web/20150212051846/http://www.glaurungchess.com/lmr.html )
                //*-------------------------------------------------------------------------------------------------------------------------------------------

                /*
                 *Late move reductions are based on the simple observation that in a program with reasonably good move ordering, a beta cutoff will usually occur either at the first node, or
                 *not at all. We make use of this observation by searching the first few moves at every node with full depth, and searching the remaining moves with reduced depth unless they
                 *look particularly forcing or interesting in some other way. If one of the reduced moves surprise us by returning a score above alpha, the move is re-searched with full
                 * depth.
                 */

                //*TODO: depthLimit should be ply here
                if (searchedMoveCount >= FULL_DEPTH_MOVES && ply >= REDUCTION_DEPTH_LIMIT && moveCanBeReduced(move, inCheck))
                {
                    // *search current move with reduced depth (and also reduced aloha-beta bandwidth):
                    currScore = -negamax(depthLimit - 2, -(alpha + 1), -alpha, ply + 1, isChildNodeFollowingPVLine);
                }
                // *hack to ensure that full-depth search is done (making currScore = alpha + 1, so that the following if(){} block is entered and full PVS search is done )
                else
                {
                    currScore = alpha + 1;
                }

                //*if the move can not be reduced(i.e, full PVS search is to be done) or The reduced move produces a better score than alpha(prev best score), then do normal PVS
                if (currScore > alpha)
                {

                    //*search with beta = alpha + 1
                    currScore = -negamax(depthLimit - 1, -(alpha + 1), -alpha, ply + 1, isChildNodeFollowingPVLine);

                    //*if the narrow search finds that it is not a bad move that is , this move is better than the first found PV move, then research again with full alpha,beta bandwidth
                    if (currScore > alpha && currScore < beta)
                    {
                        currScore = -negamax(depthLimit - 1, -beta, -alpha, ply + 1, isChildNodeFollowingPVLine);
                    }
                }
            }

            //*-----------------------------------------------------------------------------------------------------------------------------------------------------------------*//
            //*-----------------------------------------------------------_________________________-----------------------------------------------------------------------------*//
            //*----------------------------------------------------------|   PRINCIPAL VARIATION   |----------------------------------------------------------------------------*//
            //*----------------------------------------------------------|        SEARCH           |----------------------------------------------------------------------------*//
            //*----------------------------------------------------------| ======== (END) ======== |----------------------------------------------------------------------------*//
            //*----------------------------------------------------------|_________________________|----------------------------------------------------------------------------*//
            //*-----------------------------------------------------------------------------------------------------------------------------------------------------------------*//








            //*restore board
            boardPosition = backUpCopyOfBoard;

            //*return 0 if time is up
            if (stopped)
                return 0;
            
            //*if better move is found
            if (currScore > maxScore)
            {
                //*if it is a quiet move, consider its HISTORY_MOVE_SCORE
                if (!move.captureFlag)
                {
                    //*store history move score
                    HISTORY_MOVE_SCORE[move.pieceMoved][move.toSquare] += depthLimit;
                }

                maxScore = alpha = currScore;

                //*---------------------Mark foundPVNode flag true(Needed for Principal variation search)--------------------------//
                // foundPVNode = true;

                //*------------------------------------set PV move in PV_TABLE------------------------------------
                //*reset pv line of current ply
                PV_TABLE[ply].clearSize();

                //*store this move as the first move of the pv line
                PV_TABLE[ply].push_back(move);

                //*store the moves of the next ply as the next moves of this pv line
                for (int i = 0; i < PV_TABLE[ply + 1].size(); i++)
                    PV_TABLE[ply].push_back(PV_TABLE[ply + 1][i]);

                //*as in special positions like check, we increased depth and searched, so in that case PV Line becomes longer becuase we search for extra depth , so set the original depth
                PV_TABLE[ply].setSize(oldDepthLimit);

                if (ply == 0)
                    BEST_MOVE = move; //*current move is the best move
            }

            //*fail hard beta-cutoff  (move fails as it is too good (i.e, alpha >= beta) )
            if (alpha >= beta)
            {
                //*for quiet move, store it as killer move
                if (!move.captureFlag)
                {
                    //*store this move as killer move, since it caused the beta cut off , and also make the prev 1st killer move as 2nd killer move
                    KILLER_MOVES[1][ply] = KILLER_MOVES[0][ply];
                    KILLER_MOVES[0][ply] = move;
                }

                return beta;
            }

            //*increment searchedMoveCount
            searchedMoveCount++;
        }

        //*if current player doesnt have any legal moves
        if (legalMoves == 0)
        {
            //*if king is in check, then it is checkmate
            if (inCheck)
            {
                //*Important : "- depthLimit" is needed to find the nearest checkmate , becuase if there are 2 checkmates at depth 3 and depth 7 , we need to return specifically the checkmate at depth 3 , and at depth 3 depthLimit is higher than at depth 7
                return (-49000 - depthLimit);
            }
            //*stalemate
            else
            {
                return 0;
            }
        }

        //*return best score
        return alpha;
    }

    //*resets the tables of the engine
    void inline resetTablesOfSearch()
    {
        nodeCount = 0;
        BEST_MOVE = Move::INVALID_MOVE;

        //*TODO:
        stopped = false;

        //*reset KILLER_MOVE table
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < MAX_DEPTH; j++)
                KILLER_MOVES[i][j] = Move::INVALID_MOVE;
        }

        //*reset history move scores
        for (int i = 0; i < 12; i++)
        {
            for (int j = 0; j < 64; j++)
                HISTORY_MOVE_SCORE[i][j] = 0;
        }

        //*reset PV table
        for (int i = 0; i < MAX_DEPTH; i++)
        {
            PV_TABLE[i].reset();
        }
    }



    //*find the best move upto given depth
    void inline searchPosition(int depth)
    {
        //*IMPORTANT: Dont forget to reset the tables, otherwise middle game performance will be so much dropped
        resetTablesOfSearch();

        int res = negamax(depth, -50000, 50000, 0, true);

        if (BEST_MOVE == Move::INVALID_MOVE)
        {
            cout << "\n\nProblem searchPostion : INVALID_MOVE returned\n\n";
            return;
        }
        else
        {
            cout << "info score cp " << res << " depth " << depth << " nodes " << nodeCount;

            //*print Principal variation line
            cout << " pv ";
            //*TODO: print pv moves
            for (int i = 0; i < PV_TABLE[0].size(); i++)
            {
                cout << PV_TABLE[0][i].getUCIMove();

                if (i != PV_TABLE[0].size() - 1)
                    cout << " ";
            }
            cout << endl;
            cout << "bestmove " << BEST_MOVE.getUCIMove() << endl;
        }
    }







    //*-----------------------------------------------------------------------------------------------------------------------------------------------------------------*//
    //*-----------------------------------------------------------_________________________-----------------------------------------------------------------------------*//
    //*----------------------------------------------------------|       ITERATIVE         |----------------------------------------------------------------------------*//
    //*----------------------------------------------------------|       DEEPENING         |----------------------------------------------------------------------------*//
    //*----------------------------------------------------------|        SEARCH           |----------------------------------------------------------------------------*//
    //*----------------------------------------------------------|_________________________|----------------------------------------------------------------------------*//
    //*-----------------------------------------------------------------------------------------------------------------------------------------------------------------*//
    /*
    *   It serves two purposes:
    *       1. Time Management: Instead of searching upto a specific depth limit, we can keep a best move
    *                           by searching for each of the depths up to the specific depth limit. 
    *                           Thus the engine will have a best move ready, whenever we ask the engine. 
    * 
    *       2. Using PV line for next search: We can search the PV line of previous search first to get more cut offs.
    *   
    *   Time Complexity: 
    *   Asymptotic time complexity remains the same as normal search.
    * 
    */

    //*find the best move upto given depth
    void inline searchPositionIterativeDeepening(int depth)
    {
        // long long totalNodes = 0;

        //*IMPORTANT: Dont forget to reset the tables, otherwise middle game performance will be so much dropped
        resetTablesOfSearch();

        long long currDepthNodeCount = 0;



        /*
         *   IMPORTANT: if the search was stopped because of time exceeding or any
         *              other reason, then best move should be the last depth search's best moove, don't
         *              print the best move of the last incomplete search which was stopped
         *
         */
        Move prevDepthSearchBestMove = Move::INVALID_MOVE;

        for (int currDepth = 1; currDepth <= depth; currDepth++)
        {
            //TODO: comment this
            // Engine::resetTablesOfSearch();

            //*each start position of each search follows PV line
            int res = negamax(currDepth, -50000, 50000, 0, true);

            if (stopped)
            {
                // cout << "stopped" << endl;
                break;
            }

            if (BEST_MOVE == Move::INVALID_MOVE)
            {
                cout << "\n\nProblem searchPositionIterativeDeepening : INVALID_MOVE returned\n\n";
                return;
            }
            else
            {
                // totalNodes += res.nodeCount;

                currDepthNodeCount = nodeCount - currDepthNodeCount;

                cout << "info score cp " << res << " depth " << currDepth << " nodes " << currDepthNodeCount << " time " << (getTimeInMilliSeconds() - starttime);

                //*
                prevDepthSearchBestMove = BEST_MOVE;

                //*print Principal variation line
                cout << " pv ";
                //*TODO: print pv moves
                for (int i = 0; i < PV_TABLE[0].size(); i++)
                {
                    cout << PV_TABLE[0][i].getUCIMove();

                    if (i != PV_TABLE[0].size() - 1)
                        cout << " ";
                }
                cout << endl;
            }
        }

        /*
         *   IMPORTANT: if the search was stopped because of time exceeding or any other reason, then best move should be the last depth search's best moove, don't
         *               print the best move of the last incomplete search which was stopped
         *
         */
        if (!this->stopped)
            cout << "bestmove " << BEST_MOVE.getUCIMove() << endl;
        else
            cout << "bestmove " << prevDepthSearchBestMove.getUCIMove() << endl;

        cout << "\nTotal Nodes of iterative deepening : " << nodeCount << endl;
    }
};







//*-----------------------------------------------------------------------------------------------------------------------------------------------------------------*//
//*-----------------------------------------------------------_________________________-----------------------------------------------------------------------------*//
//*----------------------------------------------------------|       INITIALIZE        |----------------------------------------------------------------------------*//
//*----------------------------------------------------------|        TABLES           |----------------------------------------------------------------------------*//
//*----------------------------------------------------------|_________________________|----------------------------------------------------------------------------*//
//*-----------------------------------------------------------------------------------------------------------------------------------------------------------------*//

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
*                   Pawn Knight Bishop   Rook  Queen   King (Victims)
*   
*       Pawn        105    205    305    405    505    605
*       Knight      104    204    304    404    504    604
*       Bishop      103    203    303    403    503    603
*       Rook        102    202    302    402    502    602
*       Queen       101    201    301    401    501    601
*       King        100    200    300    400    500    600
*   (Attackers)
*   
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






