#pragma once
#include <ECS/task.h>
#include <SFML/Graphics.hpp>

class SFMLInputProxy : public Task {
public:
	SFMLInputProxy(Engine& engine, sf::RenderWindow& window);
	void update() override;

private:
	sf::RenderWindow& window;
	sf::Vector2i lastMousePosition;
};
