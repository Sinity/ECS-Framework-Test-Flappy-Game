#pragma once
#include <ECS/task.h>
#include <SFML/Graphics.hpp>

class SFMLInputProxy : public Task {
public:
	SFMLInputProxy(Engine& engine, sf::RenderWindow& window);
	void update() override;

private:
    //for polling events from SFML and switching between window and world coordinate spaces(for mouse events)
	sf::RenderWindow& window;

    //position of mouse on last MouseMoved event, used for calculating how much mouse moved from last time
	sf::Vector2i lastMousePosition;
};
