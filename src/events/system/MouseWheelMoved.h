#pragma once
#include <SFML/Window.hpp>

struct MouseWheelMoved {
	MouseWheelMoved(sf::Event::MouseWheelEvent event) :
			ticks(event.delta),
			x(event.x),
			y(event.y) {
	}

	int ticks;
	int x;
	int y;
};

