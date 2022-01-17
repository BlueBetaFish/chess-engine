out << "{ ";
    for (int squareIndex = 0; squareIndex < 64; squareIndex++)
    {

        cout << maskKnightAttacks(squareIndex).getDecimalValue() << "ULL";
        if (squareIndex != 63)
            cout << ", ";
    }
    c