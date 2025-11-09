#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "Entity.h"

class Game {
private:
    int screenW, screenH;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    bool running = true;

    float speedFactor = 1;

    Entity firstEntitys = Entity(SDL_Color{
        255,0,255,255
        });

    uint64_t perfFreq = 0;
    uint64_t lastTime = 0;
    int fps = 0;
    int fpsCounter = 0;
    float fpsTimer = 0.0f;

    void processInput();
    void renderFrame();
    void spawnEntities(size_t count);
    void broadPhaseCollisions();
public:
    Game(int w, int h);
    ~Game();
    bool init();
    void run();


};
