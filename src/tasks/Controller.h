#pragma once
#include <ECS/task.h>
#include <SFML/Graphics.hpp>

struct ApplicationClosedEvent;
struct MouseWheelMoved;
struct MouseButtonPressed;
struct MouseButtonReleased;
struct MouseMoved;
class Controller : public Task {
public:
	Controller(Engine& engine);
	void update() override;
	void receive(ApplicationClosedEvent& closeRequest);

private:
	sf::RenderWindow window;
};
