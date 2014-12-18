#pragma once
#include <ECS/component.h>
#include <SFML/System.hpp>

struct CollisionEvent {
    //bodies taking part in this collision
    Entity firstBody = 0;
    Entity secondBody = 0;
    
    //bodies were translated by these vectors
    //if bodies weren't translated, then these vectors are set to 0
	sf::Vector2f firstBodyTranslation = {0.f, 0.f};
    sf::Vector2f secondBodyTranslation = {0.f, 0.f}; 

    //if bodies weren't translated, then it's vector that applied to first entity will
    //push it from collision by minimum possible distance. To be applied to second body, flip sign
    //0 if bodies were translated already
	sf::Vector2f minimumTranslationVector = {0.f, 0.f};
};
