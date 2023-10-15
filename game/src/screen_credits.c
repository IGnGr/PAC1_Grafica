/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Credits Screen Functions Definitions (Init, Update, Draw, Unload)
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

#define STANDARD_TITLE_SPACING 4.0f
//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;


//----------------------------------------------------------------------------------
// Credits Screen Functions Definition
//----------------------------------------------------------------------------------

// Credits Screen Initialization logic
void InitCreditsScreen(void)
{
    // TODO: Initialize Credits screen variables here!
    framesCounter = 0;
    finishScreen = 0;

}

// Credits Screen Update logic
void UpdateCreditsScreen(void)
{
    // TODO: Update Credits screen variables here!

    // Press enter or tap to change to ENDING screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        finishScreen = 1;
        PlaySound(fxCoin);
    }

}

static void DrawTextExCentered(Font font, const char* text, float fontSize, float spacing, Color tint)
{
    Vector2 position;
    position.x = GetScreenWidth() / 2 - MeasureTextEx(font, text, fontSize, spacing).x / 2;
    position.y = GetScreenHeight() / 2 - MeasureTextEx(font, text, fontSize, spacing).y / 2;
    DrawTextEx(font, text, position, fontSize, spacing, tint);
}

// Credits Screen Draw logic
void DrawCreditsScreen(void)
{
    // TODO: Draw Credits screen here!
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), PURPLE);
    Vector2 pos = { 20, 10 };
    DrawTextEx(font, "Credits SCREEN", pos, TITLE_FONT_SIZE, STANDARD_TITLE_SPACING, MAROON);
    DrawTextExCentered(font, "Coded by: Francisco Jose Palacios Marquez", TITLE_FONT_SIZE, STANDARD_TITLE_SPACING, MAROON);

}



// Credits Screen Unload logic
void UnloadCreditsScreen(void)
{
    // TODO: Unload Credits screen variables here!
}

// Credits Screen should finish?
int FinishCreditsScreen(void)
{
    return finishScreen;
}