#ifndef LOSE_SCREEN_H
#define LOSE_SCREEN_H

#include "Scene.h"

class LoseScreen : public Scene
{
public:
    LoseScreen();
    LoseScreen(Vector2 origin, const char *bgHexCode);
    ~LoseScreen();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // LOSE_SCREEN_H
