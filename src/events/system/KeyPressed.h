#include <SFML/Window.hpp>

struct KeyPressed {
	KeyPressed(sf::Event::KeyEvent key) :
			key(key) {
	}

	sf::Event::KeyEvent key;
};

