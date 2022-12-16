// THIS IS SUCH A MESS
// i'll trim it down in next version and clean up sarcastic comments
// sorry for what you're about to go thru trying to interpret this

// can only be compiled using ce toolchain btw lol so don't even try it ik you want to
// also ignore all the errors you see because
// i can in fact confirm that this compiles onto my calc and runs (somewhat) perfectly

// Necessary libraries
#include <ti/screen.h>
// getcscs.h to parse some input
#include <ti/getcsc.h>
// graphx.h for graphics
#include <graphx.h>
// obviously stdlib.h
#include <stdlib.h>
// keypadc.h for rapidly polling keypad
#include <keypadc.h>

// util.h I forgot what it's for but it's there
#include <sys/util.h>
// same here
#include <sys/rtc.h>
// timers.h necessary for sleep function i think
#include <sys/timers.h>
// include generated sprites from gfx.h
#include "gfx/gfx.h"

// A bunch of random unnecesary functions
// Start's gfx
void begin();
// Updates logo position
void step();
// Draws the logo obv
void drawLogo(int posX, int posY);
// Handles colision physics with walls
void collide();
// Take keypad input
uint8_t keyParse(void);
// Dislay the splashscreen
void splashScreen();
// Center's a string on the screen. returns x coordinate for text
int centerStringX(const char *);

// Define logo
// Logo dimensions
static int LOGO_WIDTH = 56, LOGO_HEIGHT = 30;

// Side note about below, wish i could use vectors but nooo because it's annoying
// Logo position
int logoPos[2]{(GFX_LCD_WIDTH / 2) - (LOGO_WIDTH / 2), (GFX_LCD_HEIGHT / 2) - (LOGO_HEIGHT / 2)};
// Logo velocity
int logoVel[2]{0, 0};

int main(void)
{
    // Start GFX
    begin();

    // Display splashscreen
    splashScreen();
    // Await the user pressing enter to start or clear to exit
    while (true)
    {
        if (os_GetCSC() == sk_Enter)
            break;
        // Exit
        else if (os_GetCSC() == sk_Clear)
        {
            gfx_End();
            return 0;
        }
    }

    // Main loop run while user hasn't pressed clear
    while (os_GetCSC() != sk_Clear)
    {
        // Draw
        drawLogo(logoPos[0], logoPos[1]);

        // Parse keypad input and change velocity according to arrow keys
        uint8_t key = keyParse();
        switch (key)
        {
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

        // Update position and check collision physics
        step();
        collide();

        // Queue current frame for display
        gfx_SwapDraw();

        // Wait
        // honestly i don't even think the sleep function works bc sleep takes int as arg
        // so 0.1 rounds down to 0 but i'm too lazy to test rn i'll fix later
        sleep(0.1);
    }

    // Exit
    gfx_End();
    return 0;
}

void begin()
{
    // Starts drawing frames
    gfx_SetDrawBuffer();
    // Clears the screen
    os_ClrHome();
    // Fills the screen completely white
    gfx_FillScreen(0xFF);
    // Starts drawing
    gfx_Begin();
}

// kinda self explanatory here
void step()
{
    logoPos[0] += logoVel[0];
    logoPos[1] += logoVel[1];
}

void drawLogo(int posX, int posY)
{
    // Fills the screen white
    gfx_FillScreen(0xFF);
    // Draws dvd logo in correct(ish) position on top of white bg
    gfx_TransparentSprite(dvd_logo, posX, posY);
}

void collide()
{
    // Reverse x velocity if it hits a horizontal wall
    if (logoPos[0] < 0 || logoPos[0] + LOGO_WIDTH > GFX_LCD_WIDTH)
        logoVel[0] *= -1;
    // Reverse x velocity if it hits a vertical wall
    if (logoPos[1] < 0 || logoPos[1] + LOGO_HEIGHT > GFX_LCD_HEIGHT)
        logoVel[1] *= -1;
}

// Kinda just render a splashscreen not super important
void splashScreen()
{
    gfx_FillScreen(0xFF);
    gfx_PrintStringXY("bounce v0", centerStringX("bounce v0"), 0);
    gfx_TransparentSprite(splashscreen, (GFX_LCD_WIDTH / 2) - (gfx_GetZX7SpriteWidth(splashscreen) / 2), 50);
    gfx_PrintStringXY("use the arrow keys to change velocity", centerStringX("use the arrow keys to change velocity"), 160);
    gfx_PrintStringXY("press enter to begin", centerStringX("press enter to begin"), 170);
}

// Output the x coordinate for a string so that it's centered on screen
// doesn't rly work but im lazy so ykyk
int centerStringX(const char *string)
{
    return (GFX_LCD_WIDTH / 2) - (gfx_GetStringWidth(string) / 2);
}

// completly stolen function idek what it does but it works somehow
// code by jacobly
// https://ce-programming.github.io/toolchain/libraries/keypadc.html
uint8_t keyParse(void)
{
    static uint8_t last_key;
    uint8_t only_key = 0;
    kb_Scan();
    for (uint8_t key = 1, group = 7; group; --group)
    {
        for (uint8_t mask = 1; mask; mask <<= 1, ++key)
        {
            if (kb_Data[group] & mask)
            {
                if (only_key)
                {
                    last_key = 0;
                    return 0;
                }
                else
                {
                    only_key = key;
                }
            }
        }
    }
    if (only_key == last_key)
    {
        return 0;
    }
    last_key = only_key;
    return only_key;
}

// thanks for reading this misery