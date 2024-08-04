# Bluebetafish (Chess engine in C++)

## Overview 
- Bluebetafish is a [UCI](https://en.wikipedia.org/wiki/Universal_Chess_Interface "Communication protocol between GUI and engine") chess engine in C++. 
- It is a command line program which can find best move for a given chess position by searching upto specific depth or by searching for specific amount of time.
- You can use this CLI program using UCI commands, or you can play against it using any UCI protocol supported chess GUI.
- Approximate Elo: 1700


## Tutorial
- You can download the executable from `bin` directory, or you can compile the source code using `make` command.
- You can use the CLI program using UCI commands: [Each command is explained later]
    - `position fen r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -`: will initialize the board with given position via the FEN string notation.
    - `go movetime 5000`: the engine finds best move for 5000 millisecond.
    - **Output** (`bestmove e2a6`): Within 5 seconds, the engine could search upto depth 8 and best move is `e2a6`.  
    <image src='README-FILES/cmdTutorial.png' >

- Or, play against the engine using my web app: [Github](https://github.com/sourashis59/chess-webapp)
    <image src='README-FILES/chessWebAppCheckmate.gif' width=600px>

- You can play against the engine also using any UCI protocol supported GUI. Arena is a free chess GUI, which you can download from here : http://www.playwitharena.de/
    <image src='README-FILES/ArenaTutorial.gif' width=600px>



## Supported UCI commands
### *GUI to Engine*
- ### `position [fen  | startpos ]  moves  ....`
    - Sets up the position described in fenstring on the internal board and
    plays the moves on the internal chess board.
    - Pass `startpos` as argument if you want to initialize the board to starting position.
    - `moves` is optional argument. If it is not passed, no moves are played after the given fen/startpos.
    - Example: 
        - Run the command `position startpos` :

            <image src='README-FILES/positionStartpos.png' width='500'>
        - Run the commande `position fen "FEN_STRING"`  

            <image src='README-FILES/positionFen.png'>
        - Run the command `position startpos moves e2e4 e7e5 g2g3`

            <image src='README-FILES/positionStartposMoves.png' width='500'>
- ### `go`
    - Start calculating on the current position set up with the "position" command.
    - There are a number of commands that can follow this command, all will be sent in the same string. If one command is not send its value should be interpreted as it would not influence the search.
    - `depth "DEPTH_VALUE"`
        - Searches the position upto depth = "DEPTH_VALUE"
        - Example : Run the command `go depth 6`
            <image src='README-FILES/goDepth.png' width='500'>
    - `movetime "MOVETIME_VALUE"`
        - Searches the position for "MOVE_TIME" milliseconds
        - Example : Run the command `go movetime 5000` (Searches for 5 seconds)
            <image src='README-FILES/goMovetime.png' width='500'>
    - `wtime "WTIME_VALUE"` :  Searches the position assuming white has "WTIME_VALUE" milliseconds remaining
    - `btime "BTIME_VALUE"` :  Searches the position assuming black has "BTIME_VALUE" milliseconds remaining
        - Example : Run the command `go wtime 180000` (Searches the position assuming white has 180 seconds)
            <image src='README-FILES/goWtime.png' width='500'>
    - `winc` : white increment per move in milliseconds
    - `binc` : black increment per move in milliseconds
    - `movestogo "MOVESTOGO"` : time increment begins after "MOVESTOGO" moves                 
- ### `stop`
    - Use this command to stop the engine while it is calculating.
    - Example: (the engine stops while calculating for depth=9)                 
        <image src='README-FILES/stop.png' width='500'>
- ### `quit` 
    - The program quits.        



### *Engine to GUI*
- ### `bestmove "MOVE"`
    - The engine has stopped searching and found the best move for the current position.
- ### `info`
    - The engine wants to send infos to the GUI.    
    - The engine can send only selected infos and multiple infos can be send with one info command.
    - `depth` : current depth upto which search has been done.
    - `time` : time searched in millisecond.
    - `nodes` : number of nodes searched
    - `pv "MOVE1" "MOVE2" "MOVE3" .....` : the principal variation line(best move sequence for both players) found.
    - `score cp "SCORE"` :  the score(evaluation) in centi-pawn unit from engine's POV.
