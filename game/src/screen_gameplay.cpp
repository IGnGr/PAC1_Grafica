/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Gameplay Screen Functions Definitions (Init, Update, Draw, Unload)
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
#include <math.h>
#include <vector>
#include <time.h>
#include <stdlib.h>

#define FULL_SCREEN_RECTANGLE {0.0f,0.0f,(float) GetScreenWidth(),(float)GetScreenHeight()}

struct Player {
    Texture2D sprite;
    Vector2 position;
    Vector2 currentInput; 
    Vector2 spriteCenter; 
    float rotationDegrees;  
    float rotationAlpha;
    float currentSpeed;
    float speedAlpha;
    float decelerationRate;
};

struct Asteroid {
    Texture2D sprite;
    Vector2 position;
    Vector2 spriteCenter;
    float rotationDegrees;  
    float movementSpeed; 
    int size;
};

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
std::vector<Asteroid> asteroidsInStage;

float defaultAsteroidSpeed = 5.0f;


Texture2D backgroundImage = { 0 };
NPatchInfo nPatchBackground;
Player player;
Texture2D asteroidSpriteSize1;
Texture2D asteroidSpriteSize2;
Texture2D asteroidSpriteSize3;


//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------
Vector2 getSpriteCenter(const Texture2D &sprite)
{
    return { (float)sprite.width / 2, (float)sprite.height / 2 };
}

Vector2 generateRandomPositionInScreen(void)
{
    //Positions only at the edge to avoid spawns inside the player on startup
    Vector2 aux = { 0,0 };
    if ((rand() % 2) + 1 == 2)
    {
        aux.x = 0;
        aux.y = rand() % GetScreenHeight() + 1;
    }
    else 
    {
        aux.x = rand() % GetScreenWidth() + 1;
        aux.y = 0;  
    }
    return aux ;
}

int generateRandomRotationDegrees(void)
{
    return rand() % 360 + 1;
}
void generateRandomAsteroid(int instances)
{
    for (int i = 0; i < instances; i++)
    {
        Asteroid auxAsteroid;
        auxAsteroid.sprite = asteroidSpriteSize3;
        auxAsteroid.position = generateRandomPositionInScreen();
        auxAsteroid.spriteCenter = getSpriteCenter(auxAsteroid.sprite);
        auxAsteroid.rotationDegrees = generateRandomRotationDegrees();
        auxAsteroid.movementSpeed = defaultAsteroidSpeed;
        auxAsteroid.size = 3;

        asteroidsInStage.push_back(auxAsteroid);
    }
}

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    // TODO: Initialize GAMEPLAY screen variables here!

    srand(time(NULL));

    framesCounter = 0;
    finishScreen = 0;
    backgroundImage = LoadTexture("resources/textures/Landscape.png");

    player.sprite = LoadTexture("resources/textures/SpaceShip.png");
    player.position =  { (float)GetScreenWidth() / 2 ,(float)GetScreenHeight() / 2 };
    player.spriteCenter = getSpriteCenter(player.sprite);
    player.rotationDegrees = 0;
    player.rotationAlpha = 10.0f;
    player.speedAlpha = 5.0f;
    player.decelerationRate = 0.1f;
    player.currentInput = { 0,0 };

    asteroidSpriteSize1 = LoadTexture("resources/textures/SmallMeteor.png");
    asteroidSpriteSize2 = LoadTexture("resources/textures/MendiumMeteor.png");
    asteroidSpriteSize3 = LoadTexture("resources/textures/BigMeteor.png");


    generateRandomAsteroid(rand() % 8 + 1);


    nPatchBackground = { FULL_SCREEN_RECTANGLE, 0, 0, 0, 0, NPATCH_NINE_PATCH };


}

float degreeToRadians(float degrees)
{
    return degrees * PI / 180;
}


void MoveObjectForwards(Vector2 *pos, float rotationDegrees, float speed, float rotationDegreesOffset)
{
    pos->x += speed * cos(degreeToRadians(rotationDegrees + rotationDegreesOffset));
    pos->y += speed * sin(degreeToRadians(rotationDegrees + rotationDegreesOffset));


    //Handling moves outside the screen
    if (pos->x < 0) pos->x += GetScreenWidth();
    if (pos->y < 0) pos->y += GetScreenHeight();
    if (pos->x > GetScreenWidth()) pos->x -= GetScreenWidth();
    if (pos->y > GetScreenHeight()) pos->y -= GetScreenHeight();

}

void handlePlayerInputs(void)
{

    //Rotation
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
    {
        player.currentInput.y = -1;
    }
    else 
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
        {
            player.currentInput.y = 1;
        }
        else 
        {
            player.currentInput.y = 0;
        }


    //Translation
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
    {
        player.currentInput.x = 1;
        player.currentSpeed = player.speedAlpha;

    }
    else
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
        {
            player.currentInput.x = -1;
            player.currentSpeed = -player.speedAlpha;
        }
        else
        {
            player.currentInput.x = 0;
        }

    //Quit
    if (IsKeyPressed(KEY_ESCAPE))
    {
        finishScreen = 1;
    }
}



void handlePlayerMovement(void)
{

    player.rotationDegrees += player.currentInput.y * player.rotationAlpha;
    MoveObjectForwards(&player.position, player.rotationDegrees, player.speedAlpha * player.currentInput.x, -90);


    //Gradual deceleration when no movement input is applied
    if (player.currentInput.x == 0)
    {
        if (player.currentSpeed > 0) player.currentSpeed -= player.decelerationRate;
        if (player.currentSpeed < 0) player.currentSpeed += player.decelerationRate;

        MoveObjectForwards(&player.position, player.rotationDegrees, player.currentSpeed, -90);
    }

}

void handleAsteroidsMovement(void)
{
    for (auto& asteroid : asteroidsInStage)
    {
        MoveObjectForwards(&asteroid.position, asteroid.rotationDegrees, asteroid.movementSpeed, 0);
    }
}


// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    // TODO: Update GAMEPLAY screen variables here!
    handlePlayerInputs();
    handlePlayerMovement();
    handleAsteroidsMovement();

}


void SetBackground(void)
{
    //DrawTextureNPatch(backgroundImage, nPatchBackground, FULL_SCREEN_RECTANGLE, (Vector2) { 0, 0 }, 0, WHITE);
    DrawTexturePro(backgroundImage, { 0.0f, 0.0f, (float) backgroundImage.width, (float)backgroundImage.height }, FULL_SCREEN_RECTANGLE, { 0, 0 }, 0, WHITE);

}

void DrawPlayer(void)
{
    DrawTexturePro(player.sprite, { 0.0f, 0.0f, (float)player.sprite.width, (float)player.sprite.height }, { player.position.x, player.position.y, (float)player.sprite.width, (float)player.sprite.height }, player.spriteCenter, player.rotationDegrees, WHITE);
}

void DrawAsteroids(void)
{
    for (auto &asteroid : asteroidsInStage)
    {
        DrawTexturePro(asteroid.sprite, { 0.0f, 0.0f, (float)asteroid.sprite.width, (float)asteroid.sprite.height }, { asteroid.position.x, asteroid.position.y, (float)asteroid.sprite.width, (float)asteroid.sprite.height }, asteroid.spriteCenter, asteroid.rotationDegrees, WHITE);
    }

}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    // TODO: Draw GAMEPLAY screen here!
    SetBackground();
    DrawPlayer();
    DrawAsteroids();



}




// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!

    UnloadTexture(backgroundImage);
    UnloadTexture(player.sprite);

    
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}