// Name: SOURASHIS MONDAL

#include <bits/stdc++.h>
#include "engine.h"

using namespace std;

void printCoordinatesForEnums()
{
    for (int rank = 8; rank >= 1; rank--)
    {
        printf("a%d, b%d, c%d, d%d, e%d, f%d, g%d, h%d,", rank, rank, rank, rank, rank, rank, rank, rank);
        cout << endl;
    }
}

BitBoard get_not_a_file_BitBoard()
{
    BitBoard result = 0ULL;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            int index = i * 8 + j;
            if (j != 0)
                result.setBitAt(index);
        }
    }

    return result;
}

BitBoard get_not_h_file_BitBoard()
{
    BitBoard result = 0ULL;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            int index = i * 8 + j;
            if (j != 7)
                result.setBitAt(index);
        }
    }

    return result;
}

BitBoard get_not_gh_file_BitBoard()
{
    BitBoard result = 0ULL;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            int index = i * 8 + j;
            if (j != 7 && j != 6)
                result.setBitAt(index);
        }
    }

    return result;
}

BitBoard get_not_ab_file_BitBoard()
{
    BitBoard result = 0ULL;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            int index = i * 8 + j;
            if (j != 0 && j != 1)
                result.setBitAt(index);
        }
    }

    return result;
}
int main()
{
    cout << "\n\nHello earth\n\n\n";

    get_not_ab_file_BitBoard().print();

    return 0;
}