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
#define SPEED 300000000ULL
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

#ifdef TEST
int main(int argc, char *argv[]) {
    PrintConsole* con = consoleInit(NULL);
    int y = con->windowHeight;
    int x = con->windowWidth;   // 44?

    for (int i = 0; i < y; i++) {
        if (i == 0 || i == (y - 1)) {
            for (int j = 0; j < x; j++) {
                printf("X");
            }
            continue;
        }
        printf("Y\n");
    }
    //printf("Height: %d\nWidth: %d\n", con->windowHeight, con->windowWidth);

    while (appletMainLoop())
    {
        // Scan all the inputs. This should be done once for each frame
        hidScanInput();

        // hidKeysDown returns information about which buttons have been
        // just pressed in this frame compared to the previous one
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS)
            break; // break in order to return to hbmenu
        
        consoleUpdate(NULL);
    }

    consoleExit(NULL);
    return 0;
}
#endif
#ifndef TEST
// Main program entrypoint
int main(int argc, char* argv[])
{
    bool gameOver = false;
    // This example uses a text console, as a simple way to output text to the screen.
    // If you want to write a software-rendered graphics application,
    //   take a look at the graphics/simplegfx example, which uses the libnx gfx API instead.
    // If on the other hand you want to write an OpenGL based application,
    //   take a look at the graphics/opengl set of examples, which uses EGL instead.
    PrintConsole* con = consoleInit(NULL);

    // 1px offset workaround to prevent screen autoscroll
    Game game(con->windowHeight - 2, con->windowWidth - 1);
    game.setSpeed(SPEED);

    // Other initialization goes here. As a demonstration, we print hello world.
    //printf("Hello World!\n");

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
            continue;
        }

        GameState gameState = game.getState();
        if (gameState != PLAYING) {
            gameOver = true;
            onGameFinish(gameState, game.getScore());
            continue;
        }

        Direction d = decodeButtonDirection(kDown);
        game.DrawTable();
        
        if (d != NOP) {
            game.setDirection(d);
        }

        game.Process();

        // Update the console, sending a new frame to the display
        consoleUpdate(NULL);
    }

    // Deinitialize and clean up resources used by the console (important!)
    consoleExit(NULL);
    return 0;
}
#endif