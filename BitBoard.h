#pragma once

#include "neededHeaders.h"

using namespace std;

#include "definitions.h"
#include "PieceClass.h"

//*----------------IMPORTANT--------------------------------------*//
//* always create any BitBoard object before using the static lookup tables , so that they are initialized

/*
    * BitBoard index :
    *
    *Rank-> 8   0  1  2  3  4  5  6  7
    *       7   8  9  10 11 12 13 14 15
    *       6   16 17 18 19 20 21 22 23
    *       5   24 25 26 27 28 29 30 31
    *       4   32 33 34 35 36 37 38 39
    *       3   40 41 42 43 44 45 46 47
    *       2   48 49 50 51 52 53 54 55
    *       1   56 57 58 59 60 61 62 63
    *
    *           a  b  c  d  e  f  g  h  -----> Files


*/
class BitBoard
{
    U64 value;

    //*---------------------lookup tables-------------------------------*//
    static bool lookupTablesInitialized;

    static BitBoard BISHOP_ATTACK_MASK[64];
    static BitBoard ROOK_ATTACK_MASK[64];
    static int BISHOP_ATTACK_MASK_SET_BIT_COUNT[64];
    static int ROOK_ATTACK_MASK_SET_BIT_COUNT[64];
    static U64 BISHOP_MAGIC_NUMBER[64];
    static U64 ROOK_MAGIC_NUMBER[64];

public:
    static BitBoard PAWN_ATTACK_TABLE[2][64];
    static BitBoard KNIGHT_ATTACK_TABLE[64];
    static BitBoard KING_ATTACK_TABLE[64];

    //* BISHOP_ATTACK_TABLE[squareIndex][occupancyBitBoard], size = 64 * 512 (--------------------------- to be initialized---------------------------)
    static vector<vector<BitBoard>> BISHOP_ATTACK_TABLE; // *size = 64 * 512

    //* ROOK_ATTACK_TABLE[squareIndex][occupancyBitBoard] , size = 64 * 4096 (--------------------------- to be initialized---------------------------)
    static vector<vector<BitBoard>> ROOK_ATTACK_TABLE; //*size = 64 * 4096

public:
    //*-------------------------------Constructor----------------------------------------*//

    //*Important : For creating Normal BitBoard objects to use , never pass false as argument "initializeLookupTables" ---> this argument is for internal look up table initialization to stop infinite recursive calls

    BitBoard(U64 value = 0ULL, bool initializeLookupTables = true)
    {
        this->value = value;

        //*IMPORTANT : this is for initializing the look up tables . To initialize the look up table i have to use the constructor , so to avoid infinite recursive calls, i have to separate the constructor for initialization of look up tables and creating normal BitBoard objects to use
        if (initializeLookupTables)
        {
            //*if attack tables were not initialized , then initialize them
            if (BitBoard::lookupTablesInitialized == false)
            {
                BitBoard::lookupTablesInitialized = true;
                BitBoard::initializeAllLookupTables();
            }
        }
    }

    static char inline getFileOfSquareIndex(int index)
    {
        return 'a' + index % 8;
    }

    static int inline getRankOfSquareIndex(int index)
    {
        return 8 - (index / 8);
    }

    //*rank in [1,8] and file in ['a', 'h']
    static int inline getIndexFromRankAndFile(int rank, char file)
    {
        return (8 - rank) * 8 + (file - 'a');
    }

    // *row and column numbers are 0 based
    static int inline getIndexFromRowAndColumnNumbers(int row, int column)
    {
        return row * 8 + column;
    }

    static std::string inline getAlgebraicCoordinateFromIndex(int index)
    {
        return BitBoard::getFileOfSquareIndex(index) + std::to_string(BitBoard::getRankOfSquareIndex(index));
    }

    string inline to_string()
    {
        return std::to_string(this->value);
    }

    //* Dont use these 2 functions after testing, to reduce number of function calls
    static bool inline isSquareIndexValid(int squareIndex)
    {
        return (0 <= squareIndex && squareIndex < 64);
    }

    static void inline checkSquareIndexValidity(int squareIndex)
    {
        if (!BitBoard::isSquareIndexValid(squareIndex))
            throw runtime_error("Square index out of bound inside getBitAt()");
    }

    U64 inline getDecimalValue() const
    {
        return this->value;
    }

    int inline getBitAt(int squareIndex) const
    {
        // BitBoard::checkSquareIndexValidity(squareIndex);
        assert(0 <= squareIndex && squareIndex < 64);

        return ((this->value & (1ULL << squareIndex)) == 0) ? 0 : 1;
    }

    void inline setBitAt(int squareIndex)
    {
        // BitBoard::checkSquareIndexValidity(squareIndex);
        assert(0 <= squareIndex && squareIndex < 64);

        this->value = (this->value | (1ULL << squareIndex));
    }

    void inline toggleBitAt(int squareIndex)
    {
        // BitBoard::checkSquareIndexValidity(squareIndex);
        assert(0 <= squareIndex && squareIndex < 64);

        this->value = (this->value ^ (1ULL << squareIndex));
    }

    void inline unsetBitAt(int squareIndex)
    {
        // BitBoard::checkSquareIndexValidity(squareIndex);
        assert(0 <= squareIndex && squareIndex < 64);

        if (getBitAt(squareIndex) == 1)
            toggleBitAt(squareIndex);
    }

    inline int countSetBits() const
    {
        U64 x = this->value;
        int count = 0;
        while (x)
        {
            count++;
            x &= x - 1;
        }
        return count;

        // dont use function outside the class
        //  return countSetBitsOfU64(this->value);
    }

    inline int getFirstLeastSignificantBitIndexFromRight() const
    {
        U64 x = this->value;
        if (x == 0)
            return -1;

        BitBoard temp = ((x & -x) - 1);

        return temp.countSetBits();

        // return getFirstLeastSignificantBitIndexFromRightU64(this->value);
    }

    void inline printWithoutBoxes() const
    {
        for (int i = 0; i < 8; i++)
        {
            // print rank number
            int rank = 8 - i;
            cout << rank << "    ";

            for (int j = 0; j < 8; j++)
            {
                int file = j;
                int index = i * 8 + j;
                cout << this->getBitAt(index) << "  ";
            }
            cout << endl;
        }

        // print files
        cout << "\n     a  b  c  d  e  f  g  h";

        // print decimal value of the bitboard
        cout << "\n\n    Decimal value : " << this->value << endl
             << endl;
    }

    void inline print(bool withZeros = false) const
    {
        // cout << "+---+---+---+---+---+---+---+---+" << endl;
        cout << endl;
        for (int i = 0; i < 8; i++)
        {
            // print rank number
            int rank = 8 - i;
            // cout << rank << "    ";

            cout << "+---+---+---+---+---+---+---+---+" << endl;

            for (int j = 0; j < 8; j++)
            {
                int file = j;
                int index = i * 8 + j;

                int bit = this->getBitAt(index);

                cout << "| ";
                cout << (bit == 1 ? "1" : (withZeros ? "0" : " "));
                cout << " ";
            }
            cout << "|";
            cout << "   " << rank << endl;
        }

        cout << "+---+---+---+---+---+---+---+---+---+---+" << endl;

        cout << "  a   b   c   d   e   f   g   h   --------> File";

        // print decimal value of the bitboard
        cout << "\n\n    Decimal value : " << this->value << endl
             << endl;
    }

    static BitBoard inline getPawnAttacks(int squareIndex, int playerColor)
    {
        // BitBoard::checkSquareIndexValidity(squareIndex);
        assert(0 <= squareIndex && squareIndex < 64);

        return PAWN_ATTACK_TABLE[playerColor][squareIndex];
    }

    static BitBoard inline getKnightAttacks(int squareIndex)
    {
        // BitBoard::checkSquareIndexValidity(squareIndex);
        assert(0 <= squareIndex && squareIndex < 64);

        return KNIGHT_ATTACK_TABLE[squareIndex];
    }

    static BitBoard inline getKingAttacks(int squareIndex)
    {
        // BitBoard::checkSquareIndexValidity(squareIndex);
        assert(0 <= squareIndex && squareIndex < 64);

        return KING_ATTACK_TABLE[squareIndex];
    }

    //*Finds bishop attacks assuming current board occupancy as "blockers"
    static BitBoard inline getBishopAttacks(int squareIndex, const BitBoard &blockers)
    {
        // BitBoard::checkSquareIndexValidity(squareIndex);
        assert(0 <= squareIndex && squareIndex < 64);

        U64 occupancyValue = blockers.getDecimalValue();

        occupancyValue &= BitBoard::BISHOP_ATTACK_MASK[squareIndex].getDecimalValue();
        occupancyValue *= BitBoard::BISHOP_MAGIC_NUMBER[squareIndex];
        occupancyValue >>= (64 - BitBoard::BISHOP_ATTACK_MASK_SET_BIT_COUNT[squareIndex]);

        U64 index = occupancyValue;
        return BitBoard::BISHOP_ATTACK_TABLE[squareIndex][index];
    }

    //*Finds rook attacks assuming current board occupancy as "blockers"
    static BitBoard inline getRookAttacks(int squareIndex, const BitBoard &blockers)
    {
        // BitBoard::checkSquareIndexValidity(squareIndex);
        assert(0 <= squareIndex && squareIndex < 64);

        U64 occupancyValue = blockers.getDecimalValue();

        occupancyValue &= BitBoard::ROOK_ATTACK_MASK[squareIndex].getDecimalValue();
        occupancyValue *= BitBoard::ROOK_MAGIC_NUMBER[squareIndex];
        occupancyValue >>= (64 - BitBoard::ROOK_ATTACK_MASK_SET_BIT_COUNT[squareIndex]);

        U64 index = occupancyValue;
        return BitBoard::ROOK_ATTACK_TABLE[squareIndex][index];
    }

    //*Finds queen attacks assuming current board occupancy as "blockers"
    static BitBoard inline getQueenAttacks(int squareIndex, const BitBoard &blockers)
    {
        // BitBoard::checkSquareIndexValidity(squareIndex);
        assert(0 <= squareIndex && squareIndex < 64);

        U64 res = (BitBoard::getBishopAttacks(squareIndex, blockers).getDecimalValue()) | (BitBoard::getRookAttacks(squareIndex, blockers).getDecimalValue());
        return BitBoard(res);
    }

    /*
     *finds the attacks of given piece from given square
     *Dont Forget to pass occupancy for slider pieces
     */
    static BitBoard inline getPieceAttacks(int squareIndex, int piece, const BitBoard &occupancy = BitBoard(0ULL))
    {
        // if (piece < Piece::P || piece > Piece::k)
        //     throw runtime_error("\n\ninvalid piece inside getPieceAttacks() function of BitBoard class\n\n");

        assert(!(piece < Piece::P || piece > Piece::k));

        switch (piece)
        {
        case Piece::P:
            return PAWN_ATTACK_TABLE[WHITE][squareIndex];
        case Piece ::p:
            return PAWN_ATTACK_TABLE[BLACK][squareIndex];

        case Piece ::N:
        case Piece::n:
            return KNIGHT_ATTACK_TABLE[squareIndex];

        case Piece ::B:
        case Piece::b:
            return getBishopAttacks(squareIndex, occupancy);

        case Piece ::R:
        case Piece::r:
            return getRookAttacks(squareIndex, occupancy);

        case Piece ::Q:
        case Piece::q:
            return getQueenAttacks(squareIndex, occupancy);

        case Piece ::K:
        case Piece::k:
            return KING_ATTACK_TABLE[squareIndex];

        default:
            throw runtime_error("\n\nproblem inside getPieceAttacks() function of BitBoard Class\n\n");
        }
    }

private:
    //*---------------------------------FOR INITIALIZING LOOK UP TABLES----------------------------------------------------

    //*-------------IMPORTANT : While constructing any bitboard object from U64 inside any code for initializeing look up tables , pass the 2nd argument of constructor as false
    //*------------------------ so that recursively the initializeAllLookUpTables() function is not again called

    /*
        generates attacks upto the blocker piece. i.e, drops the squares in the attack ray after hitting the blocker piece

        example :
    blocker bitboard :
    +---+---+---+---+---+---+---+---+
    | 1 |   |   |   |   |   |   |   |   8
    +---+---+---+---+---+---+---+---+
    |   |   |   |   |   |   | 1 |   |   7
    +---+---+---+---+---+---+---+---+
    |   |   |   |   |   |   |   |   |   6
    +---+---+---+---+---+---+---+---+
    |   |   | 1 |   |   |   |   |   |   5
    +---+---+---+---+---+---+---+---+
    |   |   |   |   |   |   |   |   |   4
    +---+---+---+---+---+---+---+---+
    |   |   |   |   |   |   |   |   |   3
    +---+---+---+---+---+---+---+---+
    |   |   |   |   |   |   |   |   |   2
    +---+---+---+---+---+---+---+---+
    | 1 |   |   |   |   |   |   | 1 |   1
    +---+---+---+---+---+---+---+---+---+---+
      a   b   c   d   e   f   g   h   --------> File

        Decimal value : 9295429630959828993

    bishop attacks :
    +---+---+---+---+---+---+---+---+
    |   |   |   |   |   |   |   |   |   8
    +---+---+---+---+---+---+---+---+
    |   |   |   |   |   |   | 1 |   |   7
    +---+---+---+---+---+---+---+---+
    |   |   |   |   |   | 1 |   |   |   6
    +---+---+---+---+---+---+---+---+
    |   |   | 1 |   | 1 |   |   |   |   5
    +---+---+---+---+---+---+---+---+
    |   |   |   |   |   |   |   |   |   4
    +---+---+---+---+---+---+---+---+
    |   |   | 1 |   | 1 |   |   |   |   3
    +---+---+---+---+---+---+---+---+
    |   | 1 |   |   |   | 1 |   |   |   2
    +---+---+---+---+---+---+---+---+
    | 1 |   |   |   |   |   | 1 |   |   1
    +---+---+---+---+---+---+---+---+---+---+
      a   b   c   d   e   f   g   h   --------> File

        Decimal value : 4693335752243691520
    */
    //*including the edge of the board and occupancy of blocker pieces
    static BitBoard generateBishopAttacksOnTheFly(int squareIndex, const BitBoard &blockers)
    {
        // BitBoard::checkSquareIndexValidity(squareIndex);
        assert(0 <= squareIndex && squareIndex < 64);

        BitBoard result(0ULL, false);
        // BitBoard::checkSquareIndexValidity(squareIndex);
        assert(0 <= squareIndex && squareIndex < 64);

        int targetIndex = -1;

        int currentRank = BitBoard::getRankOfSquareIndex(squareIndex);
        char currentFile = BitBoard::getFileOfSquareIndex(squareIndex);

        int rank = 0;
        char file = 0;

        //*upper right diagonal
        for (rank = currentRank + 1, file = currentFile + 1; rank <= 8 && file <= 'h'; rank++, file++)
        {
            targetIndex = BitBoard::getIndexFromRankAndFile(rank, file);
            result.setBitAt(targetIndex);

            if (blockers.getBitAt(targetIndex) == 1)
                break;
        }

        //*lower right diagonal
        for (rank = currentRank - 1, file = currentFile + 1; rank >= 1 && file <= 'h'; rank--, file++)
        {
            targetIndex = BitBoard::getIndexFromRankAndFile(rank, file);
            result.setBitAt(targetIndex);

            if (blockers.getBitAt(targetIndex) == 1)
                break;
        }

        //*upper left diagonal
        for (rank = currentRank + 1, file = currentFile - 1; rank <= 8 && file >= 'a'; rank++, file--)
        {
            targetIndex = BitBoard::getIndexFromRankAndFile(rank, file);
            result.setBitAt(targetIndex);

            if (blockers.getBitAt(targetIndex) == 1)
                break;
        }

        //*lower left diagonal
        for (rank = currentRank - 1, file = currentFile - 1; rank >= 1 && file >= 'a'; rank--, file--)
        {
            targetIndex = BitBoard::getIndexFromRankAndFile(rank, file);
            result.setBitAt(targetIndex);

            if (blockers.getBitAt(targetIndex) == 1)
                break;
        }

        return result;
    }

    /*
        generates attacks upto the blocker piece. i.e, drops the squares in the attack ray after hitting the blocker piece

        example :


    blocker bitboard :
    +---+---+---+---+---+---+---+---+
    | 1 |   |   |   |   |   |   |   |   8
    +---+---+---+---+---+---+---+---+
    |   |   |   |   |   |   |   |   |   7
    +---+---+---+---+---+---+---+---+
    |   |   |   |   |   |   |   |   |   6
    +---+---+---+---+---+---+---+---+
    |   |   |   | 1 |   |   |   |   |   5
    +---+---+---+---+---+---+---+---+
    |   | 1 |   |   |   |   | 1 |   |   4
    +---+---+---+---+---+---+---+---+
    |   |   |   |   |   |   |   |   |   3
    +---+---+---+---+---+---+---+---+
    |   |   |   | 1 |   |   |   |   |   2
    +---+---+---+---+---+---+---+---+
    |   |   |   |   |   |   |   |   |   1
    +---+---+---+---+---+---+---+---+---+---+
      a   b   c   d   e   f   g   h   --------> File

        Decimal value : 2252083415744513


    Rook attacks :
    +---+---+---+---+---+---+---+---+
    |   |   |   |   |   |   |   |   |   8
    +---+---+---+---+---+---+---+---+
    |   |   |   |   |   |   |   |   |   7
    +---+---+---+---+---+---+---+---+
    |   |   |   |   |   |   |   |   |   6
    +---+---+---+---+---+---+---+---+
    |   |   |   | 1 |   |   |   |   |   5
    +---+---+---+---+---+---+---+---+
    |   | 1 | 1 |   | 1 | 1 | 1 |   |   4
    +---+---+---+---+---+---+---+---+
    |   |   |   | 1 |   |   |   |   |   3
    +---+---+---+---+---+---+---+---+
    |   |   |   | 1 |   |   |   |   |   2
    +---+---+---+---+---+---+---+---+
    |   |   |   |   |   |   |   |   |   1
    +---+---+---+---+---+---+---+---+---+---+
      a   b   c   d   e   f   g   h   --------> File

        Decimal value : 2261102847066112
    */

    //*Exludes the edge of the board
    static BitBoard generateRookAttacksOnTheFly(int squareIndex, const BitBoard &blockers)
    {
        // BitBoard::checkSquareIndexValidity(squareIndex);
        assert(0 <= squareIndex && squareIndex < 64);

        BitBoard result(0ULL, false);
        // BitBoard::checkSquareIndexValidity(squareIndex);
        assert(0 <= squareIndex && squareIndex < 64);

        int targetIndex = -1;

        int currentRank = BitBoard::getRankOfSquareIndex(squareIndex);
        char currentFile = BitBoard::getFileOfSquareIndex(squareIndex);

        // cout << "\ncurrentRank = " << currentRank;

        int rank = currentRank;
        char file = currentFile;

        //*upper file
        for (rank = currentRank + 1; rank <= 8; rank++)
        {
            targetIndex = BitBoard::getIndexFromRankAndFile(rank, currentFile);
            result.setBitAt(targetIndex);

            if (blockers.getBitAt(targetIndex) == 1)
                break;
        }

        //*lower file
        for (rank = currentRank - 1; rank >= 1; rank--)
        {
            targetIndex = BitBoard::getIndexFromRankAndFile(rank, currentFile);
            result.setBitAt(targetIndex);

            if (blockers.getBitAt(targetIndex) == 1)
                break;
        }

        //*left rank
        for (file = currentFile - 1; file >= 'a'; file--)
        {
            targetIndex = BitBoard::getIndexFromRankAndFile(currentRank, file);
            result.setBitAt(targetIndex);

            if (blockers.getBitAt(targetIndex) == 1)
                break;
        }

        //*right rank
        for (file = currentFile + 1; file <= 'h'; file++)
        {
            targetIndex = BitBoard::getIndexFromRankAndFile(currentRank, file);
            result.setBitAt(targetIndex);

            if (blockers.getBitAt(targetIndex) == 1)
                break;
        }

        return result;
    }

    /*
     TODO: Dunno much about this function for now :) , gotta research later
     */
    static BitBoard setOccupancy(int index, BitBoard attackMask)
    {
        // occupancy mask
        BitBoard occupancy(0ULL, false);

        int numberOfSetBits = attackMask.countSetBits();

        // loop over the range of set bits of attack masks
        for (int count = 0; count < numberOfSetBits; count++)
        {
            // get the index of the Least significant set bit within our attack mask
            int squareIndex = attackMask.getFirstLeastSignificantBitIndexFromRight();

            // pop the Least significant set bit
            attackMask.unsetBitAt(squareIndex);

            // make sure occupancy is on board
            if (index & (1 << count)) // TODO: Dunno
                // populate occupancy mask
                occupancy.setBitAt(squareIndex);
        }

        return occupancy;
    }

    static void inline initializeSliderPieceAttackTables()
    {
        //*make it true so that the attack tables are not initialized for every object creation of BitBoard class
        BitBoard::lookupTablesInitialized = true;

        //*assign values to the attack tables

        for (int squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            //*for bishop_____________________________________________________________________________
            int relevantBitCount = BitBoard::BISHOP_ATTACK_MASK_SET_BIT_COUNT[squareIndex];
            // initialize occupancyIndex
            int occupancyIndex = (1 << relevantBitCount);

            for (int index = 0; index < occupancyIndex; index++)
            {
                // initilalize current occupancy variation
                BitBoard occupancy = BitBoard::setOccupancy(index, BitBoard::BISHOP_ATTACK_MASK[squareIndex]);

                // initialize magic index
                U64 magicIndex = (occupancy.getDecimalValue() * BitBoard::BISHOP_MAGIC_NUMBER[squareIndex]) >> (64 - relevantBitCount);

                // initialize bishop attacks
                BitBoard::BISHOP_ATTACK_TABLE[squareIndex][magicIndex] = BitBoard::generateBishopAttacksOnTheFly(squareIndex, occupancy);
            }

            //*for rook_____________________________________________________________________________
            relevantBitCount = BitBoard::ROOK_ATTACK_MASK_SET_BIT_COUNT[squareIndex];
            // initialize occupancyIndex
            occupancyIndex = (1 << relevantBitCount);

            for (int index = 0; index < occupancyIndex; index++)
            {
                // initilalize current occupancy variation
                //*copy constructor will not call initializeAllLookupTables()
                BitBoard occupancy(BitBoard::setOccupancy(index, BitBoard::ROOK_ATTACK_MASK[squareIndex]));

                // initialize magic index
                U64 magicIndex = (occupancy.getDecimalValue() * BitBoard::ROOK_MAGIC_NUMBER[squareIndex]) >> (64 - relevantBitCount);

                // initialize bishop attacks
                BitBoard::ROOK_ATTACK_TABLE[squareIndex][magicIndex] = BitBoard::generateRookAttacksOnTheFly(squareIndex, occupancy);
            }
        }
    }

    static void initializeAllLookupTables()
    {
        //*TODO: uncomment the printer line after testing :)
        cout << "\ninitializeAllLookupTables() called\n";

        // BitBoard::initializePrecomputedLookupTables();
        BitBoard::initializeSliderPieceAttackTables();
    }

    //*-----------------------------------------------friend functions for testing----------------------------------------------------
    friend U64 findMagicNumber(int squareIndex, bool bishop);

    //*-----------------------------------------------friend functions for testing----------------------------------------------------
};

//
//
//

//
//
//
//
//
//
//*-------------------------Define LOOKUP TABLES______________________________________

//*-------------------------important--------------------------------------------------------------------------------
bool BitBoard::lookupTablesInitialized = false;
//*-------------------------important--------------------------------------------------------------------------------

/////
//
//
//
//
//
//
//
//
//
//*-----------------------------------------------------------------IMPORTANT-----------------------------------------------------------*//
//*-----------------------------------------------------------------IMPORTANT-----------------------------------------------------------*//
//*-----------------------------------------------------------------IMPORTANT-----------------------------------------------------------*//

//*IMPORTANT : Whenever the parameterized constructor is called while initializing the loop up tables, always pass false as the value of "initializeLookupTables" argument of the constructor

//*IMPORTANT : The order of initializations of these tables matter, as some of the tables depend on other tables

//*-----------------------------------------------------------------IMPORTANT-----------------------------------------------------------*//
//*-----------------------------------------------------------------IMPORTANT-----------------------------------------------------------*//

//*Exludes the edge of the board , size = 64
BitBoard BitBoard::BISHOP_ATTACK_MASK[64] =
    {BitBoard(18049651735527936ULL, false), BitBoard(70506452091904ULL, false), BitBoard(275415828992ULL, false), BitBoard(1075975168ULL, false), BitBoard(38021120ULL, false), BitBoard(8657588224ULL, false), BitBoard(2216338399232ULL, false), BitBoard(567382630219776ULL, false), BitBoard(9024825867763712ULL, false), BitBoard(18049651735527424ULL, false), BitBoard(70506452221952ULL, false), BitBoard(275449643008ULL, false), BitBoard(9733406720ULL, false), BitBoard(2216342585344ULL, false), BitBoard(567382630203392ULL, false), BitBoard(1134765260406784ULL, false), BitBoard(4512412933816832ULL, false), BitBoard(9024825867633664ULL, false), BitBoard(18049651768822272ULL, false), BitBoard(70515108615168ULL, false), BitBoard(2491752130560ULL, false), BitBoard(567383701868544ULL, false), BitBoard(1134765256220672ULL, false), BitBoard(2269530512441344ULL, false), BitBoard(2256206450263040ULL, false), BitBoard(4512412900526080ULL, false), BitBoard(9024834391117824ULL, false), BitBoard(18051867805491712ULL, false), BitBoard(637888545440768ULL, false), BitBoard(1135039602493440ULL, false), BitBoard(2269529440784384ULL, false), BitBoard(4539058881568768ULL, false), BitBoard(1128098963916800ULL, false), BitBoard(2256197927833600ULL, false), BitBoard(4514594912477184ULL, false), BitBoard(9592139778506752ULL, false), BitBoard(19184279556981248ULL, false), BitBoard(2339762086609920ULL, false), BitBoard(4538784537380864ULL, false), BitBoard(9077569074761728ULL, false), BitBoard(562958610993152ULL, false), BitBoard(1125917221986304ULL, false), BitBoard(2814792987328512ULL, false), BitBoard(5629586008178688ULL, false), BitBoard(11259172008099840ULL, false), BitBoard(22518341868716544ULL, false), BitBoard(9007336962655232ULL, false), BitBoard(18014673925310464ULL, false), BitBoard(2216338399232ULL, false), BitBoard(4432676798464ULL, false), BitBoard(11064376819712ULL, false), BitBoard(22137335185408ULL, false), BitBoard(44272556441600ULL, false), BitBoard(87995357200384ULL, false), BitBoard(35253226045952ULL, false), BitBoard(70506452091904ULL, false), BitBoard(567382630219776ULL, false), BitBoard(1134765260406784ULL, false), BitBoard(2832480465846272ULL, false), BitBoard(5667157807464448ULL, false), BitBoard(11333774449049600ULL, false), BitBoard(22526811443298304ULL, false), BitBoard(9024825867763712ULL, false), BitBoard(18049651735527936ULL, false)};

//*Exludes the edge of the board
/*
example :

for squareINdex = d4

+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   8
+---+---+---+---+---+---+---+---+
|   |   |   | 1 |   |   |   |   |   7
+---+---+---+---+---+---+---+---+
|   |   |   | 1 |   |   |   |   |   6
+---+---+---+---+---+---+---+---+
|   |   |   | 1 |   |   |   |   |   5
+---+---+---+---+---+---+---+---+
|   | 1 | 1 |   | 1 | 1 | 1 |   |   4
+---+---+---+---+---+---+---+---+
|   |   |   | 1 |   |   |   |   |   3
+---+---+---+---+---+---+---+---+
|   |   |   | 1 |   |   |   |   |   2
+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   1
+---+---+---+---+---+---+---+---+---+---+
  a   b   c   d   e   f   g   h   --------> File
*/
// *size = 64
BitBoard BitBoard::ROOK_ATTACK_MASK[64] =
    {BitBoard(282578800148862ULL, false), BitBoard(565157600297596ULL, false), BitBoard(1130315200595066ULL, false), BitBoard(2260630401190006ULL, false), BitBoard(4521260802379886ULL, false), BitBoard(9042521604759646ULL, false), BitBoard(18085043209519166ULL, false), BitBoard(36170086419038334ULL, false), BitBoard(282578800180736ULL, false), BitBoard(565157600328704ULL, false), BitBoard(1130315200625152ULL, false), BitBoard(2260630401218048ULL, false), BitBoard(4521260802403840ULL, false), BitBoard(9042521604775424ULL, false), BitBoard(18085043209518592ULL, false), BitBoard(36170086419037696ULL, false), BitBoard(282578808340736ULL, false), BitBoard(565157608292864ULL, false), BitBoard(1130315208328192ULL, false), BitBoard(2260630408398848ULL, false), BitBoard(4521260808540160ULL, false), BitBoard(9042521608822784ULL, false), BitBoard(18085043209388032ULL, false), BitBoard(36170086418907136ULL, false), BitBoard(282580897300736ULL, false), BitBoard(565159647117824ULL, false), BitBoard(1130317180306432ULL, false), BitBoard(2260632246683648ULL, false), BitBoard(4521262379438080ULL, false), BitBoard(9042522644946944ULL, false), BitBoard(18085043175964672ULL, false), BitBoard(36170086385483776ULL, false), BitBoard(283115671060736ULL, false), BitBoard(565681586307584ULL, false), BitBoard(1130822006735872ULL, false), BitBoard(2261102847592448ULL, false), BitBoard(4521664529305600ULL, false), BitBoard(9042787892731904ULL, false), BitBoard(18085034619584512ULL, false), BitBoard(36170077829103616ULL, false), BitBoard(420017753620736ULL, false), BitBoard(699298018886144ULL, false), BitBoard(1260057572672512ULL, false), BitBoard(2381576680245248ULL, false), BitBoard(4624614895390720ULL, false), BitBoard(9110691325681664ULL, false), BitBoard(18082844186263552ULL, false), BitBoard(36167887395782656ULL, false), BitBoard(35466950888980736ULL, false), BitBoard(34905104758997504ULL, false), BitBoard(34344362452452352ULL, false), BitBoard(33222877839362048ULL, false), BitBoard(30979908613181440ULL, false), BitBoard(26493970160820224ULL, false), BitBoard(17522093256097792ULL, false), BitBoard(35607136465616896ULL, false), BitBoard(9079539427579068672ULL, false), BitBoard(8935706818303361536ULL, false), BitBoard(8792156787827803136ULL, false), BitBoard(8505056726876686336ULL, false), BitBoard(7930856604974452736ULL, false), BitBoard(6782456361169985536ULL, false), BitBoard(4485655873561051136ULL, false), BitBoard(9115426935197958144ULL, false)};

//*relevant occupancy set bit count for every square on board for bishop , size = 64
int BitBoard::BISHOP_ATTACK_MASK_SET_BIT_COUNT[64] = {6, 5, 5, 5, 5, 5, 5, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7, 7, 7, 7, 5, 5, 5, 5, 7, 9, 9, 7, 5, 5, 5, 5, 7, 9, 9, 7, 5, 5, 5, 5, 7, 7, 7, 7, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 5, 5, 5, 5, 5, 5, 6};

//*relevant occupancy set bit count for every square on board for rook , size = 64
int BitBoard::ROOK_ATTACK_MASK_SET_BIT_COUNT[64] = {12, 11, 11, 11, 11, 11, 11, 12, 11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11, 12, 11, 11, 11, 11, 11, 11, 12};

//*Magic numbers for Bishop , size = 64
U64 BitBoard::BISHOP_MAGIC_NUMBER[64] = {
    0x40040844404084ULL, 0x2004208a004208ULL, 0x10190041080202ULL, 0x108060845042010ULL, 0x581104180800210ULL, 0x2112080446200010ULL, 0x1080820820060210ULL, 0x3c0808410220200ULL, 0x4050404440404ULL, 0x21001420088ULL, 0x24d0080801082102ULL, 0x1020a0a020400ULL, 0x40308200402ULL, 0x4011002100800ULL, 0x401484104104005ULL, 0x801010402020200ULL, 0x400210c3880100ULL, 0x404022024108200ULL, 0x810018200204102ULL, 0x4002801a02003ULL, 0x85040820080400ULL, 0x810102c808880400ULL, 0xe900410884800ULL, 0x8002020480840102ULL, 0x220200865090201ULL, 0x2010100a02021202ULL, 0x152048408022401ULL, 0x20080002081110ULL, 0x4001001021004000ULL, 0x800040400a011002ULL, 0xe4004081011002ULL, 0x1c004001012080ULL, 0x8004200962a00220ULL, 0x8422100208500202ULL, 0x2000402200300c08ULL, 0x8646020080080080ULL, 0x80020a0200100808ULL, 0x2010004880111000ULL, 0x623000a080011400ULL, 0x42008c0340209202ULL, 0x209188240001000ULL, 0x400408a884001800ULL, 0x110400a6080400ULL, 0x1840060a44020800ULL, 0x90080104000041ULL, 0x201011000808101ULL, 0x1a2208080504f080ULL, 0x8012020600211212ULL, 0x500861011240000ULL, 0x180806108200800ULL, 0x4000020e01040044ULL, 0x300000261044000aULL, 0x802241102020002ULL, 0x20906061210001ULL, 0x5a84841004010310ULL, 0x4010801011c04ULL, 0xa010109502200ULL, 0x4a02012000ULL, 0x500201010098b028ULL, 0x8040002811040900ULL, 0x28000010020204ULL, 0x6000020202d0240ULL, 0x8918844842082200ULL, 0x4010011029020020ULL};

//*Magic numbers for ROOK , size = 64
U64 BitBoard::ROOK_MAGIC_NUMBER[64] = {
    0x8a80104000800020ULL, 0x140002000100040ULL, 0x2801880a0017001ULL, 0x100081001000420ULL, 0x200020010080420ULL, 0x3001c0002010008ULL, 0x8480008002000100ULL, 0x2080088004402900ULL, 0x800098204000ULL, 0x2024401000200040ULL, 0x100802000801000ULL, 0x120800800801000ULL, 0x208808088000400ULL, 0x2802200800400ULL, 0x2200800100020080ULL, 0x801000060821100ULL, 0x80044006422000ULL, 0x100808020004000ULL, 0x12108a0010204200ULL, 0x140848010000802ULL, 0x481828014002800ULL, 0x8094004002004100ULL, 0x4010040010010802ULL, 0x20008806104ULL, 0x100400080208000ULL, 0x2040002120081000ULL, 0x21200680100081ULL, 0x20100080080080ULL, 0x2000a00200410ULL, 0x20080800400ULL, 0x80088400100102ULL, 0x80004600042881ULL, 0x4040008040800020ULL, 0x440003000200801ULL, 0x4200011004500ULL, 0x188020010100100ULL, 0x14800401802800ULL, 0x2080040080800200ULL, 0x124080204001001ULL, 0x200046502000484ULL, 0x480400080088020ULL, 0x1000422010034000ULL, 0x30200100110040ULL, 0x100021010009ULL, 0x2002080100110004ULL, 0x202008004008002ULL, 0x20020004010100ULL, 0x2048440040820001ULL, 0x101002200408200ULL, 0x40802000401080ULL, 0x4008142004410100ULL, 0x2060820c0120200ULL, 0x1001004080100ULL, 0x20c020080040080ULL, 0x2935610830022400ULL, 0x44440041009200ULL, 0x280001040802101ULL, 0x2100190040002085ULL, 0x80c0084100102001ULL, 0x4024081001000421ULL, 0x20030a0244872ULL, 0x12001008414402ULL, 0x2006104900a0804ULL, 0x1004081002402ULL};
//
//
//
//
//
//
//
//
//*_____________________________ATTACK TABLES____________________________________________________________________

//*PAWN ATTACK TABLE______________________________________
// pawnAttacks[playerColor][squareIndex]  , size = 2 * 64 (white or black * 64 squares)
BitBoard BitBoard::PAWN_ATTACK_TABLE[2][64] =
    {{BitBoard(0ULL, false), BitBoard(0ULL, false), BitBoard(0ULL, false), BitBoard(0ULL, false), BitBoard(0ULL, false), BitBoard(0ULL, false), BitBoard(0ULL, false), BitBoard(0ULL, false), BitBoard(2ULL, false), BitBoard(5ULL, false), BitBoard(10ULL, false), BitBoard(20ULL, false), BitBoard(40ULL, false), BitBoard(80ULL, false), BitBoard(160ULL, false), BitBoard(64ULL, false), BitBoard(512ULL, false), BitBoard(1280ULL, false),
      BitBoard(2560ULL, false), BitBoard(5120ULL, false), BitBoard(10240ULL, false), BitBoard(20480ULL, false), BitBoard(40960ULL, false), BitBoard(16384ULL, false), BitBoard(131072ULL, false), BitBoard(327680ULL, false), BitBoard(655360ULL, false), BitBoard(1310720ULL, false), BitBoard(2621440ULL, false), BitBoard(5242880ULL, false), BitBoard(10485760ULL, false), BitBoard(4194304ULL, false), BitBoard(33554432ULL, false), BitBoard(83886080ULL, false), BitBoard(167772160ULL, false), BitBoard(335544320ULL, false), BitBoard(671088640ULL, false), BitBoard(1342177280ULL, false), BitBoard(2684354560ULL, false), BitBoard(1073741824ULL, false), BitBoard(8589934592ULL, false), BitBoard(21474836480ULL, false), BitBoard(42949672960ULL, false), BitBoard(85899345920ULL, false), BitBoard(171798691840ULL, false), BitBoard(343597383680ULL, false), BitBoard(687194767360ULL, false), BitBoard(274877906944ULL, false), BitBoard(2199023255552ULL, false), BitBoard(5497558138880ULL, false), BitBoard(10995116277760ULL, false), BitBoard(21990232555520ULL, false), BitBoard(43980465111040ULL, false), BitBoard(87960930222080ULL, false), BitBoard(175921860444160ULL, false), BitBoard(70368744177664ULL, false), BitBoard(562949953421312ULL, false), BitBoard(1407374883553280ULL, false), BitBoard(2814749767106560ULL, false), BitBoard(5629499534213120ULL, false), BitBoard(11258999068426240ULL, false), BitBoard(22517998136852480ULL, false), BitBoard(45035996273704960ULL, false), BitBoard(18014398509481984ULL, false)},
     {BitBoard(512ULL, false), BitBoard(1280ULL, false), BitBoard(2560ULL, false), BitBoard(5120ULL, false), BitBoard(10240ULL, false), BitBoard(20480ULL, false), BitBoard(40960ULL, false), BitBoard(16384ULL, false), BitBoard(131072ULL, false), BitBoard(327680ULL, false), BitBoard(655360ULL, false), BitBoard(1310720ULL, false), BitBoard(2621440ULL, false), BitBoard(5242880ULL, false), BitBoard(10485760ULL, false), BitBoard(4194304ULL, false), BitBoard(33554432ULL, false), BitBoard(83886080ULL, false), BitBoard(167772160ULL, false), BitBoard(335544320ULL, false), BitBoard(671088640ULL, false), BitBoard(1342177280ULL, false), BitBoard(2684354560ULL, false), BitBoard(1073741824ULL, false), BitBoard(8589934592ULL, false), BitBoard(21474836480ULL, false), BitBoard(42949672960ULL, false), BitBoard(85899345920ULL, false), BitBoard(171798691840ULL, false), BitBoard(343597383680ULL, false), BitBoard(687194767360ULL, false), BitBoard(274877906944ULL, false), BitBoard(2199023255552ULL, false), BitBoard(5497558138880ULL, false), BitBoard(10995116277760ULL, false), BitBoard(21990232555520ULL, false), BitBoard(43980465111040ULL, false), BitBoard(87960930222080ULL, false), BitBoard(175921860444160ULL, false), BitBoard(70368744177664ULL, false), BitBoard(562949953421312ULL, false), BitBoard(1407374883553280ULL, false), BitBoard(2814749767106560ULL, false), BitBoard(5629499534213120ULL, false), BitBoard(11258999068426240ULL, false), BitBoard(22517998136852480ULL, false), BitBoard(45035996273704960ULL, false), BitBoard(18014398509481984ULL, false), BitBoard(144115188075855872ULL, false), BitBoard(360287970189639680ULL, false), BitBoard(720575940379279360ULL, false), BitBoard(1441151880758558720ULL, false), BitBoard(2882303761517117440ULL, false), BitBoard(5764607523034234880ULL, false), BitBoard(11529215046068469760ULL, false), BitBoard(4611686018427387904ULL, false), BitBoard(0ULL, false), BitBoard(0ULL, false), BitBoard(0ULL, false), BitBoard(0ULL, false), BitBoard(0ULL, false), BitBoard(0ULL, false), BitBoard(0ULL, false), BitBoard(0ULL, false)}};

//*KNIGHT ATTACK TABLE______________________________________
// knightAttacks[squareIndex] , size = 64
BitBoard BitBoard::KNIGHT_ATTACK_TABLE[64] =
    {BitBoard(132096ULL, false), BitBoard(329728ULL, false), BitBoard(659712ULL, false), BitBoard(1319424ULL, false), BitBoard(2638848ULL, false), BitBoard(5277696ULL, false), BitBoard(10489856ULL, false), BitBoard(4202496ULL, false), BitBoard(33816580ULL, false), BitBoard(84410376ULL, false), BitBoard(168886289ULL, false), BitBoard(337772578ULL, false), BitBoard(675545156ULL, false), BitBoard(1351090312ULL, false), BitBoard(2685403152ULL, false), BitBoard(1075839008ULL, false), BitBoard(8657044482ULL, false), BitBoard(21609056261ULL, false), BitBoard(43234889994ULL, false), BitBoard(86469779988ULL, false), BitBoard(172939559976ULL, false), BitBoard(345879119952ULL, false), BitBoard(687463207072ULL, false), BitBoard(275414786112ULL, false), BitBoard(2216203387392ULL, false), BitBoard(5531918402816ULL, false), BitBoard(11068131838464ULL, false), BitBoard(22136263676928ULL, false), BitBoard(44272527353856ULL, false), BitBoard(88545054707712ULL, false), BitBoard(175990581010432ULL, false), BitBoard(70506185244672ULL, false), BitBoard(567348067172352ULL, false), BitBoard(1416171111120896ULL, false), BitBoard(2833441750646784ULL, false), BitBoard(5666883501293568ULL, false), BitBoard(11333767002587136ULL, false), BitBoard(22667534005174272ULL, false), BitBoard(45053588738670592ULL, false), BitBoard(18049583422636032ULL, false), BitBoard(145241105196122112ULL, false), BitBoard(362539804446949376ULL, false), BitBoard(725361088165576704ULL, false), BitBoard(1450722176331153408ULL, false), BitBoard(2901444352662306816ULL, false), BitBoard(5802888705324613632ULL, false), BitBoard(11533718717099671552ULL, false), BitBoard(4620693356194824192ULL, false), BitBoard(288234782788157440ULL, false), BitBoard(576469569871282176ULL, false), BitBoard(1224997833292120064ULL, false), BitBoard(2449995666584240128ULL, false), BitBoard(4899991333168480256ULL, false), BitBoard(9799982666336960512ULL, false), BitBoard(1152939783987658752ULL, false), BitBoard(2305878468463689728ULL, false), BitBoard(1128098930098176ULL, false), BitBoard(2257297371824128ULL, false), BitBoard(4796069720358912ULL, false), BitBoard(9592139440717824ULL, false), BitBoard(19184278881435648ULL, false), BitBoard(38368557762871296ULL, false), BitBoard(4679521487814656ULL, false), BitBoard(9077567998918656ULL, false)};

// kingAttacks[squareIndex] , size = 64
BitBoard BitBoard::KING_ATTACK_TABLE[64] = {BitBoard(770ULL, false), BitBoard(1797ULL, false), BitBoard(3594ULL, false), BitBoard(7188ULL, false), BitBoard(14376ULL, false), BitBoard(28752ULL, false), BitBoard(57504ULL, false), BitBoard(49216ULL, false), BitBoard(197123ULL, false), BitBoard(460039ULL, false), BitBoard(920078ULL, false), BitBoard(1840156ULL, false), BitBoard(3680312ULL, false), BitBoard(7360624ULL, false), BitBoard(14721248ULL, false), BitBoard(12599488ULL, false), BitBoard(50463488ULL, false), BitBoard(117769984ULL, false), BitBoard(235539968ULL, false), BitBoard(471079936ULL, false), BitBoard(942159872ULL, false), BitBoard(1884319744ULL, false), BitBoard(3768639488ULL, false), BitBoard(3225468928ULL, false), BitBoard(12918652928ULL, false), BitBoard(30149115904ULL, false), BitBoard(60298231808ULL, false), BitBoard(120596463616ULL, false), BitBoard(241192927232ULL, false), BitBoard(482385854464ULL, false), BitBoard(964771708928ULL, false), BitBoard(825720045568ULL, false), BitBoard(3307175149568ULL, false), BitBoard(7718173671424ULL, false), BitBoard(15436347342848ULL, false), BitBoard(30872694685696ULL, false), BitBoard(61745389371392ULL, false), BitBoard(123490778742784ULL, false), BitBoard(246981557485568ULL, false), BitBoard(211384331665408ULL, false), BitBoard(846636838289408ULL, false), BitBoard(1975852459884544ULL, false), BitBoard(3951704919769088ULL, false), BitBoard(7903409839538176ULL, false), BitBoard(15806819679076352ULL, false), BitBoard(31613639358152704ULL, false), BitBoard(63227278716305408ULL, false), BitBoard(54114388906344448ULL, false), BitBoard(216739030602088448ULL, false), BitBoard(505818229730443264ULL, false), BitBoard(1011636459460886528ULL, false), BitBoard(2023272918921773056ULL, false), BitBoard(4046545837843546112ULL, false), BitBoard(8093091675687092224ULL, false), BitBoard(16186183351374184448ULL, false), BitBoard(13853283560024178688ULL, false), BitBoard(144959613005987840ULL, false), BitBoard(362258295026614272ULL, false), BitBoard(724516590053228544ULL, false), BitBoard(1449033180106457088ULL, false), BitBoard(2898066360212914176ULL, false), BitBoard(5796132720425828352ULL, false), BitBoard(11592265440851656704ULL, false), BitBoard(4665729213955833856ULL, false)};

//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
// //* BISHOP_ATTACK_TABLE[squareIndex][occupancyBitBoard], size = 64 * 512 (---------------------------need to be initialized---------------------------)
// BitBoard BISHOP_ATTACK_TABLE[64][512];

// //* ROOK_ATTACK_TABLE[squareIndex][occupancyBitBoard] , size = 64 * 4096 (---------------------------need to be initialized---------------------------)
// BitBoard ROOK_ATTACK_TABLE[64][4096];

//*initialize the vectors with given size
vector<vector<BitBoard>> emptyBishopVector(64, vector<BitBoard>(512, BitBoard(0ULL, false)));
vector<vector<BitBoard>> emptyRookVector(64, vector<BitBoard>(4096, BitBoard(0ULL, false)));

vector<vector<BitBoard>> BitBoard::BISHOP_ATTACK_TABLE = emptyBishopVector;
vector<vector<BitBoard>> BitBoard::ROOK_ATTACK_TABLE = emptyRookVector;