#include "Scene.h"

Scene::Scene() : mGameState{}, mOrigin{}
{
    mGameState.nextSceneID = NO_SCENE;
}

Scene::Scene(Vector2 origin, const char *bgHexCode)
    : mGameState{}, mOrigin{ origin }, mBGColourHexCode{ bgHexCode }
{
    mGameState.nextSceneID = NO_SCENE;
    mBackgroundTexture = LoadTexture("assets/game/background.png");
}

Scene::~Scene()
{
    UnloadTexture(mBackgroundTexture);
}

void Scene::renderBackground(Color tint)
{
    Rectangle sourceArea = {
        0.0f,
        0.0f,
        static_cast<float>(mBackgroundTexture.width),
        static_cast<float>(mBackgroundTexture.height)
    };

    Rectangle destinationArea = {
        mOrigin.x - 500.0f,
        mOrigin.y - 300.0f,
        1000.0f,
        600.0f
    };

    DrawTexturePro(
        mBackgroundTexture,
        sourceArea,
        destinationArea,
        { 0.0f, 0.0f },
        0.0f,
        tint
    );
}