#include <SFML/Window.hpp>

struct KeyReleased {
	KeyReleased(sf::Event::KeyEvent key) :
			key(key) {
	}

	sf::Event::KeyEvent key;
};

