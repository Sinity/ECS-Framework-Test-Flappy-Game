#pragma once
#include <SFML/Window.hpp>

struct MouseButtonReleased {
	MouseButtonReleased(sf::Event::MouseButtonEvent button) :
			button(button) {
	}

	sf::Event::MouseButtonEvent button;
};

