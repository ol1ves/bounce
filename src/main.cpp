#include <ti/screen.h>
#include <ti/getcsc.h>
#include <graphx.h>
#include <stdlib.h>

#include <sys/util.h>
#include <sys/rtc.h>
#include <sys/timers.h>
#include "gfx/gfx.h"

void begin();
void end();
bool step();
void draw(int posX, int posY);
void collide();

static int LOGO_WIDTH = 56, LOGO_HEIGHT = 30; 



int logoPos[2] {0, 0};
int logoVel[2] {1, 1};

int main(void)
{
    gfx_SetDrawBuffer();

    // Begin
    begin();
    
    // Enter palletized mode
    gfx_Begin();

    // While no keypress
    while (!step()) {

        logoPos[0] += logoVel[0];
        logoPos[1] += logoVel[1];

        collide();

        gfx_ZeroScreen();

        // Draw the current frame
        draw(logoPos[0], logoPos[1]);

        // Queue the current frame for display
        gfx_SwapDraw();

        sleep(0.1);
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

void draw(int posX, int posY) {
    gfx_TransparentSprite(dvd_logo, posX, posY);
}

void collide() {
    if (logoPos[0] < 0 || logoPos[0] + LOGO_WIDTH > GFX_LCD_WIDTH) logoVel[0] *= -1;
    if (logoPos[1] < 0 || logoPos[1] + LOGO_HEIGHT > GFX_LCD_HEIGHT) logoVel[1] *= -1;
}