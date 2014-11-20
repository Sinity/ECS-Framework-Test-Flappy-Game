#pragma once
#include <ECS/component.h>

COMPONENT(OrientationComponent) {
    //rotation of object with orgin in it's center, in degrees
	float rotation = 0.f;
};
