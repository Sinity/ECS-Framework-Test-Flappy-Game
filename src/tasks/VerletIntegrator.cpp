#include "VerletIntegrator.h"
#include "../components/MovementComponent.h"
#include "../components/PositionComponent.h"

VerletIntegrator::VerletIntegrator(Engine& engine) :
		Task(engine) {
}

void VerletIntegrator::update() {
    auto elapsedTime = (float)frequency.count() / 1000;

	auto movementComponents = std::vector<MovementComponent*>{};
	auto positions = std::vector<PositionComponent*>{};
	engine.components.intersection(movementComponents, positions);

	for (auto i = 0u; i < movementComponents.size(); i++) {
		for(auto force : movementComponents[i]->persistentForces) {
			movementComponents[i]->resultantForce += force;
		}

		auto currentPosition = positions[i]->position;
        auto velocity = currentPosition - movementComponents[i]->oldPosition;
		auto acceleration = movementComponents[i]->resultantForce / movementComponents[i]->mass;
		positions[i]->position = positions[i]->position + velocity + acceleration * (elapsedTime*elapsedTime);

		movementComponents[i]->oldPosition = currentPosition;
		movementComponents[i]->resultantForce = {0, 0};
	}
}

