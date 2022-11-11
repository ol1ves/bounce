#include <ti/screen.h>
#include <ti/getcsc.h>
#include <graphx.h>
#include <stdlib.h>
#include <keypadc.h>

#include <sys/util.h>
#include <sys/rtc.h>
#include <sys/timers.h>
#include "gfx/gfx.h"

void begin();
void step();
void drawLogo(int posX, int posY);
void collide();
uint8_t keyParse(void);
void splashScreen();
int centerStringX(const char *);

// Define logo
static int LOGO_WIDTH = 56, LOGO_HEIGHT = 30; 
int logoPos[2] {(GFX_LCD_WIDTH / 2) - (LOGO_WIDTH / 2), (GFX_LCD_HEIGHT / 2) - (LOGO_HEIGHT / 2)};
int logoVel[2] {0, 0};

int main(void)
{
    // Start GFX
    begin();

    // Splashscreen
    splashScreen();
    while (true) {
        if (os_GetCSC() == sk_Enter) break;
        else if (os_GetCSC() == sk_Clear) {
            gfx_End();
            return 0;
        }
    }

    // Main loop
    while (os_GetCSC() != sk_Clear) {
        // Draw
        drawLogo(logoPos[0], logoPos[1]);

        // Parse keypad input
        uint8_t key = keyParse();
        switch (key) {
            case sk_Up:
                logoVel[1]--;
                break;
            case sk_Down:
                logoVel[1]++;
                break;
            case sk_Left:
                logoVel[0]--;
                break;
            case sk_Right:
                logoVel[0]++;
                break;
        }

        // Physics
        step();
        collide();

        // Queue current frame for display
        gfx_SwapDraw();
        
        // Wait
        sleep(0.1);
    }
    
    // Exit
    gfx_End();
    return 0;
}

void begin() {
    gfx_SetDrawBuffer();
    os_ClrHome();
    gfx_FillScreen(0xFF);
    gfx_Begin();
}

void step() {
    logoPos[0] += logoVel[0];
    logoPos[1] += logoVel[1];
}

void drawLogo(int posX, int posY) {
    gfx_FillScreen(0xFF);
    gfx_TransparentSprite(dvd_logo, posX, posY);
}

void collide() {
    if (logoPos[0] < 0 || logoPos[0] + LOGO_WIDTH > GFX_LCD_WIDTH) logoVel[0] *= -1;
    if (logoPos[1] < 0 || logoPos[1] + LOGO_HEIGHT > GFX_LCD_HEIGHT) logoVel[1] *= -1;
}

void splashScreen() {
    gfx_FillScreen(0xFF);
    gfx_PrintStringXY("bounce v0", centerStringX("bounce v0"), 0);
    gfx_TransparentSprite(splashscreen, (GFX_LCD_WIDTH / 2) - (gfx_GetZX7SpriteWidth(splashscreen) / 2), 50);
    gfx_PrintStringXY("use the arrow keys to change velocity", centerStringX("use the arrow keys to change velocity"), 160);
    gfx_PrintStringXY("press enter to begin", centerStringX("press enter to begin"), 170);
}

int centerStringX(const char * string) {
    return (GFX_LCD_WIDTH / 2) - (gfx_GetStringWidth(string) / 2);
}

// code by jacobly
// https://ce-programming.github.io/toolchain/libraries/keypadc.html
uint8_t keyParse(void) {
    static uint8_t last_key;
    uint8_t only_key = 0;
    kb_Scan();
    for (uint8_t key = 1, group = 7; group; --group) {
        for (uint8_t mask = 1; mask; mask <<= 1, ++key) {
            if (kb_Data[group] & mask) {
                if (only_key) {
                    last_key = 0;
                    return 0;
                } else {
                    only_key = key;
                }
            }
        }
    }
    if (only_key == last_key) {
        return 0;
    }
    last_key = only_key;
    return only_key;
}

