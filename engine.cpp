// Name: SOURASHIS MONDAL

#include <bits/stdc++.h>

using namespace std;
class BitBoard;

#include "definitions.h"

/*
    * BitBoard index :
    *
    *
    *
    *
    *
    *
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
    *
    *
    *

*/

//*helper functions
#include "helperFunctions.h"

class BitBoard
{
    U64 value;

public:
    bool isSquareIndexValid(int squareIndex) const
    {
        return (0 <= squareIndex && squareIndex < 64);
    }

    void checkSquareIndexValidity(int squareIndex) const
    {
        if (!isSquareIndexValid(squareIndex))
            throw runtime_error("Square index out of bound inside getBitAt()");
    }

    U64 inline getDecimalValue() const
    {
        return this->value;
    }

    BitBoard(U64 value = 0ULL)
    {
        this->value = value;
    }

    int inline getBitAt(int index) const
    {
        checkSquareIndexValidity(index);

        return ((this->value & (1ULL << index)) == 0) ? 0 : 1;
    }

    void inline setBitAt(int index)
    {
        checkSquareIndexValidity(index);

        this->value = (this->value | (1ULL << index));
    }

    void inline toggleBitAt(int index)
    {
        checkSquareIndexValidity(index);

        this->value = (this->value ^ (1ULL << index));
    }

    void inline unsetBitAt(int index)
    {
        checkSquareIndexValidity(index);

        if (getBitAt(index) == 1)
            toggleBitAt(index);
    }

    inline int countSetBits() const
    {
        return countSetBitsOfU64(this->value);
    }

    inline int getFirstLeastSignificantBitIndexFromRight() const
    {
        return getFirstLeastSignificantBitIndexFromRightU64(this->value);
    }

    void inline print() const
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
                cout << getBitAt(index) << "  ";
            }
            cout << endl;
        }

        // print files
        cout << "\n     a  b  c  d  e  f  g  h";

        // print decimal value of the bitboard
        cout << "\n\n    Decimal value : " << this->value << endl
             << endl;
    }

    void printWithBoxes(bool withZeros = false) const
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

                int bit = getBitAt(index);

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

    //*-----------------------------------------------for testing----------------------------------------------------
    friend U64 findMagicNumber(int squareIndex, bool bishop);

    //*-----------------------------------------------for testing----------------------------------------------------
};

//*Constants

//*_____________________________ATTACK TABLES____________________________________________________________________
#include "tables.h"

BitBoard maskPawnAttacks(int playerColor, int squareIndex)
{
    BitBoard result;
    result.checkSquareIndexValidity(squareIndex);

    //*for white pawns
    if (playerColor == WHITE)
    {
        //*front left square index = squareIndex - 9
        if (squareIndex % 8 != 0 && result.isSquareIndexValid(squareIndex - 9)) // if squareIndex is not on a file
            result.setBitAt(squareIndex - 9);

        //*front right square index = squareIndex - 7
        if (squareIndex % 8 != 7 && result.isSquareIndexValid(squareIndex - 7)) // if squareIndex is not on h file
            result.setBitAt(squareIndex - 7);
    }
    //*for black pawns
    else if (playerColor == BLACK)
    {
        //*front left square index = squareIndex + 9
        if (squareIndex % 8 != 7 && result.isSquareIndexValid(squareIndex + 9)) // if squareIndex is not on h file
            result.setBitAt(squareIndex + 9);

        //*front right square index = squareIndex + 7
        if (squareIndex % 8 != 0 && result.isSquareIndexValid(squareIndex + 7)) // if squareIndex is not on a file
            result.setBitAt(squareIndex + 7);
    }

    return result;
}

BitBoard maskKnightAttacks(int squareIndex)
{
    BitBoard result;
    result.checkSquareIndexValidity(squareIndex);

    int targetIndex = -1;

    //*2 squares up, 1 square right , targetIndex = squareIndex - 15
    targetIndex = squareIndex - 15;
    if (getFileOfSquareIndex(squareIndex) != 'h' && result.isSquareIndexValid(targetIndex)) // if squareIndex is not on h file
        result.setBitAt(targetIndex);

    //*2 squares up, 1 square left , targetIndex = squareIndex - 17
    targetIndex = squareIndex - 17;
    if (getFileOfSquareIndex(squareIndex) != 'a' && result.isSquareIndexValid(targetIndex)) // if squareIndex is not on a file
        result.setBitAt(targetIndex);

    //*2 squares down, 1 square right , targetIndex = squareIndex + 17
    targetIndex = squareIndex + 17;
    if (getFileOfSquareIndex(squareIndex) != 'h' && result.isSquareIndexValid(targetIndex)) // if squareIndex is not on h file
        result.setBitAt(targetIndex);

    //*2 squares down, 1 square left , targetIndex = squareIndex + 15
    targetIndex = squareIndex + 15;
    if (getFileOfSquareIndex(squareIndex) != 'a' && result.isSquareIndexValid(targetIndex)) // if squareIndex is not on a file
        result.setBitAt(targetIndex);

    //*2 squares right, 1 square up , targetIndex = squareIndex - 6
    targetIndex = squareIndex - 6;
    if (getFileOfSquareIndex(squareIndex) != 'h' && getFileOfSquareIndex(squareIndex) != 'g' && result.isSquareIndexValid(targetIndex)) // if squareIndex is not on h or g file
        result.setBitAt(targetIndex);

    //*2 squares right, 1 square down , targetIndex = squareIndex + 10
    targetIndex = squareIndex + 10;
    if (getFileOfSquareIndex(squareIndex) != 'h' && getFileOfSquareIndex(squareIndex) != 'g' && result.isSquareIndexValid(targetIndex)) // if squareIndex is not on h or g file
        result.setBitAt(targetIndex);

    //*2 squares left, 1 square up , targetIndex = squareIndex - 10
    targetIndex = squareIndex - 10;
    if (getFileOfSquareIndex(squareIndex) != 'a' && getFileOfSquareIndex(squareIndex) != 'b' && result.isSquareIndexValid(targetIndex)) // if squareIndex is not on h or g file
        result.setBitAt(targetIndex);

    //*2 squares left, 1 square down , targetIndex = squareIndex + 6
    targetIndex = squareIndex + 6;
    if (getFileOfSquareIndex(squareIndex) != 'a' && getFileOfSquareIndex(squareIndex) != 'b' && result.isSquareIndexValid(targetIndex)) // if squareIndex is not on h or g file
        result.setBitAt(targetIndex);

    return result;
}

BitBoard maskKingAttacks(int squareIndex)
{
    BitBoard result;
    result.checkSquareIndexValidity(squareIndex);

    int targetIndex = -1;

    //*1 square up
    targetIndex = squareIndex - 8;
    if (result.isSquareIndexValid(targetIndex))
        result.setBitAt(targetIndex);

    //*1 square down
    targetIndex = squareIndex + 8;
    if (result.isSquareIndexValid(targetIndex))
        result.setBitAt(targetIndex);

    //*1 square right
    targetIndex = squareIndex + 1;
    if (getFileOfSquareIndex(squareIndex) != 'h' && result.isSquareIndexValid(targetIndex)) // if squareIndex is not on h file
        result.setBitAt(targetIndex);

    //*1 square left
    targetIndex = squareIndex - 1;
    if (getFileOfSquareIndex(squareIndex) != 'a' && result.isSquareIndexValid(targetIndex)) // if squareIndex is not on a file
        result.setBitAt(targetIndex);

    //*1 square up, 1 square right
    targetIndex = squareIndex - 7;
    if (getFileOfSquareIndex(squareIndex) != 'h' && result.isSquareIndexValid(targetIndex)) // if squareIndex is not on h or g file
        result.setBitAt(targetIndex);

    //*1 square down, 1 square right
    targetIndex = squareIndex + 9;
    if (getFileOfSquareIndex(squareIndex) != 'h' && result.isSquareIndexValid(targetIndex)) // if squareIndex is not on h or g file
        result.setBitAt(targetIndex);

    //*1 square up, 1 square left :
    targetIndex = squareIndex - 9;
    if (getFileOfSquareIndex(squareIndex) != 'a' && result.isSquareIndexValid(targetIndex)) // if squareIndex is not on h or g file
        result.setBitAt(targetIndex);

    //*1 square down, 1 square left
    targetIndex = squareIndex + 7;
    if (getFileOfSquareIndex(squareIndex) != 'a' && result.isSquareIndexValid(targetIndex)) // if squareIndex is not on h or g file
        result.setBitAt(targetIndex);

    return result;
}

//*Exludes the edge of the board
BitBoard maskBishopAttacks(int squareIndex)
{
    BitBoard result;
    result.checkSquareIndexValidity(squareIndex);

    int targetIndex = -1;

    int currentRank = getRankOfSquareIndex(squareIndex);
    char currentFile = getFileOfSquareIndex(squareIndex);

    int rank = 0;
    char file = 0;

    //*upper right diagonal
    for (rank = currentRank + 1, file = currentFile + 1; rank <= 7 && file <= 'g'; rank++, file++)
    {
        targetIndex = getIndexFromRankAndFile(rank, file);
        result.setBitAt(targetIndex);
    }

    //*lower right diagonal
    for (rank = currentRank - 1, file = currentFile + 1; rank >= 2 && file <= 'g'; rank--, file++)
    {
        targetIndex = getIndexFromRankAndFile(rank, file);
        result.setBitAt(targetIndex);
    }

    //*upper left diagonal
    for (rank = currentRank + 1, file = currentFile - 1; rank <= 7 && file >= 'b'; rank++, file--)
    {
        targetIndex = getIndexFromRankAndFile(rank, file);
        result.setBitAt(targetIndex);
    }

    //*lower left diagonal
    for (rank = currentRank - 1, file = currentFile - 1; rank >= 2 && file >= 'b'; rank--, file--)
    {
        targetIndex = getIndexFromRankAndFile(rank, file);
        result.setBitAt(targetIndex);
    }

    return result;
}

//*Exludes the edge of the board
BitBoard maskRookAttacks(int squareIndex)
{
    BitBoard result;
    result.checkSquareIndexValidity(squareIndex);

    int targetIndex = -1;

    int currentRank = getRankOfSquareIndex(squareIndex);
    char currentFile = getFileOfSquareIndex(squareIndex);

    // cout << "\ncurrentRank = " << currentRank;

    int rank = currentRank;
    char file = currentFile;

    //*upper file
    for (rank = currentRank + 1; rank <= 7; rank++)
    {
        targetIndex = getIndexFromRankAndFile(rank, currentFile);
        result.setBitAt(targetIndex);
    }

    //*lower file
    for (rank = currentRank - 1; rank >= 2; rank--)
    {
        targetIndex = getIndexFromRankAndFile(rank, currentFile);
        result.setBitAt(targetIndex);
    }

    //*left rank
    for (file = currentFile - 1; file >= 'b'; file--)
    {
        targetIndex = getIndexFromRankAndFile(currentRank, file);
        result.setBitAt(targetIndex);
    }

    //*right rank
    for (file = currentFile + 1; file <= 'g'; file++)
    {
        targetIndex = getIndexFromRankAndFile(currentRank, file);
        result.setBitAt(targetIndex);
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
BitBoard generateBishopAttacksOnTheFly(int squareIndex, const BitBoard &blockers)
{
    BitBoard result;
    result.checkSquareIndexValidity(squareIndex);

    int targetIndex = -1;

    int currentRank = getRankOfSquareIndex(squareIndex);
    char currentFile = getFileOfSquareIndex(squareIndex);

    int rank = 0;
    char file = 0;

    //*upper right diagonal
    for (rank = currentRank + 1, file = currentFile + 1; rank <= 8 && file <= 'h'; rank++, file++)
    {
        targetIndex = getIndexFromRankAndFile(rank, file);
        result.setBitAt(targetIndex);

        if (blockers.getBitAt(targetIndex) == 1)
            break;
    }

    //*lower right diagonal
    for (rank = currentRank - 1, file = currentFile + 1; rank >= 1 && file <= 'h'; rank--, file++)
    {
        targetIndex = getIndexFromRankAndFile(rank, file);
        result.setBitAt(targetIndex);

        if (blockers.getBitAt(targetIndex) == 1)
            break;
    }

    //*upper left diagonal
    for (rank = currentRank + 1, file = currentFile - 1; rank <= 8 && file >= 'a'; rank++, file--)
    {
        targetIndex = getIndexFromRankAndFile(rank, file);
        result.setBitAt(targetIndex);

        if (blockers.getBitAt(targetIndex) == 1)
            break;
    }

    //*lower left diagonal
    for (rank = currentRank - 1, file = currentFile - 1; rank >= 1 && file >= 'a'; rank--, file--)
    {
        targetIndex = getIndexFromRankAndFile(rank, file);
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
BitBoard generateRookAttacksOnTheFly(int squareIndex, const BitBoard &blockers)
{
    BitBoard result;
    result.checkSquareIndexValidity(squareIndex);

    int targetIndex = -1;

    int currentRank = getRankOfSquareIndex(squareIndex);
    char currentFile = getFileOfSquareIndex(squareIndex);

    // cout << "\ncurrentRank = " << currentRank;

    int rank = currentRank;
    char file = currentFile;

    //*upper file
    for (rank = currentRank + 1; rank <= 8; rank++)
    {
        targetIndex = getIndexFromRankAndFile(rank, currentFile);
        result.setBitAt(targetIndex);

        if (blockers.getBitAt(targetIndex) == 1)
            break;
    }

    //*lower file
    for (rank = currentRank - 1; rank >= 1; rank--)
    {
        targetIndex = getIndexFromRankAndFile(rank, currentFile);
        result.setBitAt(targetIndex);

        if (blockers.getBitAt(targetIndex) == 1)
            break;
    }

    //*left rank
    for (file = currentFile - 1; file >= 'a'; file--)
    {
        targetIndex = getIndexFromRankAndFile(currentRank, file);
        result.setBitAt(targetIndex);

        if (blockers.getBitAt(targetIndex) == 1)
            break;
    }

    //*right rank
    for (file = currentFile + 1; file <= 'h'; file++)
    {
        targetIndex = getIndexFromRankAndFile(currentRank, file);
        result.setBitAt(targetIndex);

        if (blockers.getBitAt(targetIndex) == 1)
            break;
    }

    return result;
}

/*
TODO: Dunno much about this function for now :) , research later
*/
BitBoard setOccupancy(int index, BitBoard attackMask)
{
    // occupancy mask
    BitBoard occupancy;

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

void printPawnAttackTables()
{
    cout << "\nwhite pawn attacks : \n";

    for (int squareIndex = 0; squareIndex < 64; squareIndex++)
    {
        cout << "\nfor square : " << getAlgebraicCoordinateFromIndex(squareIndex) << " : \n";
        PAWN_ATTACK_TABLE[WHITE][squareIndex].print();
    }

    cout << "\nblack pawn attacks : \n";

    for (int squareIndex = 0; squareIndex < 64; squareIndex++)
    {
        cout << "\nfor square : " << getAlgebraicCoordinateFromIndex(squareIndex) << " : \n";
        PAWN_ATTACK_TABLE[BLACK][squareIndex].print();
    }

    // cout << "{ ";

    // cout << "{ ";
    // for (int squareIndex = 0; squareIndex < 64; squareIndex++)
    // {

    //     cout << maskPawnAttacks(WHITE, squareIndex).getDecimalValue() << "ULL";
    //     if (squareIndex != 63)
    //         cout << ", ";
    // }
    // cout << " }, \n";

    // cout << "{ ";
    // for (int squareIndex = 0; squareIndex < 64; squareIndex++)
    // {

    //     cout << maskPawnAttacks(BLACK, squareIndex).getDecimalValue() << "ULL";
    //     if (squareIndex != 63)
    //         cout << ", ";
    // }
    // cout << " } \n";

    // cout << "} ";
}

void printKngihtAttackTable()
{

    for (int squareIndex = 0; squareIndex < 64; squareIndex++)
    {
        cout << "\nfor square : " << getAlgebraicCoordinateFromIndex(squareIndex) << " : \n";
        KNIGHT_ATTACK_TABLE[squareIndex].print();
    }

    // cout << "{ ";
    // for (int squareIndex = 0; squareIndex < 64; squareIndex++)
    // {

    //     cout << maskKnightAttacks(squareIndex).getDecimalValue() << "ULL";
    //     if (squareIndex != 63)
    //         cout << ", ";
    // }
    // cout << " } \n";
}

// void initializePawnAttackTables()
// {
//     for (int squareIndex = 0; squareIndex < 64; squareIndex++)
//     {
//         pawnAttacks[WHITE][squareIndex] = maskPawnAttacks(WHITE, squareIndex);
//         pawnAttacks[BLACK][squareIndex] = maskPawnAttacks(BLACK, squareIndex);
//     }
// }

vector<BitBoard> getKnightAttackTable()
{
    vector<BitBoard> knightAttackTable(64, 0ULL);
    for (int squareIndex = 0; squareIndex < 64; squareIndex++)
    {
        knightAttackTable[squareIndex] = maskKnightAttacks(squareIndex);
    }

    return knightAttackTable;
}

vector<BitBoard> getKingAttackTable()
{
    vector<BitBoard> kingAttackTable(64, 0ULL);
    for (int squareIndex = 0; squareIndex < 64; squareIndex++)
    {
        kingAttackTable[squareIndex] = maskKingAttacks(squareIndex);
    }

    return kingAttackTable;
}

//*Exludes the edge of the board
vector<BitBoard> getBishopAttackTable()
{
    vector<BitBoard> bishopAttackTable(64, 0ULL);
    for (int squareIndex = 0; squareIndex < 64; squareIndex++)
    {
        bishopAttackTable[squareIndex] = maskBishopAttacks(squareIndex);
    }

    return bishopAttackTable;
}

//*Exludes the edge of the board
vector<BitBoard> getRookAttackTable()
{
    vector<BitBoard> rookAttackTable(64, 0ULL);
    for (int squareIndex = 0; squareIndex < 64; squareIndex++)
    {
        rookAttackTable[squareIndex] = maskRookAttacks(squareIndex);
    }

    return rookAttackTable;
}

void printKingAttacktable(const vector<BitBoard> &kingAttackTable)
{
    for (int squareIndex = 0; squareIndex < 64; squareIndex++)
    {
        cout << "\nfor square : " << getAlgebraicCoordinateFromIndex(squareIndex) << " : \n";
        kingAttackTable[squareIndex].print();
    }

    cout << "{ ";
    for (int squareIndex = 0; squareIndex < 64; squareIndex++)
    {

        cout << kingAttackTable[squareIndex].getDecimalValue() << "ULL";
        if (squareIndex != 63)
            cout << ", ";
    }
    cout << " } \n";
}

//*TODO: Dunno theory behind it
//*find appropriate magic number
/*
    bishop = true ,means magicNumber for bishop on squareIndex
            false, means magicNumber for rook on squareIndex
*/
U64 findMagicNumber(int squareIndex, bool bishop)
{

    // initialize occupancies
    BitBoard occupancies[4096]; // Dunno why this size

    // initialize attack tables
    BitBoard attacks[4096];

    // initialize used attacks
    BitBoard usedAttacks[4096];

    // initialize attack mask for current piece
    BitBoard currentPieceAttackMask = bishop ? BISHOP_ATTACK_TABLE[squareIndex] : ROOK_ATTACK_TABLE[squareIndex];

    // count set bits for attack mask of current piece at given squareIndex
    int countRelevantBits = currentPieceAttackMask.countSetBits();

    // initialize occupancy index
    int occupancyIndex = 1 << countRelevantBits;

    // loop over occupancy indices
    for (int index = 0; index < occupancyIndex; index++)
    {
        // initialize occupancies
        occupancies[index] = setOccupancy(index, currentPieceAttackMask);

        // initialize attacks
        attacks[index] = bishop ? generateBishopAttacksOnTheFly(squareIndex, occupancies[index]) : generateRookAttacksOnTheFly(squareIndex, occupancies[index]);
    }

    // test magic numbers loop
    for (int randomCount = 0; randomCount < 100000000; randomCount++)
    {
        // generate magic number candidate
        U64 magicNum = generatePotentialMagicNumber();

        // skip inappropriate magic numbers
        // 0xFF00000000000000 = 1111111100000000000000000000000000000000000000000000000000000000, i.e it has all higher 8 bits 1
        if (countSetBitsOfU64((currentPieceAttackMask.value * magicNum) & 0xFF00000000000000) < 6)
            continue;

        // initialize usedAttacks array
        // for (int i = 0; i < 4096; i++)
        // usedAttacks[i] = 0ULL;
        memset(usedAttacks, 0ULL, sizeof(usedAttacks));

        // initialize index and fail flag
        int index = 0;
        bool fail = false;

        // test magic index
        while (!fail && index < occupancyIndex)
        {
            // initialize magic index
            int magicIndex = (int)((occupancies[index].value * magicNum) >> (64 - countRelevantBits));

            // if magic index works
            if (usedAttacks[magicIndex].value == 0ULL)
                // initialize usedAttacjs
                usedAttacks[magicIndex] = attacks[index];
            else if (usedAttacks[magicIndex].value != attacks[index].value)
                fail = true;

            index++;
        }

        // if magic number works
        if (!fail)
        {
            return magicNum;
        }
    }

    // if magic number doesn't work
    cout << "\n\n\n\n\n\n\n\n\n\nmagic number doesnt work !!!!!!!!!!!!\n\n\n\n\n\n\n\n\n\n";
    return -1;
}

void initializeMagicNumbers()
{

    cout << "\n\nRook Magic numbers : ";
    for (int squareIndex = 0; squareIndex < 64; squareIndex++)
    {
        // print rook magic numbers
        // cout << findMagicNumber(squareIndex, false) << "ULL, ";
        printf(" 0x%llxULL,\n", findMagicNumber(squareIndex, false));
    }

    cout << "\n\nBishop Magic numbers : ";
    for (int squareIndex = 0; squareIndex < 64; squareIndex++)
    {
        // cout << findMagicNumber(squareIndex, true) << "ULL, ";
        printf(" 0x%llxULL,\n", findMagicNumber(squareIndex, true));
    }
}

// void initializeLeaperPieceAttackTables()
// {
//     initializePawnAttackTables();
// }

int main()
{
    cout << "\n\nHello earth\n\n\n";

    // initializeMagicNumbers();

    cout << testMagicNumberMatch(BISHOP_MAGIC_NUMBER, ROOK_MAGIC_NUMBER);
}
