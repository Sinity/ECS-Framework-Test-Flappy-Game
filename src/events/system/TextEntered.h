#include <SFML/Window.hpp>

struct TextEntered {
	TextEntered(sf::Event::TextEvent text) :
			text(text) {
	}

	sf::Event::TextEvent text;
};

