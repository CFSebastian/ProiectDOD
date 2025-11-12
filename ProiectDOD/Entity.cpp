#include "Entity.h"
#include <cmath>
#include <iostream>
#include <iterator>
#include <algorithm>

Entity::Entity( SDL_Color color_)
    : eColor(color_), eSpeed(STARTING_SPEED) {
    numberOfGroups = 1;
}

void Entity::updateNum(float windowH, float windowW)
{
    for (int i = 0; i < NUMBER_OF_ENTITYS; i++)
    {
        SDL_FRect newRect {
            SDL_rand(windowW-ENTITY_SIZE),
            SDL_rand(windowH-ENTITY_SIZE),
            ENTITY_SIZE,
            ENTITY_SIZE
        };
        velocity newVelocity{
            SDL_randf() - SDL_rand(2),
            SDL_randf() - SDL_rand(2)
        };
        eRects.push_back(newRect);
        vRects.push_back(newVelocity);
    }
    numberOfGroups++;

}

void Entity::updateDir(float speedChange)
{
    for (int i = 0; i < eRects.capacity(); i++)
    {
        eRects.data()[i].x += (vRects.data()[i].xDir * STARTING_SPEED * speedChange);
        eRects.data()[i].y += (vRects.data()[i].yDir * STARTING_SPEED * speedChange);
        //printf("pass update");
    }
}
void Entity::checkCollision(float windowH, float windowW) {
    
    for (int i = 0; i < eRects.capacity(); i++)
    {
        if (eRects.data()[i].x > windowW || eRects.data()[i].x < -ENTITY_SIZE || eRects.data()[i].y > windowH ||
            eRects.data()[i].y < -ENTITY_SIZE)
        {
            vRects.data()[i].xDir *= -1;
            vRects.data()[i].yDir *= -1;
        }

        for (int j = i + 1; j < eRects.capacity(); j++)
        {
            if (SDL_HasRectIntersectionFloat(&eRects.data()[i], &eRects.data()[j]))
            {
                vRects.data()[i].xDir *= -1;
                vRects.data()[i].yDir *= -1;
                vRects.data()[j].xDir *= -1;
                vRects.data()[j].yDir *= -1;
            }
        }
    }
}



void Entity::draw(SDL_Renderer* renderer, float speedChange)  {
    
    
    SDL_SetRenderDrawColor(renderer, eColor.r, eColor.g, eColor.b, eColor.a);
    SDL_RenderFillRects(renderer, eRects.data(), eRects.capacity());
    
}

int Entity::getNumOfEntitys() {
    return eRects.capacity();
}