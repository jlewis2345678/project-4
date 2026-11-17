#include "LoseScreen.h"

LoseScreen::LoseScreen()
    : Scene({ 0.0f, 0.0f }, "#011627")
{
}

LoseScreen::LoseScreen(Vector2 origin, const char *bgHexCode)
    : Scene(origin, bgHexCode)
{
}

LoseScreen::~LoseScreen()
{
    shutdown();
}

void LoseScreen::initialise()
{
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

void LoseScreen::update(float deltaTime)
{
    if (IsKeyPressed(KEY_R))
        mGameState.nextSceneID = START_SCREEN;
}

void LoseScreen::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));
    
    DrawText(
        "You Lose",
        static_cast<int>(
            mOrigin.x - MeasureText("You Lose", 50) / 2
        ),
        static_cast<int>(mOrigin.y - 100.0f),
        50,
        WHITE
    );
    const char *instruction = "Press R to return to the start screen";
    DrawText(
        instruction,
        static_cast<int>(
            mOrigin.x - MeasureText(instruction, 20) / 2
        ),
        static_cast<int>(mOrigin.y + 25.0f),
        20,
        LIGHTGRAY
    );
}

void LoseScreen::shutdown()
{
}
