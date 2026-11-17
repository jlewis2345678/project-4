#ifndef SCENE_H
#define SCENE_H

#include "Entity.h"

enum SceneID{
    NO_SCENE = -1,
    START_SCREEN,
    LOSE_SCREEN,
    WIN_SCREEN,
    LEVEL_ONE,
    LEVEL_TWO,
    LEVEL_THREE,
};

struct GameState
{
    Entity *man;
    Entity *enemy;
    Entity *secondEnemy;
    Entity *thirdEnemy;
    Entity *fourthEnemy;
    Entity *fifthEnemy;
    Entity *reward;
    Map    *map;

    Music bgm;
    Sound jumpSound;

    SceneID nextSceneID;
};

class Scene
{
protected:
    GameState   mGameState{};
    Vector2     mOrigin;
    const char *mBGColourHexCode = "#000000";
    Texture2D   mBackgroundTexture{};

    void renderBackground(Color tint = WHITE);

public:
    Scene();
    Scene(Vector2 origin, const char *bgHexCode);
    virtual ~Scene();

    virtual void initialise() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void shutdown() = 0;

    GameState&  getState()                 { return mGameState;        }
    GameState   getState()           const { return mGameState;        }
    Vector2     getOrigin()          const { return mOrigin;           }
    const char *getBGColourHexCode() const { return mBGColourHexCode; }
};

#endif // SCENE_H