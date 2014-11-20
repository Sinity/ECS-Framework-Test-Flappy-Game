#pragma once
#include <SFML/System.hpp>
#include <ECS/component.h>

COMPONENT(PositionComponent) {
	void init(ArgsMap args) {
		position.x = boost::lexical_cast<float>(args["x"]);
		position.y = boost::lexical_cast<float>(args["y"]);
	}

    //Point(vector with fixed orgin at (0, 0) with this Entity's position in world
	sf::Vector2f position;
};
