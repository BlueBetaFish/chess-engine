#pragma once

// #include <bits/stdc++.h>

#include <vector>
#include <string>
#include <assert.h>
#include <stdexcept>
#include <iostream>
#include <unistd.h>

#ifdef WIN64
#include <windows.h>
#else
#include <sys/time.h>
#endif



#define START_POSITION_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0"
#define PRUNING_TEST_POSITION "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 "
#define TRICKY_POSITION_FEN "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
#define KILLER_MOVE_TEST_POSITION "6k1/3q1pp1/pp5p/1r5n/8/1P3PP1/PQ4BP/2R3K1 w - - 0 1"
#define KILLER_MOVE_TEST_POSITION1 "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"


//*TODO: comment it after testing to disable the assert() calls
// #define NDEBUG