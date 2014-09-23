#pragma once
#include <SFML/Window.hpp>

struct MouseButtonPressed {
	MouseButtonPressed(sf::Event::MouseButtonEvent button) :
			button(button) {
	}

	sf::Event::MouseButtonEvent button;
};

