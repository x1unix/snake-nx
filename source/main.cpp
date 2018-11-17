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
#define V_HEIGHT 25
#define V_WIDTH 50

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
    // This example uses a text console, as a simple way to output text to the screen.
    // If you want to write a software-rendered graphics application,
    //   take a look at the graphics/simplegfx example, which uses the libnx gfx API instead.
    // If on the other hand you want to write an OpenGL based application,
    //   take a look at the graphics/opengl set of examples, which uses EGL instead.
    consoleInit(NULL);

    Game game(V_HEIGHT, V_WIDTH);
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
