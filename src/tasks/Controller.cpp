#include "Controller.h"
#include "Renderer.h"
#include "SFMLInputProxy.h"
#include "InputEcho.h"
#include "VerletIntegrator.h"
#include "events/system/ApplicationClosed.h"
#include "events/system/MouseWheelMoved.h"
#include "events/system/MouseButtonPressed.h"
#include "events/system/MouseButtonReleased.h"
#include "events/system/MouseMoved.h"
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

void Controller::receive(MouseWheelMoved& mouseWheelEvent) {
	sf::View currentView = window.getView();
	currentView.zoom(1.f + zoomFactor * mouseWheelEvent.ticks);
	window.setView(currentView);
}

void Controller::receive(MouseButtonPressed& mousePress) {
	if(mousePress.button.button == panViewMouseButtonCode) {
		mouseMode = PanView;
	}
}

void Controller::receive(MouseButtonReleased& mouseRelease) {
	if(mouseRelease.button.button == panViewMouseButtonCode) {
		mouseMode = Nothing;
	}
}

void Controller::receive(MouseMoved& mouseMoved) {
	if(mouseMode == PanView) {
		sf::View currentView = window.getView();
		currentView.move(-mouseMoved.worldDelta.x * panViewXFactor, -mouseMoved.worldDelta.y * panViewYFactor);
		window.setView(currentView);
	}
}

Controller::Controller(Engine& engine) :
		Task(engine),
		zoomFactor(engine.config.get<float>("gameplay.zoom.factor", 0.1f)),
		panViewXFactor(engine.config.get<float>("gameplay.camera.pan.factor.x", 1.f)),
		panViewYFactor(engine.config.get<float>("gameplay.camera.pan.factor.y", 1.f)),
		panViewMouseButtonCode(engine.config.get<unsigned int>("gameplay.camera.pan.mouseButtonCode", 2)) {
	engine.events.connect<ApplicationClosedEvent>(*this);
	engine.events.connect<MouseWheelMoved>(*this);
	engine.events.connect<MouseButtonPressed>(*this);
	engine.events.connect<MouseButtonReleased>(*this);
	engine.events.connect<MouseMoved>(*this);

	engine.tasks.addTask<Renderer>(window);
	engine.tasks.addTask<SFMLInputProxy>(window);
	engine.tasks.addTask<InputEcho>();
	engine.tasks.addTask<VerletIntegrator>();

	Entity e = engine.entityFactory.createEntity("TestEntity", {});
	RenderingComponent* renderingComponent = engine.components.getComponent<RenderingComponent>(e);

	sf::Texture& pipeSegment = *new sf::Texture; //it's only temp(leak)
	pipeSegment.loadFromFile("flappy_pipe.png");

	std::shared_ptr<sf::Sprite> pipe = std::make_shared<sf::Sprite>();
	pipe->setTexture(pipeSegment);
	pipe->setScale(2.f / pipe->getLocalBounds().width, 3.f / pipe->getLocalBounds().height);
	renderingComponent->drawablesList.push_back(pipe);
}
