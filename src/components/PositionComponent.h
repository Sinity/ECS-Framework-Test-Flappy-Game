#pragma once
#include <SFML/System.hpp>
#include <ECS/component.h>

COMPONENT(PositionComponent) {
	void init(ArgsMap args) {
		position.x = boost::lexical_cast<float>(args["x"]);
		position.y = boost::lexical_cast<float>(args["y"]);
	}

	sf::Vector2f position;
};
