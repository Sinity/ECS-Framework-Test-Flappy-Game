#include "Controller.h"
#include <random>
#include <components/OrientationComponent.h>
#include "Renderer.h"
#include "SFMLInputProxy.h"
#include "InputEcho.h"
#include "VerletIntegrator.h"
#include "AttachedCameraController.h"
#include "events/system/ApplicationClosed.h"
#include "events/system/MouseButtonPressed.h"
#include "components/PositionComponent.h"
#include "components/SizeComponent.h"
#include "components/MovementComponent.h"
#include "components/GraphicsComponent.h"

void Controller::update() {
}

void Controller::receive(ApplicationClosedEvent& closeRequest) {
	(void)closeRequest;
	engine.logger.info("Application close request received in Controller, stopping engine...");
	engine.stop();
}

void Controller::receive(MouseButtonPressed& buttonPress) {
	if(buttonPress.button.button == 0) {
		auto* flappyMovement = engine.components.getComponent<MovementComponent>(flappy);
		auto* flappyPosition = engine.components.getComponent<PositionComponent>(flappy);
		flappyMovement->oldPosition.y = flappyPosition->position.y; //flappy universe logic here
		flappyMovement->addTemporalForce({0, engine.config.get<float>("gameplay.flappy.forces.lift")});
	}
}

Controller::Controller(Engine& engine) :
		Task(engine) {
	engine.events.connect<ApplicationClosedEvent>(*this);
	engine.events.connect<MouseButtonPressed>(*this);

	//boot all core tasks
	engine.tasks.addTask<Renderer>(window);
	engine.tasks.addTask<SFMLInputProxy>(window);
	engine.tasks.addTask<VerletIntegrator>();
	engine.tasks.addTask<InputEcho>();

	//load textures
	pipeTex.loadFromFile(engine.config.get("gameplay.files.pipeTexture"));
	flappyTex.loadFromFile(engine.config.get("gameplay.files.flappyTexture"));

	createFlappy();
	engine.tasks.addTask<AttachedCameraController>(window, flappy,
	                                               sf::Vector2f{engine.config.get<float>("camera.offset.x"),
			                                                    engine.config.get<float>("camera.offset.y")},
	                                               engine.config.get("camera.follow.x", 1), //bool not impl in cfg
	                                               engine.config.get("camera.follow.y", 0));

	//setup sample pipe segments.
	for(float xx = 17.0f; xx < 1000.f; xx += 4.f)
		createPipeSegment(xx);
}
void Controller::createFlappy() {
	flappy = engine.components.createEntity();

	PositionComponent* flappyPosition = engine.components.createComponent<PositionComponent>(flappy);
	SizeComponent* flappySize = engine.components.createComponent<SizeComponent>(flappy);
	MovementComponent* flappyMovement = engine.components.createComponent<MovementComponent>(flappy);
	GraphicsComponent* flappyAppearance = engine.components.createComponent<GraphicsComponent>(flappy);
	OrientationComponent* flappyOrientation = engine.components.createComponent<OrientationComponent>(flappy);

	flappyPosition->position.x = engine.config.get<float>("gameplay.flappy.position.x");
	flappyPosition->position.y = engine.config.get<float>("gameplay.flappy.position.y");
	flappySize->width = engine.config.get<float>("gameplay.flappy.size.width");
	flappySize->height = engine.config.get<float>("gameplay.flappy.size.height");

	flappyMovement->oldPosition = flappyPosition->position;
	flappyMovement->addPersistentForce({0, engine.config.get<float>("gameplay.flappy.forces.gravity")});
	flappyMovement->addTemporalForce({engine.config.get<float>("gameplay.flappy.forces.forwardConst"), 0});

	flappyAppearance->texture = &flappyTex;

	flappyOrientation->rotation = 40.f;
}

void Controller::createPipeSegment(float positionX) {
    float segmentWidth = engine.config.get<float>("gameplay.pipeSegmentWidth");
    float holeHeight = engine.config.get<float>("gameplay.hole.height");
    float holeUpperMargin = engine.config.get<float>("gameplay.hole.upperMargin");
    float holeLowerMargin = engine.config.get<float>("gameplay.hole.lowerMargin");
    float invisibleSkyHeight = engine.config.get<float>("gameplay.invisibleSkyHeight");
    float floorHeight = engine.config.get<float>("gameplay.floorHeight");

	sf::View view = window.getView();
	float screenUpperBoundary = view.getCenter().y - view.getSize().y / 2;
	float screenLowerBoundary = view.getCenter().y + view.getSize().y / 2;

	float holeMinPosition = screenUpperBoundary + holeUpperMargin;
	float holeMaxPosition = screenLowerBoundary - floorHeight - holeHeight - holeLowerMargin;
	std::uniform_real_distribution<float> distr{holeMinPosition, holeMaxPosition};
	static std::random_device dev;

	float holeYPosition = distr(dev);

	Entity hole = engine.components.createEntity();
	Entity upperPipe = engine.components.createEntity();
	Entity lowerPipe = engine.components.createEntity();
	pipes.push_back(lowerPipe);
	holes.push_back(hole);
	pipes.push_back(upperPipe);

	PositionComponent* holePosComponent = engine.components.createComponent<PositionComponent>(hole);
	PositionComponent* upperPipePosition = engine.components.createComponent<PositionComponent>(upperPipe);
	PositionComponent* lowerPipePosition = engine.components.createComponent<PositionComponent>(lowerPipe);
	upperPipePosition->position = {positionX, screenUpperBoundary - invisibleSkyHeight};
	lowerPipePosition->position = {positionX, holeYPosition + holeHeight};
	holePosComponent->position = {positionX, holeYPosition};

	SizeComponent* holeSize = engine.components.createComponent<SizeComponent>(hole);
	SizeComponent* upperPipeSize = engine.components.createComponent<SizeComponent>(upperPipe);
	SizeComponent* lowerPipeSize = engine.components.createComponent<SizeComponent>(lowerPipe);
	upperPipeSize->width = segmentWidth;
	lowerPipeSize->width = segmentWidth;
	holeSize->width = segmentWidth;
	upperPipeSize->height = holeYPosition - upperPipePosition->position.y;
	lowerPipeSize->height = (screenLowerBoundary - floorHeight) - lowerPipePosition->position.y;
	holeSize->height = holeHeight;

	GraphicsComponent* upperPipeAppearance = engine.components.createComponent<GraphicsComponent>(upperPipe);
	GraphicsComponent* lowerPipeAppearance = engine.components.createComponent<GraphicsComponent>(lowerPipe);
	upperPipeAppearance->texture = &pipeTex;
	lowerPipeAppearance->texture = &pipeTex;
}
