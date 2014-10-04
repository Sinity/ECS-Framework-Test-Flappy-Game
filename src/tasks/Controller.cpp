#include "Controller.h"
#include "Renderer.h"
#include "SFMLInputProxy.h"
#include "InputEcho.h"
#include "VerletIntegrator.h"
#include "CameraMouseController.h"
#include "events/system/ApplicationClosed.h"
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
	engine.tasks.addTask<CameraMouseController>(window);

	Entity e = engine.entityFactory.createEntity("TestEntity", {});
	RenderingComponent* renderingComponent = engine.components.getComponent<RenderingComponent>(e);

	sf::Texture& pipeSegment = *new sf::Texture; //it's only temp(leak)
	pipeSegment.loadFromFile("flappy_pipe.png");

	std::shared_ptr<sf::Sprite> pipe = std::make_shared<sf::Sprite>();
	pipe->setTexture(pipeSegment);
	pipe->setScale(2.f / pipe->getLocalBounds().width, 3.f / pipe->getLocalBounds().height);
	renderingComponent->drawablesList.push_back(pipe);
}
