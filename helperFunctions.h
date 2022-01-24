

#pragma once

#include <bits/stdc++.h>
#include "definitions.h"
#include "BitBoard.h"

class BitBoard;

//*----------------------------------------helper functions--------------------------------------------

char inline getFileOfSquareIndex(int index)
{
    return 'a' + index % 8;
}

int inline getRankOfSquareIndex(int index)
{
    return 8 - (index / 8);
}

//*rank in [1,8] and file in ['a', 'h']
int inline getIndexFromRankAndFile(int rank, char file)
{
    return (8 - rank) * 8 + (file - 'a');
}

// row and column numbers are 0 based
int inline getIndexFromRowAndColumnNumbers(int row, int column)
{
    return row * 8 + column;
}

std::string inline getAlgebraicCoordinateFromIndex(int index)
{
    return getFileOfSquareIndex(index) + std::to_string(getRankOfSquareIndex(index));
}

int getOppositeColor(int color)
{
    if (color == WHITE)
        return BLACK;
    if (color == BLACK)
        return WHITE;

    return -1;
}

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
vector<BitBoard> getBishopAttackMask()
{
    vector<BitBoard> bishopAttackMask(64, 0ULL);
    for (int squareIndex = 0; squareIndex < 64; squareIndex++)
    {
        bishopAttackMask[squareIndex] = maskBishopAttacks(squareIndex);
    }

    return bishopAttackMask;
}

//*Exludes the edge of the board
vector<BitBoard> getRookAttackMask()
{
    vector<BitBoard> rookAttackMask(64, 0ULL);
    for (int squareIndex = 0; squareIndex < 64; squareIndex++)
    {
        rookAttackMask[squareIndex] = maskRookAttacks(squareIndex);
    }

    return rookAttackMask;
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

//*Karnigan's algorithm
inline int countSetBitsOfU64(U64 x)
{
    int count = 0;
    while (x)
    {
        count++;
        x &= x - 1;
    }
    return count;
}

//*0 based index
inline int getFirstLeastSignificantBitIndexFromRightU64(U64 x)
{
    // if x is 0, then return illegal index
    if (x == 0)
        return -1;

    /*
        let x = 11001000

        Now (x & 2s complement of x ) = 00001000 ----> creates a mask with only the 1st LSB as 1

        if we subtract 1 from it , then all bits after and including the set LSB will be flipped
        (x & 2s complement of x ) - 1  = 00000111

        if we count the number of bits , it is 3 , which is also equal to the index of 1st LSB of x
    */

    return countSetBitsOfU64((x & -x) - 1);
}

//*pseudo random number state
unsigned int pseudoRandomNumberInitialState = 1804289383; // a random number generated by rand()

//* generate 32-bit pseudo random numbers
unsigned int getPseudoRandomNumberU32()
{
    // get current state
    unsigned int num = pseudoRandomNumberInitialState;

    // XOR Shift algorithm
    num ^= num << 13;
    num ^= num >> 17;
    num ^= num << 5;

    // update random number state
    pseudoRandomNumberInitialState = num;

    return num;
}

//*generate 64-bit pseudo random number
U64 getPseudoRandomNumberU64()
{
    // define 4 random numbers
    U64 n1, n2, n3, n4;

    // initialize random numbers, slicing 16  bits from Most Significant 1st bit (using  &0xFFFF)
    n1 = (U64)(getPseudoRandomNumberU32()) & 0xFFFF;
    n2 = (U64)(getPseudoRandomNumberU32()) & 0xFFFF;
    n3 = (U64)(getPseudoRandomNumberU32()) & 0xFFFF;
    n4 = (U64)(getPseudoRandomNumberU32()) & 0xFFFF;

    // return random number of 64 bit by shuffling these four numbers
    return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}

//*generate magic number candidates : returns random number of 64-bit
U64 generatePotentialMagicNumber()
{
    return getPseudoRandomNumberU64() & getPseudoRandomNumberU64() & getPseudoRandomNumberU64();
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
    BitBoard currentPieceAttackMask = bishop ? BISHOP_ATTACK_MASK[squareIndex] : ROOK_ATTACK_MASK[squareIndex];

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
        if (countSetBitsOfU64((currentPieceAttackMask.getDecimalValue() * magicNum) & 0xFF00000000000000) < 6)
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
            int magicIndex = (int)((occupancies[index].getDecimalValue() * magicNum) >> (64 - countRelevantBits));

            // if magic index works
            if (usedAttacks[magicIndex].getDecimalValue() == 0ULL)
                // initialize usedAttacjs
                usedAttacks[magicIndex] = attacks[index];
            else if (usedAttacks[magicIndex].getDecimalValue() != attacks[index].getDecimalValue())
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

// //*test magic number equality with "Chess Programming" channel's video's magic numbers
// bool testMagicNumberMatch(const U64 BISHOP_MAGIC_NUMBER[64], const U64 ROOK_MAGIC_NUMBER[64])
// {
//     U64 rook_magic_numbers_of_chess_programming[64] = {
//         0x8a80104000800020ULL, 0x140002000100040ULL, 0x2801880a0017001ULL, 0x100081001000420ULL, 0x200020010080420ULL, 0x3001c0002010008ULL, 0x8480008002000100ULL, 0x2080088004402900ULL, 0x800098204000ULL, 0x2024401000200040ULL, 0x100802000801000ULL, 0x120800800801000ULL, 0x208808088000400ULL, 0x2802200800400ULL, 0x2200800100020080ULL, 0x801000060821100ULL, 0x80044006422000ULL, 0x100808020004000ULL, 0x12108a0010204200ULL, 0x140848010000802ULL, 0x481828014002800ULL, 0x8094004002004100ULL, 0x4010040010010802ULL, 0x20008806104ULL, 0x100400080208000ULL, 0x2040002120081000ULL, 0x21200680100081ULL, 0x20100080080080ULL, 0x2000a00200410ULL, 0x20080800400ULL, 0x80088400100102ULL, 0x80004600042881ULL, 0x4040008040800020ULL, 0x440003000200801ULL, 0x4200011004500ULL, 0x188020010100100ULL, 0x14800401802800ULL, 0x2080040080800200ULL, 0x124080204001001ULL, 0x200046502000484ULL, 0x480400080088020ULL, 0x1000422010034000ULL, 0x30200100110040ULL, 0x100021010009ULL, 0x2002080100110004ULL, 0x202008004008002ULL, 0x20020004010100ULL, 0x2048440040820001ULL, 0x101002200408200ULL, 0x40802000401080ULL, 0x4008142004410100ULL, 0x2060820c0120200ULL, 0x1001004080100ULL, 0x20c020080040080ULL, 0x2935610830022400ULL, 0x44440041009200ULL, 0x280001040802101ULL, 0x2100190040002085ULL, 0x80c0084100102001ULL, 0x4024081001000421ULL, 0x20030a0244872ULL, 0x12001008414402ULL, 0x2006104900a0804ULL, 0x1004081002402ULL};

//     // bishop magic numbers
//     U64 bishop_magic_numbers_of_chess_programming[64] = {
//         0x40040844404084ULL, 0x2004208a004208ULL, 0x10190041080202ULL, 0x108060845042010ULL, 0x581104180800210ULL, 0x2112080446200010ULL, 0x1080820820060210ULL, 0x3c0808410220200ULL, 0x4050404440404ULL, 0x21001420088ULL, 0x24d0080801082102ULL, 0x1020a0a020400ULL, 0x40308200402ULL, 0x4011002100800ULL, 0x401484104104005ULL, 0x801010402020200ULL, 0x400210c3880100ULL, 0x404022024108200ULL, 0x810018200204102ULL, 0x4002801a02003ULL, 0x85040820080400ULL, 0x810102c808880400ULL, 0xe900410884800ULL, 0x8002020480840102ULL, 0x220200865090201ULL, 0x2010100a02021202ULL, 0x152048408022401ULL, 0x20080002081110ULL, 0x4001001021004000ULL, 0x800040400a011002ULL, 0xe4004081011002ULL, 0x1c004001012080ULL, 0x8004200962a00220ULL, 0x8422100208500202ULL, 0x2000402200300c08ULL, 0x8646020080080080ULL, 0x80020a0200100808ULL, 0x2010004880111000ULL, 0x623000a080011400ULL, 0x42008c0340209202ULL, 0x209188240001000ULL, 0x400408a884001800ULL, 0x110400a6080400ULL, 0x1840060a44020800ULL, 0x90080104000041ULL, 0x201011000808101ULL, 0x1a2208080504f080ULL, 0x8012020600211212ULL, 0x500861011240000ULL, 0x180806108200800ULL, 0x4000020e01040044ULL, 0x300000261044000aULL, 0x802241102020002ULL, 0x20906061210001ULL, 0x5a84841004010310ULL, 0x4010801011c04ULL, 0xa010109502200ULL, 0x4a02012000ULL, 0x500201010098b028ULL, 0x8040002811040900ULL, 0x28000010020204ULL, 0x6000020202d0240ULL, 0x8918844842082200ULL, 0x4010011029020020ULL};

//     //*test bishop magic numbers
//     for (int i = 0; i < 64; i++)
//     {
//         if (BISHOP_MAGIC_NUMBER[i] != bishop_magic_numbers_of_chess_programming[i])
//             return false;
//     }

//     //*test rook magic numbers
//     for (int i = 0; i < 64; i++)
//     {
//         if (ROOK_MAGIC_NUMBER[i] != rook_magic_numbers_of_chess_programming[i])
//             return false;
//     }

//     return true;
// }

// void printSliderPieceAttackTablesIntoFile()
// {
//     ofstream fptr("sliderPieceAttackTable.txt");
//     if (!fptr)
//     {
//         cout << "\nproblem in opening file \n\n\n\n\n";
//         exit(1);
//     }
//     // fprintf(fptr, "hello");

//     fptr << "\n/";
//     fptr << "/";
//     fptr << "* ROOK_ATTACK_TABLE[squareIndex][occupancyBitBoard]\nBitBoard ROOK_ATTACK_TABLE[64][4096] = \n";

//     fptr << "{\n";
//     for (int squareIndex = 0; squareIndex < 64; squareIndex++)
//     {
//         fptr << "\n\t{";
//         for (int magicIndex = 0; magicIndex < 4096; magicIndex++)
//         {
//             fptr << ROOK_ATTACK_TABLE[squareIndex][magicIndex].to_string();

//             if (magicIndex != 4095)
//                 fptr << ", ";
//         }

//         if (squareIndex != 63)
//             fptr << "},";
//         else
//             fptr << "}";
//     }
//     fptr << "\n};\n\n";

//     fptr << "\n/";
//     fptr << "/";
//     fptr << "* BISHOP_ATTACK_TABLE[squareIndex][occupancyBitBoard]\nBitBoard BISHOP_ATTACK_TABLE[64][512] = \n";

//     fptr << "{\n";
//     for (int squareIndex = 0; squareIndex < 64; squareIndex++)
//     {
//         fptr << "\n\t{";
//         for (int magicIndex = 0; magicIndex < 512; magicIndex++)
//         {
//             fptr << BISHOP_ATTACK_TABLE[squareIndex][magicIndex].to_string();

//             if (magicIndex != 511)
//                 fptr << ", ";
//         }

//         if (squareIndex != 63)
//             fptr << "},";
//         else
//             fptr << "}";
//     }
//     fptr << "\n};\n\n";

//     fptr.close();
// }