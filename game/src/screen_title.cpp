/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Title Screen Functions Definitions (Init, Update, Draw, Unload)
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

#define MAX_OPTIONS 3
#define PRESS_ENTER_TEXT "PRESS ENTER" 
#define OPTIONS_TEXT "OPTIONS" 
#define CREDITS_TEXT "CREDITS" 
#define PLAY_TEXT "PLAY"


//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
static int currentLogoPositionX = 0;
static int currentLogoPositionY = 0;
static int finalLogoPositionX = 0;
static int finalLogoPositionY = 0;
static int pressEnterPositionX = 0;
static int pressEnterPositionY = 0;
static int pressEnterOffsetFromTitle = 350;
static int offsetBetweenMenuOptions = 100;
static float alpha = 1.0f;         // Useful for fading
static int cursorIndex = 0;
static bool hasPressedEntered = false;

GameScreen menuOptions[MAX_OPTIONS] = { GAMEPLAY,OPTIONS,CREDITS };
Texture2D titleImage = { 0 };
Texture2D cursorImage = { 0 };

Sound cursorSound = { 0 };
//----------------------------------------------------------------------------------
// Title Screen Functions Definition
//----------------------------------------------------------------------------------

// Title Screen Initialization logic
void InitTitleScreen(void)
{
    // TODO: Initialize TITLE screen variables here!
    framesCounter = 0;
    finishScreen = 0;
    alpha = 1.0f;
    //hasPressedEntered = false;

    titleImage = LoadTexture("resources/textures/pixil-frame-0.png");
    cursorImage = LoadTexture("resources/textures/SpaceShip.png");
    cursorSound = LoadSound("resources/Sounds/shot.wav");

    SetSoundVolume(cursorSound, volumeLevel);

    if(!IsMusicStreamPlaying(music)) PlayMusicStream(music);

    
    if (!hasPressedEntered)
    {
        finalLogoPositionX = GetScreenWidth() / 2 - titleImage.width * 2.5 / 2;
        finalLogoPositionY = GetScreenHeight() / 2 - titleImage.height * 4;

        currentLogoPositionX = finalLogoPositionX;
        currentLogoPositionY = -200; //Offset so it's outside the screen

        pressEnterPositionX = GetScreenWidth() / 2 - MeasureTextEx(font, PRESS_ENTER_TEXT, TITLE_FONT_SIZE, STANDARD_TITLE_SPACING).x / 2;
        pressEnterPositionY = finalLogoPositionY + pressEnterOffsetFromTitle;
    }
}

// Title Screen Update logic
void UpdateTitleScreen(void)
{

    // Press enter or tap to change to GAMEPLAY screen
    if (hasPressedEntered && (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)))
    {
        // Load next screen
        finishScreen = menuOptions[cursorIndex];
        PlaySound(cursorSound);
    }

    // TODO: Update TITLE screen variables here!
    if (!hasPressedEntered && (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)))
    {
        hasPressedEntered = true;
        PlaySound(cursorSound);

    }


    if (hasPressedEntered && (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)))
    {

        cursorIndex += 1;
        if (cursorIndex >= MAX_OPTIONS) cursorIndex = 0;
        PlaySound(fxCoin);
    }

    if (hasPressedEntered && (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)))
    {

        cursorIndex -= 1;
        if (cursorIndex < 0) cursorIndex = MAX_OPTIONS - 1;
        PlaySound(fxCoin);
    }


    framesCounter++;

    alpha -= 0.02f;

    if (alpha <= 0.0f)
    {
        alpha = 1.0f;
    }


    if(currentLogoPositionY < finalLogoPositionY) currentLogoPositionY++;
}


// Title Screen Draw logic
void DrawTitleScreen(void)
{
    // TODO: Draw TITLE screen here!
    DrawTexturePro(backgroundImage, { 0.0f, 0.0f, (float)backgroundImage.width, (float)backgroundImage.height }, FULL_SCREEN_RECTANGLE, { 0, 0 }, 0, WHITE);

    if (!hasPressedEntered)
    {
        DrawTextEx(font, PRESS_ENTER_TEXT, { (float) pressEnterPositionX,(float) pressEnterPositionY }, TITLE_FONT_SIZE, STANDARD_TITLE_SPACING, Fade(DARKGREEN, alpha));
    }

    if (hasPressedEntered)
    {
        DrawTextEx(font, PLAY_TEXT,  { (float) GetScreenWidth() / 2 - MeasureTextEx(font, PLAY_TEXT, TITLE_FONT_SIZE, STANDARD_TITLE_SPACING).x / 2,  (float) pressEnterPositionY }, TITLE_FONT_SIZE, STANDARD_TITLE_SPACING, DARKGREEN);
        DrawTextEx(font, OPTIONS_TEXT,  { GetScreenWidth() / 2 - MeasureTextEx(font, OPTIONS_TEXT, TITLE_FONT_SIZE, STANDARD_TITLE_SPACING).x / 2,  (float) pressEnterPositionY + offsetBetweenMenuOptions }, TITLE_FONT_SIZE, STANDARD_TITLE_SPACING, DARKGREEN);
        DrawTextEx(font, CREDITS_TEXT,  { GetScreenWidth() / 2 - MeasureTextEx(font, CREDITS_TEXT, TITLE_FONT_SIZE, STANDARD_TITLE_SPACING).x / 2,  (float) pressEnterPositionY + offsetBetweenMenuOptions * 2 }, TITLE_FONT_SIZE, STANDARD_TITLE_SPACING, DARKGREEN);
        DrawTextureEx(cursorImage,  { (float)pressEnterPositionX - cursorImage.width * 2,  (float)pressEnterPositionY + cursorIndex * offsetBetweenMenuOptions }, 0, 1, WHITE);
    }

    //DrawText("+", pressEnterPositionX - 25, pressEnterPositionY + cursorIndex * 50, font.baseSize, MAROON);


    DrawTextureEx(titleImage, { (float) finalLogoPositionX,  (float)currentLogoPositionY }, 0, 3, WHITE);


}

// Title Screen Unload logic
void UnloadTitleScreen(void)
{
    // TODO: Unload TITLE screen variables here!
    //UnloadTexture(titleImage);
    //UnloadTexture(cursorImage);
    //UnloadSound(cursorSound);
}

// Title Screen should finish?
int FinishTitleScreen(void)
{
    return finishScreen;
}