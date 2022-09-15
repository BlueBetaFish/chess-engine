#pragma once

#include "neededHeaders.h"
#include "definitions.h"
#include "helperFunctions.h"

using namespace std;




//*Class to store the informations of the user's "go" command to search a position
/*
*   quit: flag to denote if the user wants to quit 
*   movestogo: number of moves to be palyed within total time (to get increment)
*   movetime: search for the given amount of time
*   time: time to search
*   inc: amount of time to be incremented after each move
*   starttime: starttime(in millisecond) : when the engine started searching a position
*   stoptime: stoptime(in millisecond) : when the engine will stop searching
*   timeset: flag to denote if the searching is to be done for limited time(if not, then searching is to be done upto given depth)
*   stopped: flag to denote if the searching is stopped
*
*
*
*
*/
class UCISearchInfo
{
public:
    bool quit;
    int movestogo;
    int movetime; //* -1 denotes that movetime is not set
    int time; //* -1 denotes that time is not set
    int inc;  //* 0 denotes that inc is not set
    int starttime; //* 0 denotes that starttime is not set
    int stoptime; //* 0 denotes that stoptime is not set
    bool timeset;
    bool stopped;

public:

    //*constructor
    UCISearchInfo()
    {
        resetUCISearchInfo();
    }

    //*reset the properties
    void resetUCISearchInfo()
    {
        quit = false;

        /*
        * by default we will set "movestogo" to 30
        * so the engine will use (total time of the current player/ 30) for a single move
        * 
        * 
        * For example:
        *   if white has total time 180 seconds, then the engine will search for 180/30 = 6 seconds (commande : go wtime 180000)
        *   in the next move white has total time 174 seconds, so the engine will search for 174/6 seconds
        *   ........ and so on
        */
        movestogo = 30;
        movetime = -1;
        time = -1;
        inc = 0;
        starttime = 0;
        stoptime = 0;
        timeset = false;
        stopped = false;
    }

    //* a bridge function to interact between search and GUI input
    //*sets stopped=true, if the user has entered something in stdin while the search is going on
    inline void communicateWithGUI()
    {
        //* if time is over
        if (timeset && getTimeInMilliSeconds() > stoptime)
            stopped = true;

        //* read GUI/user input
        read_input();
    }



private:

    /*
     *
     *    function to listen to GUI's input during search.
     *    it's waiting for the user input from STDIN.
     *    OS dependent.
     *
     */
    //*Reference : https://www.youtube.com/watch?v=NBl92Vs0fos&list=PLZ1QII7yudbc-Ky058TEaOstZHVbT-2hg&index=66)
    inline int input_waiting()
    {
        #ifndef WIN32
            fd_set readfds;
            struct timeval tv;
            FD_ZERO(&readfds);
            FD_SET(fileno(stdin), &readfds);
            tv.tv_sec = 0;
            tv.tv_usec = 0;
            select(16, &readfds, 0, 0, &tv);

            return (FD_ISSET(fileno(stdin), &readfds));
        #else
            static int init = 0, pipe;
            static HANDLE inh;
            DWORD dw;

            if (!init)
            {
                init = 1;
                inh = GetStdHandle(STD_INPUT_HANDLE);
                pipe = !GetConsoleMode(inh, &dw);
                if (!pipe)
                {
                    SetConsoleMode(inh, dw & ~(ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT));
                    FlushConsoleInputBuffer(inh);
                }
            }

            if (pipe)
            {
                if (!PeekNamedPipe(inh, NULL, 0, NULL, &dw, NULL))
                    return 1;
                return dw;
            }

            else
            {
                GetNumberOfConsoleInputEvents(inh, &dw);
                return dw <= 1 ? 0 : dw;
            }

        #endif
    }

    //* reads GUI/user input
    //*Reference : https://www.youtube.com/watch?v=NBl92Vs0fos&list=PLZ1QII7yudbc-Ky058TEaOstZHVbT-2hg&index=66)
    inline void read_input()
    {
        // bytes to read holder
        int bytes;

        // GUI/user input
        char input[256] = "", *endc;

        // "listen" to STDIN
        if (input_waiting())
        {
            // tell engine to stop calculating
            stopped = true;

            // loop to read bytes from STDIN
            do
            {
                // read bytes from STDIN
                bytes = read(fileno(stdin), input, 256);
            }
            // until bytes available
            while (bytes < 0);

            // searches for the first occurrence of '\n'
            endc = strchr(input, '\n');

            // if found new line set value at pointer to 0
            if (endc)
                *endc = 0;

            // if input is available
            if (strlen(input) > 0)
            {
                // match UCI "quit" command
                if (!strncmp(input, "quit", 4))
                {
                    // tell engine to terminate exacution
                    quit = true;
                }

                // // match UCI "stop" command
                else if (!strncmp(input, "stop", 4))
                {
                    // tell engine to terminate exacution
                    quit = true;
                }
            }
        }
    }

    
};