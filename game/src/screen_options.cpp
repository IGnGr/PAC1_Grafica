/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Options Screen Functions Definitions (Init, Update, Draw, Unload)
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
static int selectedVolume = 0;


//----------------------------------------------------------------------------------
// Options Screen Functions Definition
//----------------------------------------------------------------------------------

// Options Screen Initialization logic
void InitOptionsScreen(void)
{
    // TODO: Initialize Options screen variables here!
    framesCounter = 0;
    finishScreen = 0;


    selectedVolume = volumeLevel * 10;
}

// Options Screen Update logic
void UpdateOptionsScreen(void)
{
    // TODO: Update Options screen variables here!

    // Press enter or tap to change to ENDING screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        finishScreen = 1;
        PlaySound(fxCoin);
    }

    if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))
    {
        if(selectedVolume > 0) selectedVolume--;
    }

    if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT))
    {
        if (selectedVolume < 10) selectedVolume++;
    }


    volumeLevel = selectedVolume / 10.0;
    SetMusicVolume(music, volumeLevel);
    SetSoundVolume(fxCoin, volumeLevel);

}

static void DrawTextExCentered(Font font, const char* text, float fontSize, float spacing, Color tint, int offsetY)
{
    Vector2 position;
    position.x = GetScreenWidth() / 2 - MeasureTextEx(font, text, fontSize, spacing).x / 2;
    position.y = GetScreenHeight() / 2 - MeasureTextEx(font, text, fontSize, spacing).y / 2 + offsetY;
    DrawTextEx(font, text, position, fontSize, spacing, tint);
}

// Options Screen Draw logic
void DrawOptionsScreen(void)
{

    DrawTexturePro(backgroundImage, { 0.0f, 0.0f, (float)backgroundImage.width, (float)backgroundImage.height }, FULL_SCREEN_RECTANGLE, { 0, 0 }, 0, WHITE);

    DrawRectangle( 3*GetScreenWidth() / 8, GetScreenHeight() / 3, GetScreenWidth() / 4, GetScreenHeight() / 20, BLACK);
    DrawRectangle( 3*GetScreenWidth() / 8, GetScreenHeight() / 3, GetScreenWidth()/40 * selectedVolume, GetScreenHeight() / 20, DARKGREEN);
    DrawTextExCentered(font, "Volume level", TITLE_FONT_SIZE, STANDARD_TITLE_SPACING, DARKGREEN, - GetScreenHeight() / 20);
    DrawTextExCentered(font, "Press enter to go back to the main menu", TITLE_FONT_SIZE, STANDARD_TITLE_SPACING, DARKGREEN, 40);

}

// Options Screen Unload logic
void UnloadOptionsScreen(void)
{
    // TODO: Unload Options screen variables here!
}

// Options Screen should finish?
int FinishOptionsScreen(void)
{
    return finishScreen;
}