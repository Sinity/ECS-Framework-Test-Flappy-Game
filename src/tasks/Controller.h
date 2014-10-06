#pragma once
#include <ECS/task.h>
#include <SFML/Graphics.hpp>

struct ApplicationClosedEvent;
struct MouseButtonPressed;
class Controller : public Task {
public:
	Controller(Engine& engine);
	void update() override;
	void receive(ApplicationClosedEvent& closeRequest);
	void receive(MouseButtonPressed& buttonPress);

private:
	sf::RenderWindow window;

	Entity flappy;
	std::vector<Entity> pipes;
	std::vector<Entity> holes;

	sf::Texture flappyTex;
	sf::Texture pipeTex;

	void createPipeSegment(float positionX);
	void createFlappy();
};
