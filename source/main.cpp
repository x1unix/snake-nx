// Include the most common headers from the C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>

// Include the main libnx system header, for Switch development
#include <switch.h>
using namespace std;

map<u64, string> dbg_keys = {
    {KEY_A, "A"},
    {KEY_B, "B"},
    {KEY_X, "X"},
    {KEY_Y, "Y"},
    {KEY_DUP, "UP"},
    {KEY_DDOWN, "DOWN"},
    {KEY_DLEFT, "LEFT"},
    {KEY_DRIGHT, "RIGHT"},
};

void debugButtonsState(u64 kDown) {
    for (auto it = dbg_keys.begin(); it != dbg_keys.end(); ++it) {
        if (kDown & it->first) {
            printf("Key '%s' was pressed\n", it->second.c_str());
            return;
        }
    }
}

// Main program entrypoint
int main(int argc, char* argv[])
{
    // This example uses a text console, as a simple way to output text to the screen.
    // If you want to write a software-rendered graphics application,
    //   take a look at the graphics/simplegfx example, which uses the libnx gfx API instead.
    // If on the other hand you want to write an OpenGL based application,
    //   take a look at the graphics/opengl set of examples, which uses EGL instead.
    consoleInit(NULL);

    // Other initialization goes here. As a demonstration, we print hello world.
    printf("Hello World!\n");

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
        
        if (kDown > 0) {
            debugButtonsState(kDown);
        }

        // Your code goes here

        // Update the console, sending a new frame to the display
        consoleUpdate(NULL);
    }

    // Deinitialize and clean up resources used by the console (important!)
    consoleExit(NULL);
    return 0;
}
