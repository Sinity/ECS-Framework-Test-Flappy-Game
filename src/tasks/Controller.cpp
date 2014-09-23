#include "Controller.h"
#include "Renderer.h"
#include "SFMLInputProxy.h"
#include "InputEcho.h"
#include "VerletIntegrator.h"
#include "events/system/ApplicationClosed.h"
#include "components/PositionComponent.h"
#include "components/MovementComponent.h"
#include "components/RenderingComponent.h"

void Controller::update() {
}

void Controller::receive(ApplicationClosedEvent& closeRequest) {
	(void)closeRequest;
	engine.logger.info("Application close request received in Controller, stopping engine...");
	engine.stop();
}

Controller::Controller(Engine& engine) :
		Task(engine) {
	engine.events.connect<ApplicationClosedEvent>(*this);

	engine.tasks.addTask<Renderer>(window);
	engine.tasks.addTask<SFMLInputProxy>(window);
	engine.tasks.addTask<InputEcho>();
	engine.tasks.addTask<VerletIntegrator>();

	Entity e = engine.entityFactory.createEntity("TestEntity", {});
	RenderingComponent* renderingComponent = engine.components.getComponent<RenderingComponent>(e);
	std::shared_ptr<sf::RectangleShape> rect = std::make_shared<sf::RectangleShape>();
	rect->setFillColor(sf::Color::Red);
	rect->setSize({1.2f, 0.6f});
	renderingComponent->drawablesList.push_back(rect);
}
