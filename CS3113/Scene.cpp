#include "Scene.h"

Scene::Scene() : mGameState{}, mOrigin{}
{
    mGameState.nextSceneID = NO_SCENE;
}

Scene::Scene(Vector2 origin, const char *bgHexCode)
    : mGameState{}, mOrigin{ origin }, mBGColourHexCode{ bgHexCode }
{
    mGameState.nextSceneID = NO_SCENE;
}