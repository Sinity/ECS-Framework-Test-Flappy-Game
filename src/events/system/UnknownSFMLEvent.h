#include <SFML/Window.hpp>

struct UnknownSFMLEvent {
	UnknownSFMLEvent(sf::Event event) :
			event(event) {
	}

	sf::Event event;
};
