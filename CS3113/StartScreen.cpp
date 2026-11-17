#include "StartScreen.h"

StartScreen::StartScreen()
    : Scene({ 0.0f, 0.0f }, "#011627")
{
}

StartScreen::StartScreen(Vector2 origin, const char *bgHexCode)
    : Scene(origin, bgHexCode)
{
}

StartScreen::~StartScreen()
{
    shutdown();
}

void StartScreen::initialise()
{
    resetLives();

    mGameState.nextSceneID = NO_SCENE;
    mGameState.man = nullptr;
    mGameState.enemy = nullptr;
    mGameState.secondEnemy = nullptr;
    mGameState.thirdEnemy = nullptr;
    mGameState.fourthEnemy = nullptr;
    mGameState.fifthEnemy = nullptr;
    mGameState.reward = nullptr;
    mGameState.map = nullptr;
}

void StartScreen::update(float deltaTime)
{
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
    {
        mGameState.nextSceneID = LEVEL_ONE;
    }
}

void StartScreen::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    DrawText(
        "Platformer",
        static_cast<int>(
            mOrigin.x - MeasureText("Platformer", 50) / 2
        ),
        static_cast<int>(mOrigin.y - 150.0f),
        50,
        WHITE
    );


    Rectangle sourceArea = {
        1.0f * (mMenuButtons.width / 2.0f),
        0.0f,
        mMenuButtons.width / 2.0f,
        mMenuButtons.height / 9.0f
    };

    Rectangle destinationArea = {
        mOrigin.x - 100.0f,
        mOrigin.y - 25.0f,
        200.0f,
        50.0f
    };

    const char *instruction = "Press ENTER or SPACE to play";
    int instructionFontSize = 20;

    DrawText(
        "Press ENTER or SPACE to play",
        static_cast<int>(
            mOrigin.x -
            MeasureText(instruction, 20) / 2
        ),
        static_cast<int>(mOrigin.y + 100.0f),
        instructionFontSize,
        LIGHTGRAY
    );
}

void StartScreen::shutdown()
{
    if (mMenuButtons.id != 0)
    {
        UnloadTexture(mMenuButtons);
        mMenuButtons = {};
    }
}