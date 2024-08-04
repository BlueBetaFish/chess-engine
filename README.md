# Bluebetafish (Chess engine in C++)

## Overview 
- Bluebetafish is a [UCI](https://en.wikipedia.org/wiki/Universal_Chess_Interface "Communication protocol between GUI and engine") chess engine written in C++. 
- It is a command line program which can find best move for a given chess position by searching up to specific depth or by searching for a specific amount of time.
- You can use this CLI program using UCI commands, or you can play against it using any UCI protocol supported chess GUI.
- Approximate Elo: 1700


## Tutorial
- You can download and run the executable from the `bin` directory, or you can compile the source code using `make` command.
- You can use the engine via command line using UCI commands: [Each command is explained later]
    - `position fen r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -`: will initialize the board with given position (input FEN string).
    - `go movetime 5000`: engine finds the best move for 5000 milliseconds.
    - **Output** (`bestmove e2a6`): within 5 seconds, the engine could search upto depth 8 and best move is `e2a6`.  
    <image src='README-FILES/cmdTutorial.png' width=700px>

- Or, play against the engine using my web app: [Github](https://github.com/sourashis59/chess-webapp)

    <image src='README-FILES/chessWebAppCheckmate.gif'>

- You can play against the engine also using any UCI protocol supported GUI. Arena is a free chess GUI, which you can download from here: http://www.playwitharena.de/

    <image src='README-FILES/ArenaTutorial.gif'>



## Supported UCI commands
### *GUI to Engine*
- ### `position [fen  | startpos]  moves  ....`:
    - Sets up the position described in FEN string on the internal board and
    plays the moves on the internal chess board.
    - Pass `startpos` as argument if you want to initialize the board to starting position.
    - `moves` is an optional argument. If it is not passed, no move is played after the given fen/startpos.
    - Examples: 
        - `position startpos`: initializes the board with starting position

            <image src='README-FILES/positionStartpos.png' width='500'>

        - `position fen {FEN_STRING}`: initializes the board with given position using FEN notation  

            <image src='README-FILES/positionFen.png'>

        - `position startpos moves e2e4 e7e5 g2g3`: initializes the board with starting position and plays the given moves.

            <image src='README-FILES/positionStartposMoves.png' width='500'>
- ### `go`:
    - Starts calculating best move for the current position set up with the "position" command.
    - There are different arguments for this command.
    - `depth {DEPTH_VALUE}`
        - Searches the position upto depth = "DEPTH_VALUE"
        - Example: `go depth 6`

            <image src='README-FILES/goDepth.png' width='500'>
            
    - `movetime {MOVETIME_VALUE}`
        - Searches the position for "MOVE_TIME" milliseconds
        - Example: `go movetime 5000` (Searches for 5 seconds)

            <image src='README-FILES/goMovetime.png' width='500'>

    - `wtime {WTIME_VALUE}` :  Searches the position assuming white has "WTIME_VALUE" milliseconds remaining
    - `btime {BTIME_VALUE}` :  Searches the position assuming black has "BTIME_VALUE" milliseconds remaining
        - Example: `go wtime 180000` (Searches the position assuming white has 180 seconds)

            <image src='README-FILES/goWtime.png' width='500'>

    - `winc` : white increment per move in milliseconds
    - `binc` : black increment per move in milliseconds
    - `movestogo {MOVESTOGO}` : time increment begins after "MOVESTOGO" moves                 

- ### `quit` 
    - The program quits.        


----
---
### *Engine to GUI*
- ### `bestmove {MOVE}`
    - The engine has stopped searching and found the best move for the current position.
    - Example: `bestmove e2e4` [Here best move is *e2e4*]
- ### `info`
    - Example:
        ```
        go movetime 1000
        info score cp 30 depth 1 nodes 25 time 0 pv d2d4
        info score cp 0 depth 2 nodes 74 time 0 pv d2d4 d7d5
        info score cp 30 depth 3 nodes 571 time 1 pv d2d4 d7d5 b1c3
        info score cp 0 depth 4 nodes 1909 time 3 pv d2d4 d7d5 b1c3 b8c6
        info score cp 30 depth 5 nodes 11608 time 14 pv d2d4 d7d5 b1c3 b8c6 g1f3
        info score cp 0 depth 6 nodes 13051 time 31 pv d2d4 d7d5 b1c3 b8c6 g1f3 g8f6
        info score cp 20 depth 7 nodes 71258 time 98 pv d2d4 d7d5 b1c3 b8c6 g1f3 g8f6 c1f4
        info score cp 10 depth 8 nodes 213771 time 313 pv e2e4 b8c6 g1f3 g8f6 e4e5 f6d5 c2c4 d5f4
        info score cp 30 depth 9 nodes 518978 time 830 pv e2e4 b8c6 d2d4 g8f6 e4e5 f6d5 g1f3 d7d6 b1c3
        bestmove e2e4

        Total Nodes of iterative deepening : 829440
        ```
    - Engine output with more informations.    
    - `depth` : current depth upto which search has been done.
    - `time` : time (in millisecond) taken to complete the current DFS.
    - `nodes` : number of nodes searched
    - `pv {MOVE1} {MOVE2} {MOVE3} .....` : the principal variation line (best move sequence for both players) found.
    - `score cp {SCORE}` :  the score (evaluation) in centi-pawn unit from engine's POV.
