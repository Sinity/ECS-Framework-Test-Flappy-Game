#pragma once
#include <ECS/task.h>

struct UnknownSFMLEvent;
struct ApplicationClosedEvent;
struct KeyPressed;
struct KeyReleased;
struct TextEntered;
struct MouseButtonPressed;
struct MouseButtonReleased;
struct MouseMoved;
struct MouseWheelMoved;
class InputEcho : public Task {
public:
	InputEcho(Engine& engine);

	void update() override;

	void receive(UnknownSFMLEvent& unknownSFMLEvent);
	void receive(ApplicationClosedEvent& appClosed);
	void receive(KeyPressed& keyPressed);
	void receive(KeyReleased& keyReleased);
	void receive(TextEntered& textEntered);
	void receive(MouseButtonPressed& mouseButtonPressed);
	void receive(MouseButtonReleased& mouseButtonReleased);
	void receive(MouseMoved& mouseMoved);
	void receive(MouseWheelMoved& mouseWheelMoved);

private:
	Logger debugLogger;
};
