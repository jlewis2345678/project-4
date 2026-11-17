#include "WinScreen.h"

WinScreen::WinScreen()
    : Scene({ 0.0f, 0.0f }, "#0066cc")
{
}

WinScreen::WinScreen(Vector2 origin, const char *bgHexCode)
    : Scene(origin, bgHexCode)
{
}

WinScreen::~WinScreen()
{
    shutdown();
}

void WinScreen::initialise()
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

void WinScreen::update(float deltaTime)
{
    if (IsKeyPressed(KEY_R))
        mGameState.nextSceneID = START_SCREEN;
}

void WinScreen::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    DrawText(
        "You Win",
        static_cast<int>(
            mOrigin.x - MeasureText("You Win", 50) / 2
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

void WinScreen::shutdown()
{
}
