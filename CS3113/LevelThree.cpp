#include "LevelThree.h"

LevelThree::LevelThree()
    : Scene{ { 0.0f, 0.0f }, "#000000" }
{}

LevelThree::LevelThree(Vector2 origin, const char *bgHexCode)
    : Scene{ origin, bgHexCode }
{}

LevelThree::~LevelThree() { shutdown(); }

void LevelThree::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
    mGameState.enemy = nullptr;
    mGameState.secondEnemy = nullptr;
    mGameState.thirdEnemy = nullptr;
    mGameState.fourthEnemy = nullptr;
    mGameState.fifthEnemy = nullptr;
    mGameState.reward = nullptr;
    mLerperTime = 0.0f;

    mGameState.jumpSound = LoadSound("assets/game/Dirt Jump.wav");

    /*
        ----------- MAP -----------
    */
    mGameState.map = new Map(
        LEVEL_THREE_WIDTH,          // map grid cols & rows
        LEVEL_THREE_HEIGHT,         // map grid rows
        mLevelData,                 // grid data
        "assets/game/tilemap.png",  // texture filepath
        TILE_DIMENSION,             // tile size
        11, 5,                      // texture cols & rows
        mOrigin                     // in-game origin
    );

    /*
        ----------- PROTAGONIST -----------
    */
    std::map<Direction, std::vector<int>> manAnimationAtlas = {
        { LEFT,  { 0, 1, 2, 3, 4, 5, 6, 7 } },
        { RIGHT, { 0, 1, 2, 3, 4, 5, 6, 7 } }
    };

    float sizeRatio = 48.0f / 64.0f;

    mGameState.man = new Entity(
        { mGameState.map->getLeftBoundary() +(2.0f + 0.25f) * TILE_DIMENSION, 
           mGameState.map->getTopBoundary() +(14.0f + 0.25f) * TILE_DIMENSION   }, // position
        { 64.0f * sizeRatio, 64.0f },              // scale
        "assets/game/character.png",                // texture file address
        ATLAS,                                      // single image or atlas?
        { 6, 8 },                                   // atlas dimensions
        manAnimationAtlas,                          // actual atlas
        PLAYER                                      // entity type
    );

    mGameState.man->setJumpingPower(550.0f);
        mGameState.man->setColliderDimensions({mGameState.man->getScale().x,
    mGameState.man->getScale().y * 0.85f});
    mGameState.man->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });

    mGameState.reward = new Entity(
        {
            mGameState.map->getRightBoundary() -
                1.5f * TILE_DIMENSION,
            mGameState.map->getTopBoundary() +
                1.5f * TILE_DIMENSION
        },
        { 48.0f, 48.0f },
        "assets/game/tile_0061.png",
        SINGLE,
        { 1, 1 },
        {},
        NONE
    );

    mGameState.reward->setColliderDimensions({ 48.0f, 48.0f });

    std::map<Direction, std::vector<int>> enemyAnimationAtlas = {
        { LEFT,  { 8, 9, 10, 11, 12, 13, 14, 15 } },
        { RIGHT, { 8, 9, 10, 11, 12, 13, 14, 15 } }
    };

    auto createEnemy = [&](float column, float row, AIType aiType) {
        Entity *enemy = new Entity(
            {
                mGameState.map->getLeftBoundary() +
                    (column + 0.5f) * TILE_DIMENSION,
                mGameState.map->getTopBoundary() +
                    (row - 0.5f) * TILE_DIMENSION
            },
            { 48.0f, 48.0f },
            "assets/game/enemy.png",
            ATLAS,
            { 8, 5 },
            enemyAnimationAtlas,
            NPC
        );

        enemy->setAIType(aiType);
        enemy->setAIState(aiType == FOLLOWER ? IDLE : WALKING);
        enemy->setSpeed(100);
        enemy->setColliderDimensions({ 48.0f, 48.0f });

        if (aiType != LERPER)
            enemy->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });

        return enemy;
    };

    mGameState.enemy = createEnemy(5.0f, 15.0f, WANDERER);
    mGameState.secondEnemy = createEnemy(13.0f, 15.0f, FOLLOWER);
    mGameState.thirdEnemy = createEnemy(21.0f, 15.0f, WANDERER);
    mGameState.fourthEnemy = createEnemy(29.0f, 15.0f, FOLLOWER);
    mGameState.fifthEnemy = createEnemy(37.0f, 7.0f, LERPER);
}

void LevelThree::update(float deltaTime)
{
    mGameState.man->update(
        deltaTime,      // delta time / fixed timestep
        nullptr,        // player
        mGameState.map, // map
        nullptr,        // collidable entities
        0               // col. entity count
    );

    mLerperTime += deltaTime;

    Entity *enemies[] = {
        mGameState.enemy,
        mGameState.secondEnemy,
        mGameState.thirdEnemy,
        mGameState.fourthEnemy,
        mGameState.fifthEnemy
    };

    for (Entity *enemy : enemies)
    {
        if (enemy->getAIType() == LERPER)
        {
            Vector2 position = enemy->getPosition();
            position.x =
                mGameState.map->getLeftBoundary() +
                (35.0f + 3.0f * sinf(mLerperTime)) * TILE_DIMENSION;
            position.y =
                mGameState.map->getTopBoundary() +
                (7.0f + 1.0f * sinf(mLerperTime * 2.0f)) *
                    TILE_DIMENSION;
            enemy->setPosition(position);
        }
        else
        {
            enemy->update(
                deltaTime,
                mGameState.man,
                mGameState.map,
                nullptr,
                0
            );
        }
    }
}

void LevelThree::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));
    renderBackground(ColorFromHex("#1f618d"));

    mGameState.man->render();
    mGameState.map->render();

    mGameState.enemy->render();
    mGameState.secondEnemy->render();
    mGameState.thirdEnemy->render();
    mGameState.fourthEnemy->render();
    mGameState.fifthEnemy->render();
    mGameState.reward->render();
}

void LevelThree::shutdown()
{
    delete mGameState.man;
    mGameState.man = nullptr;
    delete mGameState.enemy;
    delete mGameState.secondEnemy;
    delete mGameState.thirdEnemy;
    delete mGameState.fourthEnemy;
    delete mGameState.fifthEnemy;
    mGameState.enemy = nullptr;
    mGameState.secondEnemy = nullptr;
    mGameState.thirdEnemy = nullptr;
    mGameState.fourthEnemy = nullptr;
    mGameState.fifthEnemy = nullptr;
    delete mGameState.reward;
    mGameState.reward = nullptr;
    delete mGameState.map;
    mGameState.map = nullptr;

    UnloadSound(mGameState.jumpSound);
    mGameState.jumpSound = {};
}