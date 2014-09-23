#include "SFMLInputProxy.h"
#include "events/system/ApplicationClosed.h"
#include "events/system/KeyPressed.h"
#include "events/system/KeyReleased.h"
#include "events/system/MouseButtonPressed.h"
#include "events/system/MouseButtonReleased.h"
#include "events/system/MouseMoved.h"
#include "events/system/TextEntered.h"
#include "events/system/UnknownSFMLEvent.h"

SFMLInputProxy::SFMLInputProxy(Engine& engine, sf::RenderWindow& window) :
		Task(engine),
		window(window) {
	window.setKeyRepeatEnabled(false);
}

void SFMLInputProxy::update() {
	sf::Event currentEvent;
	while(window.pollEvent(currentEvent)) {
		switch(currentEvent.type) {
			case sf::Event::Closed:
				engine.events.emplace<ApplicationClosedEvent>();
		        break;
			case sf::Event::KeyPressed:
				engine.events.emplace<KeyPressed>(currentEvent.key);
		        break;
			case sf::Event::KeyReleased:
				engine.events.emplace<KeyReleased>(currentEvent.key);
		        break;
			case sf::Event::TextEntered:
				engine.events.emplace<TextEntered>(currentEvent.text);
		        break;
			case sf::Event::MouseButtonPressed:
				engine.events.emplace<MouseButtonPressed>(currentEvent.mouseButton);
		        break;
			case sf::Event::MouseButtonReleased:
				engine.events.emplace<MouseButtonReleased>(currentEvent.mouseButton);
		        break;
			case sf::Event::MouseMoved:
				engine.events.emplace<MouseMoved>(currentEvent.mouseMove);
		        break;
			default:
				engine.events.emplace<UnknownSFMLEvent>(currentEvent);
		}
	}
}
