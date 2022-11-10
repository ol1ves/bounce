#include <ti/screen.h>
#include <ti/getcsc.h>
#include <graphx.h>
#include <stdlib.h>

#include "gfx/gfx.h"

void begin();
void end();
bool step();
void draw();

int logoPos[2] {0, 0};

int main(void)
{
    // Begin
    begin();
    
    // Enter palletized mode
    gfx_Begin();

    // While no keypress
    while (!step()) {
        // Draw the current frame
        draw();

        // Queue the current frame for display
        gfx_SwapDraw();
    }
    
    // Exit palletized mode and exit
    gfx_End();
    return 0;
}

void begin() {
    os_ClrHome();
}

void end() {

}

bool step() {
    // Return true if keypress is recieved 
    return os_GetCSC();
}

void draw() {
    gfx_TransparentSprite(dvd_logo, logoPos[0], logoPos[1]);
}