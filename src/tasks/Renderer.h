#pragma once
#include <ECS/task.h>
#include <SFML/Graphics.hpp>

class Renderer : public Task {
public:
	Renderer(Engine& engine, sf::RenderWindow& window);
	void update() override;

private:
	sf::RenderWindow& window;
	sf::Color fillColor;
};
