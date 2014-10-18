#pragma once
#include <memory>
#include <ECS/component.h>
#include <SFML/Graphics.hpp>

COMPONENT(GraphicsComponent) {
	void init(ArgsMap args) {
		plane = boost::lexical_cast<int>(args["plane"]);
	}

	int plane = 0; //renderer draws first graphics components which have highest plane -
					// so plane INT_MIN will be always drawn last, so nothing will overlap it.
	sf::Texture* texture;
};
