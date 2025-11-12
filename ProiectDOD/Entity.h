#pragma once
#include <SDL3/SDL.h>
#include <cstdint>
#include <iostream>
#include <vector>

#define ENTITY_SIZE 1.5
#define NUMBER_OF_ENTITYS 1000
#define STARTING_SPEED 0.5f

struct velocity {
    float xDir, yDir;
};

class Entity {
private:
    std::vector<SDL_FRect> eRects;
    std::vector<velocity> vRects;
    SDL_Color eColor;
    float eSpeed;
    int numberOfGroups;
public:
    int getNumOfEntitys();
    Entity(SDL_Color color);
    void updateNum(float windowH, float windowW);
    void updateDir(float speedChange);
    void draw(SDL_Renderer* renderer,float speedChange=0);
    void checkCollision(float windowH, float windowW);
    
};
