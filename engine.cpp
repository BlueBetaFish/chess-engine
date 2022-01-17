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

//*----------------------------------------helper functions--------------------------------------------

char getFileOfSquareIndex(int index)
{
    return 'a' + index % 8;
}

int getRankOfSquareIndex(int index)
{
    return 8 - (index / 8);
}
string getAlgebraicCoordinateFromIndex(int index)
{
    return getFileOfSquareIndex(index) + to_string(getRankOfSquareIndex(index));
}

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

    //*-----------------------------------------------for testing----------------------------------------------------
    friend BitBoard maskPawnAttacks(int, int);
    //*-----------------------------------------------for testing----------------------------------------------------
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

    * Not g or h file :

                    8    1  1  1  1  1  1  0  0
                    7    1  1  1  1  1  1  0  0
                    6    1  1  1  1  1  1  0  0
                    5    1  1  1  1  1  1  0  0
                    4    1  1  1  1  1  1  0  0
                    3    1  1  1  1  1  1  0  0
                    2    1  1  1  1  1  1  0  0
                    1    1  1  1  1  1  1  0  0

                        a  b  c  d  e  f  g  h

                        Decimal value : 4557430888798830399

    * Not a or b file :
                    8    0  0  1  1  1  1  1  1
                    7    0  0  1  1  1  1  1  1
                    6    0  0  1  1  1  1  1  1
                    5    0  0  1  1  1  1  1  1
                    4    0  0  1  1  1  1  1  1
                    3    0  0  1  1  1  1  1  1
                    2    0  0  1  1  1  1  1  1
                    1    0  0  1  1  1  1  1  1

                        a  b  c  d  e  f  g  h

                        Decimal value : 18229723555195321596
*/

const BitBoard NOT_A_FILE = 18374403900871474942ULL;
const BitBoard NOT_H_FILE = 9187201950435737471ULL;
const BitBoard NOT_GH_FILES = 4557430888798830399ULL;
const BitBoard NOT_AB_FILES = 18229723555195321596ULL;

//*_____________________________ATTACK TABLES____________________________________________________________________

//*PAWN ATTACK TABLE______________________________________
// pawnAttacks[playerColor][squareIndex]
const vector<vector<BitBoard>> PAWN_ATTACK_TABLE =
    {{0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 2ULL, 5ULL, 10ULL, 20ULL, 40ULL, 80ULL, 160ULL, 64ULL, 512ULL, 1280ULL,
      2560ULL, 5120ULL, 10240ULL, 20480ULL, 40960ULL, 16384ULL, 131072ULL, 327680ULL, 655360ULL, 1310720ULL, 2621440ULL, 5242880ULL, 10485760ULL, 4194304ULL, 33554432ULL, 83886080ULL, 167772160ULL, 335544320ULL, 671088640ULL, 1342177280ULL, 2684354560ULL, 1073741824ULL, 8589934592ULL, 21474836480ULL, 42949672960ULL, 85899345920ULL, 171798691840ULL, 343597383680ULL, 687194767360ULL, 274877906944ULL, 2199023255552ULL, 5497558138880ULL, 10995116277760ULL, 21990232555520ULL, 43980465111040ULL, 87960930222080ULL, 175921860444160ULL, 70368744177664ULL, 562949953421312ULL, 1407374883553280ULL, 2814749767106560ULL, 5629499534213120ULL, 11258999068426240ULL, 22517998136852480ULL, 45035996273704960ULL, 18014398509481984ULL},
     {512ULL, 1280ULL, 2560ULL, 5120ULL, 10240ULL, 20480ULL, 40960ULL, 16384ULL, 131072ULL, 327680ULL, 655360ULL, 1310720ULL, 2621440ULL, 5242880ULL, 10485760ULL, 4194304ULL, 33554432ULL, 83886080ULL, 167772160ULL, 335544320ULL, 671088640ULL, 1342177280ULL, 2684354560ULL, 1073741824ULL, 8589934592ULL, 21474836480ULL, 42949672960ULL, 85899345920ULL, 171798691840ULL, 343597383680ULL, 687194767360ULL, 274877906944ULL, 2199023255552ULL, 5497558138880ULL, 10995116277760ULL, 21990232555520ULL, 43980465111040ULL, 87960930222080ULL, 175921860444160ULL, 70368744177664ULL, 562949953421312ULL, 1407374883553280ULL, 2814749767106560ULL, 5629499534213120ULL, 11258999068426240ULL, 22517998136852480ULL, 45035996273704960ULL, 18014398509481984ULL, 144115188075855872ULL, 360287970189639680ULL, 720575940379279360ULL, 1441151880758558720ULL, 2882303761517117440ULL, 5764607523034234880ULL, 11529215046068469760ULL, 4611686018427387904ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL}};

//*KNIGHT ATTACK TABLE______________________________________
// knightAttacks[squareIndex]
const vector<BitBoard> KNIGHT_ATTACK_TABLE =
    {132096ULL, 329728ULL, 659712ULL, 1319424ULL, 2638848ULL, 5277696ULL, 10489856ULL, 4202496ULL, 33816580ULL, 84410376ULL, 168886289ULL, 337772578ULL, 675545156ULL, 1351090312ULL, 2685403152ULL, 1075839008ULL, 8657044482ULL, 21609056261ULL, 43234889994ULL, 86469779988ULL, 172939559976ULL, 345879119952ULL, 687463207072ULL, 275414786112ULL, 2216203387392ULL, 5531918402816ULL, 11068131838464ULL, 22136263676928ULL, 44272527353856ULL, 88545054707712ULL, 175990581010432ULL, 70506185244672ULL, 567348067172352ULL, 1416171111120896ULL, 2833441750646784ULL, 5666883501293568ULL, 11333767002587136ULL, 22667534005174272ULL, 45053588738670592ULL, 18049583422636032ULL, 145241105196122112ULL, 362539804446949376ULL, 725361088165576704ULL, 1450722176331153408ULL, 2901444352662306816ULL, 5802888705324613632ULL, 11533718717099671552ULL, 4620693356194824192ULL, 288234782788157440ULL, 576469569871282176ULL, 1224997833292120064ULL, 2449995666584240128ULL, 4899991333168480256ULL, 9799982666336960512ULL, 1152939783987658752ULL, 2305878468463689728ULL, 1128098930098176ULL, 2257297371824128ULL, 4796069720358912ULL, 9592139440717824ULL, 19184278881435648ULL, 38368557762871296ULL, 4679521487814656ULL, 9077567998918656ULL};

// kingAttacks[squareIndex]
const vector<BitBoard> KING_ATTACK_TABLE = {770ULL, 1797ULL, 3594ULL, 7188ULL, 14376ULL, 28752ULL, 57504ULL, 49216ULL, 197123ULL, 460039ULL, 920078ULL, 1840156ULL, 3680312ULL, 7360624ULL, 14721248ULL, 12599488ULL, 50463488ULL, 117769984ULL, 235539968ULL, 471079936ULL, 942159872ULL, 1884319744ULL, 3768639488ULL, 3225468928ULL, 12918652928ULL, 30149115904ULL, 60298231808ULL, 120596463616ULL, 241192927232ULL, 482385854464ULL, 964771708928ULL, 825720045568ULL, 3307175149568ULL, 7718173671424ULL, 15436347342848ULL, 30872694685696ULL, 61745389371392ULL, 123490778742784ULL, 246981557485568ULL, 211384331665408ULL, 846636838289408ULL, 1975852459884544ULL, 3951704919769088ULL, 7903409839538176ULL, 15806819679076352ULL, 31613639358152704ULL, 63227278716305408ULL, 54114388906344448ULL, 216739030602088448ULL, 505818229730443264ULL, 1011636459460886528ULL, 2023272918921773056ULL, 4046545837843546112ULL, 8093091675687092224ULL, 16186183351374184448ULL, 13853283560024178688ULL, 144959613005987840ULL, 362258295026614272ULL, 724516590053228544ULL, 1449033180106457088ULL, 2898066360212914176ULL, 5796132720425828352ULL, 11592265440851656704ULL, 4665729213955833856ULL};

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

void printKingAttacktable(const vector<BitBoard> &kingAttackTable)
{
    for (int squareIndex = 0; squareIndex < 64; squareIndex++)
    {
        cout << "\nfor square : " << getAlgebraicCoordinateFromIndex(squareIndex) << " : \n";
        kingAttackTable[squareIndex].print();
    }

    // cout << "{ ";
    // for (int squareIndex = 0; squareIndex < 64; squareIndex++)
    // {

    //     cout << kingAttackTable[squareIndex].getDecimalValue() << "ULL";
    //     if (squareIndex != 63)
    //         cout << ", ";
    // }
    // cout << " } \n";
}

// void initializeLeaperPieceAttackTables()
// {
//     initializePawnAttackTables();
// }

int main()
{
    cout << "\n\nHello earth\n\n\n";

    return 0;
}