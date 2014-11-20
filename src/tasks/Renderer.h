#pragma once
#include <ECS/task.h>
#include <SFML/Graphics.hpp>

class Renderer : public Task {
public:
	Renderer(Engine& engine, sf::RenderWindow& window);
	void update() override;

private:
    //mainly for rendering into it
	sf::RenderWindow& window;

    //color of background
	sf::Color fillColor;
};
