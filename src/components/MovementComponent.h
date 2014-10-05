#pragma once
#include <vector>
#include <ECS/component.h>
#include <SFML/System.hpp>

COMPONENT(MovementComponent) {
	void init(ArgsMap args) {
		mass = boost::lexical_cast<float>(args["mass"]);
	}

	void addTemporalForce(sf::Vector2f force) {
		resultantForce += force;
	}

	void addPersistentForce(sf::Vector2f force) {
		persistentForces.push_back(force);
	}

	std::vector<sf::Vector2f> persistentForces;
    sf::Vector2f resultantForce = {0, 0};
    float mass = 1.0f;
	sf::Vector2f oldPosition = {0, 0};
};
