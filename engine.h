// Name: SOURASHIS MONDAL

#include <bits/stdc++.h>

#define U64 unsigned long long

//*Macros for bit manipulation

using namespace std;
class BitBoard;

enum
{
    WHITE,
    BLACK
};
enum
{
    a8,
    b8,
    c8,
    d8,
    e8,
    f8,
    g8,
    h8,
    a7,
    b7,
    c7,
    d7,
    e7,
    f7,
    g7,
    h7,
    a6,
    b6,
    c6,
    d6,
    e6,
    f6,
    g6,
    h6,
    a5,
    b5,
    c5,
    d5,
    e5,
    f5,
    g5,
    h5,
    a4,
    b4,
    c4,
    d4,
    e4,
    f4,
    g4,
    h4,
    a3,
    b3,
    c3,
    d3,
    e3,
    f3,
    g3,
    h3,
    a2,
    b2,
    c2,
    d2,
    e2,
    f2,
    g2,
    h2,
    a1,
    b1,
    c1,
    d1,
    e1,
    f1,
    g1,
    h1,
};

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
    BitBoard(U64 value = 0ULL)
    {
        this->value = value;
    }

    int inline getBitAt(int index) const
    {
        return ((this->value & (1ULL << index)) == 0) ? 0 : 1;
    }

    void inline setBitAt(int index)
    {
        this->value = (this->value | (1ULL << index));
    }

    void inline toggleBitAt(int index)
    {
        this->value = (this->value ^ (1ULL << index));
    }

    void inline unsetBitAt(int index)
    {
        if (getBitAt(index) == 1)
            toggleBitAt(index);
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
};

//*Constants

/*
    * Not a file :
                    8    0  1  1  1  1  1  1  1
                    7    0  1  1  1  1  1  1  1
                    6    0  1  1  1  1  1  1  1
                    5    0  1  1  1  1  1  1  1
                    4    0  1  1  1  1  1  1  1
                    3    0  1  1  1  1  1  1  1
                    2    0  1  1  1  1  1  1  1
                    1    0  1  1  1  1  1  1  1

                        a  b  c  d  e  f  g  h

                        Decimal value : 18374403900871474942


    * Not h file :

                    8    1  1  1  1  1  1  1  0
                    7    1  1  1  1  1  1  1  0
                    6    1  1  1  1  1  1  1  0
                    5    1  1  1  1  1  1  1  0
                    4    1  1  1  1  1  1  1  0
                    3    1  1  1  1  1  1  1  0
                    2    1  1  1  1  1  1  1  0
                    1    1  1  1  1  1  1  1  0

                        a  b  c  d  e  f  g  h

                        Decimal value : 9187201950435737471
*/

const BitBoard NOT_A_FILE = 18374403900871474942ULL;
const BitBoard NOT_H_FILE = 9187201950435737471ULL;

//*_____________________________ATTACK TABLES____________________________________________________________________

//*PAWN ATTACK TABLE______________________________________
// vector<vector<BitBoard>> pawnAttacks(2, vector<BitBoard>(8 * 8, 0)); // pawnAttacks[playerColor][squareIndex]

// BitBoard maskPawnAttacks(int playerColor, int squareIndex)
// {
//     BitBoard result(0ULL);
// }
