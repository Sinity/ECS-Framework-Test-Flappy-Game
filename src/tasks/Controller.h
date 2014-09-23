#pragma once
#include <ECS/task.h>
#include <SFML/Graphics.hpp>

struct ApplicationClosedEvent;

class Controller : public Task {
public:
	Controller(Engine& engine);

	void receive(ApplicationClosedEvent& closeRequest);

	void update() override;

private:
	sf::RenderWindow window;
};
