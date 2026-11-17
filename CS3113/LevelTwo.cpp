#include "LevelTwo.h"

LevelTwo::LevelTwo()
    : Scene{ { 0.0f, 0.0f }, "#000000" }
{}

LevelTwo::LevelTwo(Vector2 origin, const char *bgHexCode)
    : Scene{ origin, bgHexCode }
{}

LevelTwo::~LevelTwo() { shutdown(); }

void LevelTwo::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
    mGameState.enemy = nullptr;
    mGameState.secondEnemy = nullptr;
    mGameState.thirdEnemy = nullptr;
    mGameState.fourthEnemy = nullptr;
    mGameState.fifthEnemy = nullptr;
    mGameState.reward = nullptr;

    mGameState.jumpSound = LoadSound("assets/game/Dirt Jump.wav");

    /*
        ----------- MAP -----------
    */
    mGameState.map = new Map(
        LEVEL_TWO_WIDTH,            // map grid cols & rows
        LEVEL_TWO_HEIGHT,           // map grid rows
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
        {  mGameState.map->getLeftBoundary() +(2.0f + 0.25f) * TILE_DIMENSION, 
           mGameState.map->getTopBoundary() +(10.0f + 0.25f) * TILE_DIMENSION  }, // position
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
            mGameState.map->getLeftBoundary() +
                (38.0f + 0.5f) * TILE_DIMENSION,
            mGameState.map->getTopBoundary() +
                (10.0f + 0.5f) * TILE_DIMENSION
        },
        { 48.0f, 48.0f },
        "assets/game/tile_0061.png",
        NONE
    );

    mGameState.reward->setColliderDimensions({ 48.0f, 48.0f });

        std::map<Direction, std::vector<int>> enemyAnimationAtlas = {
            { LEFT,  { 8, 9, 10, 11, 12, 13, 14, 15 } },
            { RIGHT, { 8, 9, 10, 11, 12, 13, 14, 15 } }
        };

        mGameState.enemy = new Entity(
            {
                mGameState.map->getLeftBoundary() +
                    (18.0f + 0.5f) * TILE_DIMENSION,
                mGameState.map->getTopBoundary() +
                    (7.0f - 0.5f) * TILE_DIMENSION
            },
            { 48.0f, 48.0f },
            "assets/game/enemy.png",
            ATLAS,
            { 8, 5 },
            enemyAnimationAtlas,
            NPC
        );

        mGameState.enemy->setAIType(FOLLOWER);
        mGameState.enemy->setAIState(IDLE);
        mGameState.enemy->setSpeed(100);
        mGameState.enemy->setColliderDimensions({ 48.0f, 48.0f });
        mGameState.enemy->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });

        mGameState.secondEnemy = new Entity(
            {
                mGameState.map->getLeftBoundary() +
                    (23.0f + 0.5f) * TILE_DIMENSION,
                mGameState.map->getTopBoundary() +
                    (5.0f - 0.5f) * TILE_DIMENSION
            },
            { 48.0f, 48.0f },
            "assets/game/enemy.png",
            ATLAS,
            { 8, 5 },
            enemyAnimationAtlas,
            NPC
        );

        mGameState.secondEnemy->setAIType(WANDERER);
        mGameState.secondEnemy->setAIState(WALKING);
        mGameState.secondEnemy->setSpeed(100);
        mGameState.secondEnemy->setColliderDimensions({ 48.0f, 48.0f });
        mGameState.secondEnemy->setAcceleration(
            { 0.0f, ACCELERATION_OF_GRAVITY }
        );
}

void LevelTwo::update(float deltaTime)
{
    mGameState.man->update(
        deltaTime,      // delta time / fixed timestep
        nullptr,        // player
        mGameState.map, // map
        nullptr,        // collidable entities
        0               // col. entity count
    );

    mGameState.enemy->update(
        deltaTime,
        mGameState.man,
        mGameState.map,
        nullptr,
        0
    );

    mGameState.secondEnemy->update(
        deltaTime,
        mGameState.man,
        mGameState.map,
        nullptr,
        0
    );
}

void LevelTwo::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.man->render();
    mGameState.enemy->render();
    mGameState.secondEnemy->render();
    mGameState.map->render();
    mGameState.reward->render();
}

void LevelTwo::shutdown()
{
    delete mGameState.man;
    mGameState.man = nullptr;
    delete mGameState.enemy;
    mGameState.enemy = nullptr;
    delete mGameState.secondEnemy;
    mGameState.secondEnemy = nullptr;
    delete mGameState.reward;
    mGameState.reward = nullptr;
    delete mGameState.map;
    mGameState.map = nullptr;

    UnloadSound(mGameState.jumpSound);
    mGameState.jumpSound = {};
}