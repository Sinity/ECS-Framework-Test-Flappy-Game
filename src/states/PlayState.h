#pragma once
#include "State.h"
#include <random>
#include <ECS/engine.h>
#include <SFML/Graphics.hpp>

struct ApplicationClosedEvent;
struct MouseButtonPressed;
struct CollisionEvent;
class PlayState : public State {
public:
    PlayState(Engine& engine);
	void receive(ApplicationClosedEvent& closeRequest);
	void receive(MouseButtonPressed& buttonPress);
    void receive(CollisionEvent& collision);

private:
	void createFlappy();
	void createPipeSegment(float positionX);
    void createScoreCounter();

	Entity flappy = 0;
    Entity scoreCounter = 0;
    int score = 0;

	std::vector<Entity> holes;
    std::vector<Entity> pipes; //even indices -> upper segments, odd indices -> lower segments
    Entity currentlyCollidingHole = 0;
    float lastPipePosition = 0.f;
    float pipeSpacing = 0.f;

    std::shared_ptr<sf::Texture> flappyTex = std::make_shared<sf::Texture>();
    std::shared_ptr<sf::Texture> pipeTex = std::make_shared<sf::Texture>();
    sf::Font counterFont;

	sf::RenderWindow window;
    Engine& engine;   
};
