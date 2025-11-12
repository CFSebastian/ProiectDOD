#include "Game.h"
#include <random>
#include <sstream>
#include <algorithm> 
#include <cmath>

char isOPP = 1;

#define MAX_NUMBER_OF_ENTITYS 100000

SDL_FRect mEntity[MAX_NUMBER_OF_ENTITYS] = {};
velocity mEnVelocitys[MAX_NUMBER_OF_ENTITYS] = {};
int redVal = 255;
int greenVal = 255;
int blueVal = 0;
int activeNumEnemys = 0;
void populateVecs(int numOfAditionalEn, float windowH, float windowW)
{
    for (int i = activeNumEnemys; i < activeNumEnemys+numOfAditionalEn; i++)
    {
        mEntity[i] = {
            (float)SDL_rand(windowW - ENTITY_SIZE),
            (float)SDL_rand(windowH - ENTITY_SIZE),
            ENTITY_SIZE,
            ENTITY_SIZE
        };
        mEnVelocitys[i] = {
            SDL_randf() - SDL_rand(2),
            SDL_randf() - SDL_rand(2)
        };

    }
    activeNumEnemys += numOfAditionalEn;
}

void uppdateEntitys(float speedChange, float windowH, float windowW)
{
    for (int i = 0; i < activeNumEnemys; i++)
    {
      
        // collision with the window border
        if (mEntity[i].x > windowW || mEntity[i].x < -ENTITY_SIZE || mEntity[i].y > windowH ||
            mEntity[i].y < -ENTITY_SIZE)
        {
            mEnVelocitys[i].xDir *= -1;
            mEnVelocitys[i].yDir *= -1;
        }

        // collision with other entities
        for (int j = i + 1; j < activeNumEnemys; j++)
        {
            if (SDL_HasRectIntersectionFloat(&mEntity[i], &mEntity[j]))
            {
                mEnVelocitys[i].xDir *= -1;
                mEnVelocitys[i].yDir *= -1;
                mEnVelocitys[j].xDir *= -1;
                mEnVelocitys[j].yDir *= -1;
            }
        }
        
        // Update i entity position
        mEntity[i].x += (mEnVelocitys[i].xDir * STARTING_SPEED * speedChange);
        mEntity[i].y += (mEnVelocitys[i].yDir * STARTING_SPEED * speedChange);

    }
}

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
            else if (key == SDLK_C) {
                if (isOPP == 1)
                    isOPP = 0;
                else
                    isOPP = 1;
            }
            else if (key == SDLK_LEFT) {
                speedFactor /= 1.1f;
                if (speedFactor < 0.1f) speedFactor = 0.1f;
            }
            else if (key == SDLK_SPACE) {
                printf("Space bar pressed");
                if (isOPP == 1)
                {
                    firstEntitys.updateNum(screenH, screenW);//this function does not have a upper limit
                }
                else
                {
                    populateVecs(NUMBER_OF_ENTITYS, screenH, screenW);// has upper limit
                }
                   
            }
            else if (key == SDLK_ESCAPE) {
                running = false;
            }
        }
    }
}


void Game::renderFrameOOP() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    //entitys
    firstEntitys.checkCollision(screenH,screenW);
    firstEntitys.draw(renderer);
    firstEntitys.updateDir(speedFactor);

    int numOfEntitys = firstEntitys.getNumOfEntitys(); //returns the vector capacity which has extra space for growth accommodation
    // HUD with entity count, FPS and speed
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDebugTextFormat(renderer, 1.0f, 1.0f,
                            "OOP Entitys count(+ vector aditional space): %d; FPS: %d; Current speed multiplier(press </> to change: %f)",
                            numOfEntitys, fps, speedFactor);
    
    SDL_RenderPresent(renderer);
}



void Game::renderFrameDOD() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    //entitys
    SDL_SetRenderDrawColor(renderer, redVal, greenVal, blueVal, 255);
    SDL_RenderFillRects(renderer, mEntity, activeNumEnemys);
    uppdateEntitys(speedFactor, screenH, screenW);

    int numOfEntitys = activeNumEnemys;
    // HUD with entity count, FPS and speed
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDebugTextFormat(renderer, 1.0f, 1.0f,
        "DOD Entitys count: %d; FPS: %d; Current speed multiplier(press </> to change: %f)",
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
        if (isOPP == 1)
        {
            
            renderFrameOOP();
        }
        else
        {
            renderFrameDOD();
        }
        // FPS counter / -/- reset
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
