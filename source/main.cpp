// Include the most common headers from the C standard library
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <switch.h>
#include <game.h>


////////////////////////
// Game env settings
///////////////////////

// Speed in nanoseconds
// 300 ms  -  300000000
//   5 sec -  5000000000
#define SPEED 150000000ULL
#define LEVEL 1

// Messages
#define MSG_WIN "Congratulations! You won the game!"
#define MSG_LOST "YOU LOST!"
#define MSG_SCORE "\n\nYour score is: %d\nPress '+' to exit."

using namespace std;

// Button to direction mappings (switch only)
map<u64, Direction> dbg_keys = {
    {KEY_DUP, UP},
    {KEY_DDOWN, DOWN},
    {KEY_DLEFT, LEFT},
    {KEY_DRIGHT, RIGHT},
};

Direction decodeButtonDirection(u64 kDown) {
    for (auto it = dbg_keys.begin(); it != dbg_keys.end(); ++it) {
        if (kDown & it->first) {
            return it->second;
        }
    }
    return NOP;
}

void onGameFinish(GameState state, int score) {
    consoleClear();
    if (state == WON) {
        cout << MSG_WIN;
    } else {
        cout << MSG_LOST;
    }

    printf(MSG_SCORE, score);
}

// Main program entrypoint
int main(int argc, char* argv[])
{
    bool gameOver = false;
    PrintConsole* con = consoleInit(NULL);

    // 1px offset workaround to prevent screen autoscroll
    Game game(con->windowHeight - 2, con->windowWidth - 1, con);
    game.setSpeed(SPEED);
    game.Draw();

    // Main loop
    while (appletMainLoop())
    {
        // Scan all the inputs. This should be done once for each frame
        hidScanInput();

        // hidKeysDown returns information about which buttons have been
        // just pressed in this frame compared to the previous one
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS)
            break; // break in order to return to hbmenu
        
        if (gameOver == true) {
            consoleUpdate(NULL);
            continue;
        }

        GameState gameState = game.getState();
        if (gameState == PLAYING) {
            Direction d = decodeButtonDirection(kDown);
            game.RefreshPosition();
            
            if (d != NOP) {
                game.setDirection(d);
            }

            game.Process();
        } else {
            gameOver = true;
            onGameFinish(gameState, game.getScore());
        }
        // Update the console, sending a new frame to the display
        consoleUpdate(NULL);
    }

    // Deinitialize and clean up resources used by the console (important!)
    consoleExit(NULL);
    return 0;
}