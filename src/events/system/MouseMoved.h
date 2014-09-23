#pragma once
#include <SFML/Window.hpp>

struct MouseMoved {
	MouseMoved(sf::Event::MouseMoveEvent where) :
			where(where) {
	}

	sf::Event::MouseMoveEvent where;
};

