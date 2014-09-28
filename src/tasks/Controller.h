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
	void receive(MouseWheelMoved& mouseWheelEvent);
	void receive(MouseButtonPressed& mouseButtonPressEvent);
	void receive(MouseButtonReleased& mouseButtonReleaseEvent);
	void receive(MouseMoved& mouseMoved);

private:
	sf::RenderWindow window;

	enum {
		Nothing,
		PanView
	} mouseMode{Nothing};

	float zoomFactor;
	float panViewXFactor;
	float panViewYFactor;
	unsigned int panViewMouseButtonCode;
};
