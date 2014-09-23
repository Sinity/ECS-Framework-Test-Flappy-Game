#include "VerletIntegrator.h"
#include "components/MovementComponent.h"
#include "components/PositionComponent.h"

VerletIntegrator::VerletIntegrator(Engine& engine) :
		Task(engine) {
}

void VerletIntegrator::update() {
	std::vector<MovementComponent*> movementComponents;
	std::vector<PositionComponent*> positionComponents;
	engine.components.intersection(movementComponents, positionComponents);

	for (unsigned int i = 0; i < movementComponents.size(); i++) {
		sf::Vector2f acceleration = movementComponents[i]->resultantForce / movementComponents[i]->mass;
		movementComponents[i]->resultantForce = {0, 0};
		float time = (float)frequency.count() / 1000;

		sf::Vector2f currentPosition = positionComponents[i]->position;
		positionComponents[i]->position += currentPosition -
				movementComponents[i]->oldPosition + acceleration * (time * time);
		movementComponents[i]->oldPosition = currentPosition;
	}
}
