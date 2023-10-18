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
#include <string>
#include <algorithm>
#include <cmath>

#define SHOT_SQUARE_SIZE 10
#define SHOT_SPEED 10
#define PLAYER_SPRITE_OFFSET -90
#define SHOT_FRAMES_LIFESPAN 180
#define PLAYER_SHOT_COOLDOWN_FRAMES 60
#define PLAYER_INVENCIBILITY_FRAMES 60
#define PLAYER_SPRITE_ALPHA 1
#define PLAYER_SPRITE_ALPHA_DELTA 0.05
#define ASTEROID_SPEED 5
#define POWERUP_FRAMES_LIFESPAN 180
#define GENERATION_RATE_POWERUP 500
#define NUMBER_OF_SHOTS_POWERUP 3
#define MULTIPLE_SHOT_DEVIATION_DEGREES 15
#define PLAYER_POWERUP_LIFESPAN 500

struct Player {
    Texture2D sprite;
    float spriteAlpha;
    Vector2 position;
    Vector2 currentInput; 
    Vector2 spriteCenter; 
    float rotationDegrees;  
    float rotationAlpha;
    float currentSpeed;
    float speedAlpha;
    float decelerationRate;
    bool isShootInCooldown;
    int lastShootFrameNumber;
    Rectangle bounds;
    int lives;
    int score;
    bool isShootingKeyActive;
    int lastDamageFrameCounter;
    bool isInvulnerable;
    bool hasPowerUp;
    int powerUpFramesLeft;
};

struct Asteroid {
    Texture2D sprite;
    Vector2 position;
    Vector2 spriteCenter;
    float rotationDegrees;  
    float currentSpeed;
    int size;
    Rectangle bounds;
    bool isActive;
};

struct PlayerShot {
    Vector2 position;
    Rectangle bounds;
    float currentSpeed;
    float rotationDegrees;
    int frameslifespan;
    bool isActive;
};

struct PlayerPowerUp {
    Texture2D sprite;
    Vector2 spriteCenter;
    Vector2 position;
    Rectangle bounds;
    int frameslifespan;
    bool isActive;
};
//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

std::vector<Asteroid> asteroidsInStage;
std::vector<Asteroid> pendingAsteroidsToAdd;
std::vector<PlayerShot> shotsInStage;

double timeAtInit;
double elapsedTimeFromLastInit;
bool isTherePowerUpPresent; 

//NPatchInfo nPatchBackground;
Player player;
PlayerPowerUp powerUp;

Texture2D playerSprite;
Texture2D asteroidSpriteSize1;
Texture2D asteroidSpriteSize2;
Texture2D asteroidSpriteSize3; 
Texture2D lifeActiveSprite;
Texture2D lifeInctiveSprite;
Texture2D powerUpSprite;
Music gameplayMusic;
Sound shotSound;
Sound explosionSound;
Sound accelerationSound;
Sound playerDamagedSound;
Sound pickUpSound;

int highScorePoints;
int highScoreTime;

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------
Vector2 getSpriteCenter(const Texture2D &sprite)
{
    return { (float)sprite.width / 2, (float)sprite.height / 2 };
}

Vector2 generateRandomPositionInScreen(void)
{
    Vector2 aux = { 0,0 };

    aux.x = rand() % GetScreenWidth() + 1 - GetScreenWidth() / 100;
    aux.y = rand() % GetScreenHeight() + 1 - GetScreenHeight() / 100; 

    return aux;
}

Vector2 generateRandomPositionInScreenEdge(void)
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

void generatePowerUp()
{
    powerUp.position = generateRandomPositionInScreen();
    powerUp.bounds = { (float)powerUp.position.x,(float)powerUp.position.y,(float)powerUp.sprite.width,(float)powerUp.sprite.height };

    powerUp.frameslifespan = POWERUP_FRAMES_LIFESPAN;
    powerUp.isActive = true;
    
}

void generateRandomAsteroid(int instances, Vector2 position, int size)
{
    for (int i = 0; i < instances; i++)
    {
        Asteroid auxAsteroid;

        if (size == 3) auxAsteroid.sprite = asteroidSpriteSize3;
        if (size == 2) auxAsteroid.sprite = asteroidSpriteSize2;
        if (size == 1) auxAsteroid.sprite = asteroidSpriteSize1;
        auxAsteroid.position = position;
        auxAsteroid.spriteCenter = getSpriteCenter(auxAsteroid.sprite);
        auxAsteroid.rotationDegrees = generateRandomRotationDegrees();
        auxAsteroid.currentSpeed = ASTEROID_SPEED;
        auxAsteroid.size = size;
        auxAsteroid.bounds = { (float) auxAsteroid.position.x,(float)auxAsteroid.position.y,(float)auxAsteroid.sprite.width,(float)auxAsteroid.sprite.height };
        auxAsteroid.isActive = true;


        pendingAsteroidsToAdd.push_back(auxAsteroid);
    }
}

void generateRandomAsteroid(int instances)
{
    generateRandomAsteroid(instances, generateRandomPositionInScreenEdge(), 3);
}

void LoadResources (void)
{
    playerSprite        = LoadTexture("resources/textures/SpaceShip.png");
    asteroidSpriteSize1 = LoadTexture("resources/textures/SmallMeteor.png");
    asteroidSpriteSize2 = LoadTexture("resources/textures/MendiumMeteor.png");
    asteroidSpriteSize3 = LoadTexture("resources/textures/BigMeteor.png");
    lifeActiveSprite    = LoadTexture("resources/textures/Life_Active.png");
    lifeInctiveSprite   = LoadTexture("resources/textures/Life_Inactive.png");
    powerUpSprite       = LoadTexture("resources/textures/Bonus.png");
    shotSound           = LoadSound("resources/Sounds/shot.wav");
    gameplayMusic       = LoadMusicStream("resources/Music/gameplayMusic.ogg");
    explosionSound      = LoadSound("resources/Sounds/explosion.wav");
    playerDamagedSound  = LoadSound("resources/Sounds/explosion_small.wav");
    accelerationSound   = LoadSound("resources/Sounds/jump.wav");
    pickUpSound         = LoadSound("resources/Sounds/CollectBonus.wav");

}

void InitializePlayer(void)
{
    player.sprite = playerSprite;
    player.position = { (float)GetScreenWidth() / 2 ,(float)GetScreenHeight() / 2 };
    player.spriteCenter = getSpriteCenter(player.sprite);
    player.rotationDegrees = 0;
    player.rotationAlpha = 10.0f;
    player.speedAlpha = 5.0f;
    player.decelerationRate = 0.1f;
    player.currentInput = { 0,0 };
    player.bounds = { (float)player.position.x,(float)player.position.y,(float)player.sprite.width,(float)player.sprite.height };
    player.lives = 3;
    player.score = 0;
    player.isShootInCooldown = false;
    player.lastShootFrameNumber = 0;
    player.lastDamageFrameCounter = 0;
    player.spriteAlpha = PLAYER_SPRITE_ALPHA;
    player.isInvulnerable = false;
    player.hasPowerUp = false;
    player.powerUpFramesLeft = 0;
}

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    // TODO: Initialize GAMEPLAY screen variables here!


    timeAtInit = GetTime();

    srand(time(NULL));

    framesCounter = 0;
    finishScreen = 0;
    LoadResources();

    StopMusicStream(music);
    SetMusicVolume(gameplayMusic, volumeLevel);
    PlayMusicStream(gameplayMusic);
    SetSoundVolume(shotSound, volumeLevel);
    SetSoundVolume(explosionSound, volumeLevel);
    SetSoundVolume(playerDamagedSound, volumeLevel);
    SetSoundVolume(accelerationSound, volumeLevel);
    SetSoundVolume(pickUpSound, volumeLevel);

    InitializePlayer();

    //Initializing asteroids
    generateRandomAsteroid(rand() % 2 + 1);


    //Loading high scores
    highScorePoints = LoadStorageValue(1);
    highScoreTime = LoadStorageValue(0);

    powerUp.sprite = powerUpSprite;
    powerUp.isActive = false;
    powerUp.bounds = { (float)powerUp.position.x,(float)powerUp.position.y,(float)powerUp.sprite.width,(float)powerUp.sprite.height };
    powerUp.spriteCenter = getSpriteCenter(powerUp.sprite);

}

float degreeToRadians(float degrees)
{
    return degrees * PI / 180;
}


void MoveObjectForwards(Vector2 *pos, float rotationDegrees, float speed, float rotationDegreesOffset, Rectangle *bounds)
{
    pos->x += speed * cos(degreeToRadians(rotationDegrees + rotationDegreesOffset));
    pos->y += speed * sin(degreeToRadians(rotationDegrees + rotationDegreesOffset));

    //Moving hitbox
    bounds->x = pos->x;
    bounds->y = pos->y; 

    //Handling moves outside the screen so the object appears through the other side
    if (pos->x < 0) pos->x += GetScreenWidth();
    if (pos->y < 0) pos->y += GetScreenHeight();
    if (pos->x > GetScreenWidth()) pos->x -= GetScreenWidth();
    if (pos->y > GetScreenHeight()) pos->y -= GetScreenHeight();

}

void handlePlayerInputs(void)
{
    //We store the inputs as a 2D array
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

    if (IsKeyDown(KEY_SPACE))
    {
        player.isShootingKeyActive = true;
    }
    else
    {
        player.isShootingKeyActive = false;
    }

    //Quit
    if (IsKeyPressed(KEY_Z))
    {
        finishScreen = 1;
    }
}

void handleCollisionsAsteroidPlayer(Asteroid &asteroid, Player &player)
{

    if (CheckCollisionRecs(asteroid.bounds, player.bounds))
    {

        if (framesCounter - player.lastDamageFrameCounter >= PLAYER_INVENCIBILITY_FRAMES)
        {
            if (player.lives == 1)
            {
                finishScreen = 1;
                if (highScorePoints < player.score) SaveStorageValue(1, player.score);
                if (highScoreTime < elapsedTimeFromLastInit) SaveStorageValue(0, elapsedTimeFromLastInit);
            }

            player.lives--;
            player.lastDamageFrameCounter = framesCounter;
            PlaySound(playerDamagedSound);
            player.isInvulnerable = true;

        }

    }
}

void handleCollisionsPowerUpPlayer(PlayerPowerUp& powerUp, Player& player)
{

    if (powerUp.isActive && CheckCollisionRecs(powerUp.bounds, player.bounds))
    {

        player.hasPowerUp = true;
        powerUp.isActive = false;
        player.powerUpFramesLeft = PLAYER_POWERUP_LIFESPAN;
        PlaySound(pickUpSound);
    }
}

void handleCollisionsAsteroidShot(Asteroid& asteroid, PlayerShot& shot)
{

    if (CheckCollisionRecs(asteroid.bounds, shot.bounds) && asteroid.isActive && shot.isActive)
    {
        if (asteroid.size == 3)
        {
            asteroid.size = 2;
            asteroid.sprite = asteroidSpriteSize2;
            player.score += 10;


            generateRandomAsteroid(3, asteroid.position, 2);
        }
        else {
            if (asteroid.size == 2)
            {
                asteroid.size = 1;
                player.score += 20;
                generateRandomAsteroid(3, asteroid.position, 1);

            }
            else
            {
                if (asteroid.size == 1)
                {
                    player.score += 30;
                }
            }
        }
        
        asteroid.isActive = false;
        shot.isActive = false;
        PlaySound(explosionSound);

    }
}

void handleShotsCollisions()
{
    for (auto& shot : shotsInStage)
    {
        for (auto& asteroid : asteroidsInStage)
        {
            handleCollisionsAsteroidShot(asteroid, shot);
        }
    }
}

void handlePlayerCollisions()
{
    for (auto& asteroid : asteroidsInStage)
    {
        handleCollisionsAsteroidPlayer(asteroid, player);
    }

    handleCollisionsPowerUpPlayer(powerUp,player);
}

void generatePlayerShot(static Player& player, bool isPowerUpActive)
{
    PlayerShot newShot;
    newShot.position = player.position;
    newShot.rotationDegrees = player.rotationDegrees;
    Rectangle shotRectangle = { (float)newShot.position.x,(float)newShot.position.y, (float)SHOT_SQUARE_SIZE,(float)SHOT_SQUARE_SIZE };
    newShot.bounds = shotRectangle;
    newShot.currentSpeed = SHOT_SPEED;

    newShot.frameslifespan = SHOT_FRAMES_LIFESPAN;
    newShot.isActive = true;
    shotsInStage.push_back(newShot);


    if (isPowerUpActive)
    {
        for (int i = - MULTIPLE_SHOT_DEVIATION_DEGREES; i < (NUMBER_OF_SHOTS_POWERUP - 1) * MULTIPLE_SHOT_DEVIATION_DEGREES; i+= MULTIPLE_SHOT_DEVIATION_DEGREES * 2)
        {
            PlayerShot newShot;
            newShot.position = player.position;
            newShot.rotationDegrees = player.rotationDegrees + i;
            Rectangle shotRectangle = { (float)newShot.position.x,(float)newShot.position.y, (float)SHOT_SQUARE_SIZE,(float)SHOT_SQUARE_SIZE };
            newShot.bounds = shotRectangle;
            newShot.currentSpeed = SHOT_SPEED;

            newShot.frameslifespan = SHOT_FRAMES_LIFESPAN;
            newShot.isActive = true;
            shotsInStage.push_back(newShot);
        }

    }

}



void handlePlayerMovement(void)
{

    //Rotation
    player.rotationDegrees += player.currentInput.y * player.rotationAlpha;
    //Translation
    MoveObjectForwards(&player.position, player.rotationDegrees, player.speedAlpha * player.currentInput.x, PLAYER_SPRITE_OFFSET, &player.bounds);


    //Gradual deceleration when no movement input is applied
    if (player.currentInput.x == 0)
    {
        if (player.currentSpeed > 0) player.currentSpeed -= player.decelerationRate;
        if (player.currentSpeed < 0) player.currentSpeed += player.decelerationRate;

        MoveObjectForwards(&player.position, player.rotationDegrees, player.currentSpeed, PLAYER_SPRITE_OFFSET, &player.bounds);
    }
    else
    {

        if (!IsSoundPlaying(accelerationSound))
        {
            SetSoundPitch(accelerationSound,10);
            PlaySound(accelerationSound);
        }
    }

    if (framesCounter - player.lastDamageFrameCounter >= PLAYER_INVENCIBILITY_FRAMES && player.lastDamageFrameCounter != 0)
    {
        player.isInvulnerable = false;
        player.spriteAlpha = 1;
    }

    if (player.isInvulnerable)
    {
        player.spriteAlpha -= PLAYER_SPRITE_ALPHA_DELTA;
        if (player.spriteAlpha <= 0) player.spriteAlpha = 1;
    }


    //We allow the player to shoot again if the cooldown time has passed
    if (framesCounter - player.lastShootFrameNumber >= PLAYER_SHOT_COOLDOWN_FRAMES || player.lastShootFrameNumber == 0)
    {
        player.isShootInCooldown = false;
    }

    //Player shot logic
    if (player.isShootingKeyActive && !player.isShootInCooldown)
    {
        generatePlayerShot(player,player.hasPowerUp);
        player.isShootInCooldown = true;
        PlaySound(shotSound);
        player.lastShootFrameNumber = framesCounter;
    }

    if (player.hasPowerUp)
    {
        if(player.powerUpFramesLeft > 0 ) player.powerUpFramesLeft--;
        else
        {
            player.hasPowerUp = false;
        }
    }

}

void handleAsteroidsMovement(void)
{
    for (auto& asteroid : asteroidsInStage)
    {
        MoveObjectForwards(&asteroid.position, asteroid.rotationDegrees, asteroid.currentSpeed, 0, &asteroid.bounds);
    }
}



void handleShotsMovement(void)
{

    for (auto& shot : shotsInStage)
    {
        shot.frameslifespan--;
        MoveObjectForwards(&shot.position, shot.rotationDegrees, shot.currentSpeed, PLAYER_SPRITE_OFFSET, &shot.bounds);
    }

    shotsInStage.erase(std::remove_if(shotsInStage.begin(), shotsInStage.end(),
        [](PlayerShot shot) { return shot.frameslifespan <= 0; }), shotsInStage.end());
}

void handleNumberOfAsteroids(void)
{
    if (asteroidsInStage.size() == 0)
    {
        generateRandomAsteroid(rand() % 4 + 1);
    }

    for (auto &ast: pendingAsteroidsToAdd)
    {
        asteroidsInStage.push_back(ast);
    }

    pendingAsteroidsToAdd.clear();
}

void checkElementsToRemove(void)
{
    /*asteroidsInStage.erase(std::find_if(asteroidsInStage.begin(), asteroidsInStage.end(),
        [](Asteroid ast) { return !ast.isActive }));

    shotsInStage.erase(std::find_if(shotsInStage.begin(), shotsInStage.end(),
        [](const auto& sh) { return &sh == &shot; }));
    */

    asteroidsInStage.erase(std::remove_if(asteroidsInStage.begin(), asteroidsInStage.end(), [](const Asteroid& ast) { return !ast.isActive; }), asteroidsInStage.end());
    shotsInStage.erase(std::remove_if(shotsInStage.begin(), shotsInStage.end(), [](const PlayerShot& sh) { return !sh.isActive; }), shotsInStage.end());

}

void handlePowerUp(void)
{
    //Chance of 50% after GENERATION_RATE_POWERUP number of frames
    if (!powerUp.isActive && (rand() % 2) + 1 == 2 && framesCounter % GENERATION_RATE_POWERUP == 0)
    {
        generatePowerUp();
    }

    if (powerUp.isActive)
    {
        if(powerUp.frameslifespan > 0) powerUp.frameslifespan--;
        else 
        {
            powerUp.isActive = false;
        }
    }
        
}
// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{

    handlePlayerInputs();
    handlePlayerMovement();
    handleShotsMovement();
    handlePowerUp();
    handleAsteroidsMovement();
    handleShotsCollisions();
    handlePlayerCollisions();
    handleNumberOfAsteroids();
    checkElementsToRemove();
    UpdateMusicStream(gameplayMusic);
     elapsedTimeFromLastInit = GetTime() - timeAtInit;



    
}


void DrawBackground(void)
{
    //DrawTextureNPatch(backgroundImage, nPatchBackground, FULL_SCREEN_RECTANGLE, (Vector2) { 0, 0 }, 0, WHITE);
    DrawTexturePro(backgroundImage, { 0.0f, 0.0f, (float) backgroundImage.width, (float)backgroundImage.height }, FULL_SCREEN_RECTANGLE, { 0, 0 }, 0, WHITE);

}

void DrawPlayer(void)
{
    DrawTexturePro(player.sprite, { 0.0f, 0.0f, (float)player.sprite.width, (float)player.sprite.height }, { player.position.x, player.position.y, (float)player.sprite.width, (float)player.sprite.height }, player.spriteCenter, player.rotationDegrees, Fade(WHITE, player.spriteAlpha));
}

void DrawPowerUp(void)
{
    if (powerUp.isActive)
    {
        DrawTexturePro(powerUp.sprite, { 0.0f, 0.0f, (float)powerUp.sprite.width, (float)powerUp.sprite.height }, { powerUp.position.x, powerUp.position.y, (float)powerUp.sprite.width, (float)powerUp.sprite.height }, powerUp.spriteCenter, 0, WHITE);
    }

}


void DrawAsteroids(void)
{
    for (auto& asteroid : asteroidsInStage)
    {
        DrawTexturePro(asteroid.sprite, { 0.0f, 0.0f, (float)asteroid.sprite.width, (float)asteroid.sprite.height }, { asteroid.position.x, asteroid.position.y, (float)asteroid.sprite.width, (float)asteroid.sprite.height }, asteroid.spriteCenter, asteroid.rotationDegrees, WHITE);

        //Hitbox debug
        //DrawRectanglePro( asteroid.bounds, asteroid.spriteCenter,asteroid.rotationDegrees, BLUE);
    }

}

void DrawShots(void)
{
    for (auto& shot : shotsInStage)
    {
        DrawRectanglePro(shot.bounds,{0,0}, shot.rotationDegrees, RED);
    }

}

void DrawHUD(void)
{
    //lives
    for (int i = 0; i < 3; i++)
    {
        Texture2D spriteToDraw = player.lives <= i ? lifeInctiveSprite : lifeActiveSprite;
        DrawTexturePro(spriteToDraw, { 0.0f, 0.0f, (float)spriteToDraw.width, (float)spriteToDraw.height }, { (float)GetScreenWidth() / 100 + i*50, (float)GetScreenHeight() / 100, (float)spriteToDraw.width, (float)spriteToDraw.height }, { 0, 0 }, 0, WHITE);
    }
    //Score
    DrawTextEx(font, TextFormat("Score: %d",player.score) , {(float)GetScreenWidth() / 8 , (float)GetScreenHeight() / 100}, TITLE_FONT_SIZE, STANDARD_TITLE_SPACING, WHITE);

    //Timer
    DrawTextEx(font, TextFormat("Time: %02d:%02d", (int)elapsedTimeFromLastInit / 60,  (int) elapsedTimeFromLastInit%60), { (float)GetScreenWidth() / 8  , (float)GetScreenHeight() / 100 + 25 }, TITLE_FONT_SIZE, STANDARD_TITLE_SPACING, WHITE);
    


}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    // TODO: Draw GAMEPLAY screen here!
    DrawBackground();
    DrawHUD();
    DrawPlayer();
    DrawAsteroids();
    DrawShots();
    DrawPowerUp();
    framesCounter++;


}




// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!

    UnloadTexture(playerSprite);
    UnloadTexture(asteroidSpriteSize1);
    UnloadTexture(asteroidSpriteSize2);
    UnloadTexture(asteroidSpriteSize3);
    UnloadTexture(lifeActiveSprite);
    UnloadTexture(lifeInctiveSprite);
    UnloadTexture(powerUpSprite);
    UnloadSound(shotSound);
    UnloadMusicStream(gameplayMusic);
    UnloadSound(explosionSound);
    UnloadSound(playerDamagedSound);
    UnloadSound(accelerationSound);

    asteroidsInStage.clear();
    shotsInStage.clear();

}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}