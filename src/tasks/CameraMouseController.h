#pragma once
#include <ECS/task.h>
#include <SFML/Graphics.hpp>

struct MouseWheelMoved;
struct MouseButtonPressed;
struct MouseButtonReleased;
struct MouseMoved;
class CameraMouseController : public Task {
public:
	CameraMouseController(Engine& engine, sf::RenderWindow& window);
	void update() override;
	void receive(MouseWheelMoved& mouseWheelEvent);
	void receive(MouseButtonPressed& mouseButtonPressEvent);
	void receive(MouseButtonReleased& mouseButtonReleaseEvent);
	void receive(MouseMoved& mouseMoved);

private:
	sf::RenderWindow& window;

	enum {
		Nothing,
		PanView
	} mouseMode{Nothing};

	float zoomFactor;
	float panViewXFactor;
	float panViewYFactor;
	unsigned int panViewMouseButtonCode;
};
