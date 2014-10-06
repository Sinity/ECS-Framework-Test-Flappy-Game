#pragma once
#include <ECS/task.h>
#include <SFML/Graphics.hpp>
#include "components/PositionComponent.h"

class AttachedCameraController : public Task {
public:
	AttachedCameraController(Engine& engine, sf::RenderWindow& window, Entity attachmentPoint, sf::Vector2f offset,
	                         bool followX, bool followY) :
			Task(engine),
			window(window),
			attachmentPoint(attachmentPoint), offset(offset),
			followX(followX), followY(followY) {
	}

	void update() override {
		sf::View view = window.getView();
		sf::Vector2f desiredPos = engine.components.getComponent<PositionComponent>(attachmentPoint)->position + offset;
		sf::Vector2f finalPos = {followX ? desiredPos.x : view.getCenter().x,
								followY ? desiredPos.y : view.getCenter().y};
		view.setCenter(finalPos);
		window.setView(view);
	}

private:
	sf::RenderWindow& window;
	Entity attachmentPoint;
	sf::Vector2f offset;
	bool followX, followY;
};
