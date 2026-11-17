#include "CS3113/Scene.h"
#include "CS3113/LevelOne.h"
#include "CS3113/LevelTwo.h"
#include "CS3113/LevelThree.h"
#include "CS3113/StartScreen.h"
#include "CS3113/LoseScreen.h"
#include "CS3113/WinScreen.h"


Camera2D gCamera = {};

Scene *gCurrentScene = nullptr;
SceneID gCurrentSceneID = NO_SCENE;

std::map<SceneID, Scene *> gScenes = {};
Texture2D gHeartTexture = {};
Music gRetroMusic = {};
Sound gDeathSound = {};

// Global Constants
constexpr int SCREEN_WIDTH  = 1000,
              SCREEN_HEIGHT = 600,
              FPS           = 120;

constexpr char    BG_COLOUR[]      = "#011627";
constexpr Vector2 ORIGIN           = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 },
                  ATLAS_DIMENSIONS = { 6, 8 };

constexpr float TILE_DIMENSION          = 75.0f,
                // in m/ms², since delta time is in ms
                ACCELERATION_OF_GRAVITY = 981.0f,
                FIXED_TIMESTEP          = 1.0f / 60.0f,
                END_GAME_THRESHOLD      = 800.0f;

// Global Variables
AppStatus gAppStatus       = RUNNING;
float     gPreviousTicks   = 0.0f,
          gTimeAccumulator = 0.0f;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();
void panCamera(Camera2D *camera, const Vector2 *targetPosition);

bool entitiesTouch(const Entity *first, const Entity *second)
{
    Vector2 firstPosition = first->getPosition();
    Vector2 secondPosition = second->getPosition();
    Vector2 firstSize = first->getColliderDimensions();
    Vector2 secondSize = second->getColliderDimensions();

    return fabs(firstPosition.x - secondPosition.x) <
               (firstSize.x + secondSize.x) / 2.0f &&
           fabs(firstPosition.y - secondPosition.y) <
               (firstSize.y + secondSize.y) / 2.0f;
}

/**
 * The function `panCamera` smoothly adjusts the camera's target position
 * towards a specified target position.
 *
 * @param camera The `camera` parameter is a pointer to a `Camera2D` struct,
 * which contains information about a 2D camera such as its position, target,
 * zoom level, and other properties.
 * @param targetPosition The `targetPosition` parameter is a pointer to a
 * `Vector2` structure that represents the desired position where the camera
 * should be focused or centered on.
 */
void panCamera(Camera2D *camera, const Vector2 *targetPosition)
{
    Vector2 positionDifference =
        Vector2Subtract(*targetPosition, camera->target);

    camera->target = Vector2Add(
        camera->target, Vector2Scale(positionDifference, 0.1f)
    ); // 0.1 = smoothing factor
}

void switchToScene(SceneID newSceneID)
{
    if(gCurrentScene) gCurrentScene->shutdown();

    gCurrentSceneID = newSceneID;
    gCurrentScene = gScenes[newSceneID];
    gCurrentScene->initialise();    
    if (gCurrentScene->getState().man){
        gCamera.target = gCurrentScene->getState().man->getPosition();

    } else {
        gCamera.target = gCurrentScene->getOrigin();
    }
}


void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Platformer");
    InitAudioDevice();
    gHeartTexture = LoadTexture("assets/game/heart.png");
    gRetroMusic = LoadMusicStream("assets/game/retro.mp3");
    gRetroMusic.looping = true;
    PlayMusicStream(gRetroMusic);
    gDeathSound = LoadSound("assets/game/death.mp3");

    SetMasterVolume(1.0f);

    gScenes[LEVEL_ONE] =
        new LevelOne(ORIGIN, "#123B66");

    gScenes[LEVEL_TWO] =
        new LevelTwo(ORIGIN, "#185A8D");

    gScenes[START_SCREEN] =
        new StartScreen(ORIGIN, "#0B1F3A");

    gScenes[LOSE_SCREEN] =
        new LoseScreen(ORIGIN, "#0A2A43");

    gScenes[WIN_SCREEN] =
        new WinScreen(ORIGIN, "#2D8CBA");

    gScenes[LEVEL_THREE] =
        new LevelThree(ORIGIN, "#2274A5");


    switchToScene(START_SCREEN);

    gCamera.offset   = ORIGIN;
    gCamera.rotation = 0.0f;
    gCamera.zoom     = 1.0f;

    SetTargetFPS(FPS);
}

void renderLives()
{
    const float heartSize = 40.0f;
    const float spacing = 10.0f;

    for (int i = 0; i < getLives(); i++)
    {
        Rectangle sourceArea = {
            0.0f,
            0.0f,
            static_cast<float>(gHeartTexture.width),
            static_cast<float>(gHeartTexture.height)
        };

        Rectangle destinationArea = {
            20.0f + i * (heartSize + spacing),
            20.0f,
            heartSize,
            heartSize
        };

        DrawTexturePro(
            gHeartTexture,
            sourceArea,
            destinationArea,
            { 0.0f, 0.0f },
            0.0f,
            WHITE
        );
    }
}

void processInput()
{
    Entity *man = gCurrentScene->getState().man;

    if (man)
    {
        man->resetMovement();

        if (IsKeyDown(KEY_A))
            man->moveLeft();
        else if (IsKeyDown(KEY_D))
            man->moveRight();

        if (IsKeyPressed(KEY_W) && man->isCollidingBottom())
        {
            man->jump();
            PlaySound(gCurrentScene->getState().jumpSound);
        }

        if (GetLength(man->getMovement()) > 1.0f)
            man->normaliseMovement();
    }

    if (IsKeyPressed(KEY_ONE))
        gCurrentScene->getState().nextSceneID = LEVEL_ONE;
    else if (IsKeyPressed(KEY_TWO))
        gCurrentScene->getState().nextSceneID = LEVEL_TWO;
    else if (IsKeyPressed(KEY_THREE))
        gCurrentScene->getState().nextSceneID = LEVEL_THREE;

    if (IsKeyPressed(KEY_Q) || WindowShouldClose())
        gAppStatus = TERMINATED;
}

void update()
{
    UpdateMusicStream(gRetroMusic);

    // Delta time
    float ticks     = static_cast<float>(GetTime());
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    // Fixed timestep
    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP)
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        gCurrentScene->update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;

        Entity *man = gCurrentScene->getState().man;
        if (man)
        {
            Vector2 currentPlayerPosition = man->getPosition();
            panCamera(&gCamera, &currentPlayerPosition);
        }
    }

    gTimeAccumulator = deltaTime;

}

void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));

    BeginMode2D(gCamera);

    gCurrentScene->render();

    EndMode2D();

    if (gCurrentSceneID != START_SCREEN &&
        gCurrentSceneID != LOSE_SCREEN &&
        gCurrentSceneID != WIN_SCREEN)
    {
        renderLives();
    }

    EndDrawing();
}

void shutdown()
{
    for (std::pair<const SceneID, Scene *> &entry : gScenes)
        delete entry.second;

    gScenes.clear();

    UnloadTexture(gHeartTexture);
    UnloadMusicStream(gRetroMusic);
    UnloadSound(gDeathSound);

    CloseAudioDevice();
    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();

        Entity *man = gCurrentScene->getState().man;
        bool playerTouchedEnemy = false;
        bool playerTouchedReward = false;

        if (man &&
            gCurrentSceneID != START_SCREEN &&
            gCurrentSceneID != LOSE_SCREEN &&
            gCurrentSceneID != WIN_SCREEN)
        {
            playerTouchedReward =
                gCurrentScene->getState().reward &&
                entitiesTouch(man, gCurrentScene->getState().reward);

            playerTouchedEnemy =
                (gCurrentScene->getState().enemy &&
                 entitiesTouch(man, gCurrentScene->getState().enemy)) ||
                (gCurrentScene->getState().secondEnemy &&
                 entitiesTouch(man, gCurrentScene->getState().secondEnemy)) ||
                (gCurrentScene->getState().thirdEnemy &&
                 entitiesTouch(man, gCurrentScene->getState().thirdEnemy)) ||
                (gCurrentScene->getState().fourthEnemy &&
                 entitiesTouch(man, gCurrentScene->getState().fourthEnemy)) ||
                (gCurrentScene->getState().fifthEnemy &&
                 entitiesTouch(man, gCurrentScene->getState().fifthEnemy));
        }

        bool playerFell =
            man &&
            gCurrentSceneID != START_SCREEN &&
            gCurrentSceneID != LOSE_SCREEN &&
            gCurrentSceneID != WIN_SCREEN &&
            man->getPosition().y > END_GAME_THRESHOLD;

        if (playerTouchedReward)
        {
            if (gCurrentSceneID == LEVEL_ONE)
                switchToScene(LEVEL_TWO);
            else if (gCurrentSceneID == LEVEL_TWO)
                switchToScene(LEVEL_THREE);
            else if (gCurrentSceneID == LEVEL_THREE)
                switchToScene(WIN_SCREEN);
        }
        else if (playerFell || playerTouchedEnemy)
        {
            PlaySound(gDeathSound);
            loseLife();

            if (getLives() == 0)
                switchToScene(LOSE_SCREEN);
            else
                switchToScene(gCurrentSceneID);
        }
        else if (gCurrentScene->getState().nextSceneID != NO_SCENE)
        {
            SceneID nextScene =
                gCurrentScene->getState().nextSceneID;

            switchToScene(nextScene);
        }

        render();
    }

    shutdown();

    return 0;
}
