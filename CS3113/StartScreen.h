#ifndef START_SCREEN_H
#define START_SCREEN_H

#include "Scene.h"

class StartScreen : public Scene
{
private:
    Texture2D mMenuButtons;

public:
    StartScreen();
    StartScreen(Vector2 origin, const char *bgHexCode);
    ~StartScreen();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // START_SCREEN_H