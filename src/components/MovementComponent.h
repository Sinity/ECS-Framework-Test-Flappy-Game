#pragma once
#include <SFML/System.hpp>
#include <ECS/component.h>

COMPONENT(MovementComponent) {
	void init(ArgsMap args) {
		oldPosition.x = boost::lexical_cast<float>(args["oldPositionX"]);
		oldPosition.y = boost::lexical_cast<float>(args["oldPositionY"]);
		resultantForce.x = boost::lexical_cast<float>(args["resultantForceX"]);
		resultantForce.y = boost::lexical_cast<float>(args["resultantForceY"]);
		mass = boost::lexical_cast<float>(args["mass"]);
	}

    sf::Vector2f oldPosition = {0, 0};
    sf::Vector2f resultantForce = {0, 0};
    float mass = 1.0f;
};
