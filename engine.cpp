// Name: SOURASHIS MONDAL

#include <bits/stdc++.h>

using namespace std;
class BitBoard;

#include "definitions.h"

#include "BitBoard.h"

#include "tables.h"

//*helper functions
#include "helperFunctions.h"

//*Constants

//*_____________________________ATTACK TABLES____________________________________________________________________

void initializeSliderPieceAttackTables()
{
    for (int squareIndex = 0; squareIndex < 64; squareIndex++)
    {
        //*for bishop_____________________________________________________________________________
        int relevantBitCount = BISHOP_ATTACK_MASK_SET_BIT_COUNT[squareIndex];
        // initialize occupancyIndex
        int occupancyIndex = (1 << relevantBitCount);

        for (int index = 0; index < occupancyIndex; index++)
        {
            // initilalize current occupancy variation
            BitBoard occupancy = setOccupancy(index, BISHOP_ATTACK_MASK[squareIndex]);

            // initialize magic index
            U64 magicIndex = (occupancy.getDecimalValue() * BISHOP_MAGIC_NUMBER[squareIndex]) >> (64 - relevantBitCount);

            // initialize bishop attacks
            BISHOP_ATTACK_TABLE[squareIndex][magicIndex] = generateBishopAttacksOnTheFly(squareIndex, occupancy);
        }

        //*for rook_____________________________________________________________________________
        relevantBitCount = ROOK_ATTACK_MASK_SET_BIT_COUNT[squareIndex];
        // initialize occupancyIndex
        occupancyIndex = (1 << relevantBitCount);

        for (int index = 0; index < occupancyIndex; index++)
        {
            // initilalize current occupancy variation
            BitBoard occupancy = setOccupancy(index, ROOK_ATTACK_MASK[squareIndex]);

            // initialize magic index
            U64 magicIndex = (occupancy.getDecimalValue() * ROOK_MAGIC_NUMBER[squareIndex]) >> (64 - relevantBitCount);

            // initialize bishop attacks
            ROOK_ATTACK_TABLE[squareIndex][magicIndex] = generateRookAttacksOnTheFly(squareIndex, occupancy);
        }
    }
}

//*Finds bishop attacks assuming current board occupancy as "blockers"
BitBoard inline getBishopAttacks(int squareIndex, const BitBoard &blockers)
{
    U64 occupancyValue = blockers.getDecimalValue();

    occupancyValue &= BISHOP_ATTACK_MASK[squareIndex].getDecimalValue();
    occupancyValue *= BISHOP_MAGIC_NUMBER[squareIndex];
    occupancyValue >>= (64 - BISHOP_ATTACK_MASK_SET_BIT_COUNT[squareIndex]);

    U64 index = occupancyValue;
    return BISHOP_ATTACK_TABLE[squareIndex][index];
}

//*Finds rook attacks assuming current board occupancy as "blockers"
BitBoard inline getRookAttacks(int squareIndex, const BitBoard &blockers)
{
    U64 occupancyValue = blockers.getDecimalValue();

    occupancyValue &= ROOK_ATTACK_MASK[squareIndex].getDecimalValue();
    occupancyValue *= ROOK_MAGIC_NUMBER[squareIndex];
    occupancyValue >>= (64 - ROOK_ATTACK_MASK_SET_BIT_COUNT[squareIndex]);

    U64 index = occupancyValue;
    return ROOK_ATTACK_TABLE[squareIndex][index];
}

// void initializeLeaperPieceAttackTables()
// {
//     initializePawnAttackTables();
// }

//*must be called at begining
void initializeTables()
{
    initializeSliderPieceAttackTables();
}

int main()
{
    cout << "\n\nHello earth\n\n\n";

    initializeTables();

    BitBoard blockers;
    blockers.setBitAt(a1);
    blockers.setBitAt(c3);
    blockers.setBitAt(d6);
    blockers.setBitAt(d8);
    blockers.setBitAt(g2);
    blockers.setBitAt(g7);
    blockers.setBitAt(h1);
    blockers.setBitAt(h2);
    blockers.setBitAt(f6);
    blockers.setBitAt(d2);
    blockers.setBitAt(d8);
    blockers.setBitAt(b4);
    blockers.setBitAt(a4);
    blockers.setBitAt(d3);
    blockers.setBitAt(a7);

    blockers.print();

    getBishopAttacks(d4, blockers).print();
    getRookAttacks(d4, blockers).print();

    return 0;
}
