/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Ending Screen Functions Definitions (Init, Update, Draw, Unload)
*
*   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include "raylib.h"
#include "screens.h"



//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
static int highScorePoints = 0;
static int highScoreTime = 0;

//----------------------------------------------------------------------------------
// Ending Screen Functions Definition
//----------------------------------------------------------------------------------

// Ending Screen Initialization logic
void InitEndingScreen(void)
{
    // TODO: Initialize ENDING screen variables here!
    framesCounter = 0;
    finishScreen = 0;

    //Loading high scores
    highScorePoints = LoadStorageValue(1);
    highScoreTime = LoadStorageValue(0);
}



// Ending Screen Update logic
void UpdateEndingScreen(void)
{
    // TODO: Update ENDING screen variables here!

    // Press enter or tap to return to TITLE screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        finishScreen = 3;
        PlaySound(fxCoin);
    }


    if (IsKeyPressed(KEY_Q))
    {
        finishScreen = 1;
        PlaySound(fxCoin);
    }
}


static void DrawTextExCentered(Font font, const char* text, float fontSize, float spacing, Color tint, int offsetY)
{
    Vector2 position;
    position.x = GetScreenWidth() / 2 - MeasureTextEx(font, text, fontSize, spacing).x / 2;
    position.y = GetScreenHeight() / 2 - MeasureTextEx(font, text, fontSize, spacing).y / 2 + offsetY;
    DrawTextEx(font, text, position, fontSize, spacing, tint);
}

// Ending Screen Draw logic
void DrawEndingScreen(void)
{
    // TODO: Draw ENDING screen here!
    DrawTexturePro(backgroundImage, { 0.0f, 0.0f, (float)backgroundImage.width, (float)backgroundImage.height }, FULL_SCREEN_RECTANGLE, { 0, 0 }, 0, WHITE);

    Vector2 pos = { 20, 10 };

    DrawTextExCentered(font, TextFormat("Highest score: %d", highScorePoints), TITLE_FONT_SIZE, STANDARD_TITLE_SPACING, DARKGREEN, 0);
    DrawTextExCentered(font, TextFormat("Highest time alive: %02d:%02d", (int)highScoreTime / 60, (int)highScoreTime % 60), TITLE_FONT_SIZE, STANDARD_TITLE_SPACING, DARKGREEN, 50);
    DrawTextExCentered(font, "Press enter to play again, press Q to go the main menu", TITLE_FONT_SIZE, STANDARD_TITLE_SPACING, DARKGREEN, 100);
}

// Ending Screen Unload logic
void UnloadEndingScreen(void)
{
    // TODO: Unload ENDING screen variables here!
}

// Ending Screen should finish?
int FinishEndingScreen(void)
{
    return finishScreen;
}