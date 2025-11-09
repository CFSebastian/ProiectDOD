#include "Game.h"
#include <random>
#include <sstream>
#include <algorithm> // <<--- pentru std::clamp
#include <cmath>

Game::Game(int w, int h) : screenW(w), screenH(h) {}

Game::~Game() {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
}

bool Game::init() {

    window = SDL_CreateWindow("SDL3 50K Entities", screenW, screenH, SDL_WINDOW_RESIZABLE);
    if (!window) {
        SDL_Log("SDL_CreateWindow Error: %s", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        SDL_Log("SDL_CreateRenderer Error: %s", SDL_GetError());
        return false;
    }

    perfFreq = SDL_GetPerformanceFrequency();
    lastTime = SDL_GetPerformanceCounter();

    return true;
}


void Game::processInput() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) {
            running = false;
        }
        else if (e.type == SDL_EVENT_KEY_DOWN) {
            // SDL3: key field e.key.key este un SDL_Keycode
            SDL_Keycode key = e.key.key;
            if (key == SDLK_RIGHT) {
                speedFactor *= 1.1f;
            }
            else if (key == SDLK_LEFT) {
                speedFactor /= 1.1f;
                if (speedFactor < 0.1f) speedFactor = 0.1f;
            }
            else if (key == SDLK_SPACE) {
                printf("Space bar pressed");
                firstEntitys.updateNum(screenH,screenW);
            }
            else if (key == SDLK_ESCAPE) {
                running = false;
            }
        }
    }
}


void Game::renderFrame() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    //entitys
    firstEntitys.checkCollision(screenH,screenW);
    firstEntitys.draw(renderer);
    firstEntitys.updateDir(speedFactor);

    int numOfEntitys = firstEntitys.getNumOfEntitys();
     // HUD minimal – doar 3 bare colorate
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDebugTextFormat(renderer, 1.0f, 1.0f,
                            "Entitys count: %d; FPS: %d; Current speed multiplier(press </> to change: %f)",
                            numOfEntitys, fps, speedFactor);
    
    SDL_RenderPresent(renderer);
}

void Game::run() {
    running = true;
    while (running) {
        uint64_t now = SDL_GetPerformanceCounter();
        float dt = float(now - lastTime) / float(perfFreq);
        lastTime = now;

        processInput();
        renderFrame();
        // 
        fpsCounter++;
        fpsTimer += dt;
        if (fpsTimer >= 1.0f) {
            fps = fpsCounter;
            fpsCounter = 0;
            fpsTimer = 0.0f;
        }
        SDL_Delay(1);
    }
}
