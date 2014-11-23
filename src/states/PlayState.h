#pragma once
#include "State.h"
#include <random>
#include <ECS/engine.h>
#include <SFML/Graphics.hpp>

struct ApplicationClosedEvent;
struct MouseButtonPressed;
class PlayState : public State {
public:
    PlayState(Engine& engine);
	void receive(ApplicationClosedEvent& closeRequest);
	void receive(MouseButtonPressed& buttonPress);

private:
	void createFlappy();
	void createPipeSegment(float positionX);

	Entity flappy;
	std::vector<Entity> pipes;
	std::vector<Entity> holes;

	sf::Texture flappyTex;
	sf::Texture pipeTex;
	sf::RenderWindow window;

    Engine& engine;   
};
