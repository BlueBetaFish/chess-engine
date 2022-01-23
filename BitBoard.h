#pragma once

#include <bits/stdc++.h>
using namespace std;

#include "definitions.h"

class BitBoard;

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
class BitBoard
{
    U64 value;

public:
    string inline to_string()
    {
        return std::to_string(this->value);
    }

    bool inline isSquareIndexValid(int squareIndex) const
    {
        return (0 <= squareIndex && squareIndex < 64);
    }

    void inline checkSquareIndexValidity(int squareIndex) const
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

    //*-----------------------------------------------for testing----------------------------------------------------
};