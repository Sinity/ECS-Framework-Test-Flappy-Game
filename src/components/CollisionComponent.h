#pragma once
#include <ECS/component.h>

COMPONENT(CollisionComponent) {
    //if true, then body will be automatically pushed apart from collision by minimum possible distance
    //if true in both Entity's CollisionComponents, then both bodies are pushed by 1/2 of collision area
    //affects PositionComponent
	bool pushFromCollision = false;

    //whether CollisionEvent will be fired or not depends on this variable
	bool emitEvent = false;

    //in case both emitEvent and pushFromCollision are false, collision with this Entity is effectively disabled
    
    //Invariant that object will never change it's position.
    //If both objects in collision detection are static they won't be tested
    bool isStatic = false;
};
