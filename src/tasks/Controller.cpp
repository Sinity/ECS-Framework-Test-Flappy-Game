#include "Controller.h"
#include "Renderer.h"
#include "SFMLInputProxy.h"
#include "InputEcho.h"
#include "VerletIntegrator.h"
#include "CameraMouseController.h"
#include "events/system/ApplicationClosed.h"
#include "events/system/MouseButtonPressed.h"
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

void Controller::receive(MouseButtonPressed& buttonPress) {
	if(buttonPress.button.button == 0) {
		MovementComponent* flappyMovement = engine.components.getComponent<MovementComponent>(flappy);
		flappyMovement->addTemporalForce({0, engine.config.get("gameplay.flappy.forces.lift", -1000.f)});
	}
}

Controller::Controller(Engine& engine) :
		Task(engine) {
	engine.events.connect<ApplicationClosedEvent>(*this);
	engine.events.connect<MouseButtonPressed>(*this);

	engine.tasks.addTask<Renderer>(window);
	engine.tasks.addTask<SFMLInputProxy>(window);
	engine.tasks.addTask<InputEcho>();
	engine.tasks.addTask<VerletIntegrator>();
	engine.tasks.addTask<CameraMouseController>(window);

	//setup flappy graphics
	flappyTex.loadFromFile(engine.config.get("gameplay.files.flappyTexture"));
	std::shared_ptr<sf::Sprite> flappySprite = std::make_shared<sf::Sprite>();
	flappySprite->setTexture(flappyTex);
	flappySprite->setScale(engine.config.get("gameplay.flappy.size.x", 1.f) / flappySprite->getLocalBounds().width,
	                       engine.config.get("gameplay.flappy.size.y", 1.f) / flappySprite->getLocalBounds().height);


	//setup flappy
	flappy = engine.components.createEntity();
	PositionComponent* flappyPosition = engine.components.createComponent<PositionComponent>(flappy);
	MovementComponent* flappyMovement = engine.components.createComponent<MovementComponent>(flappy);
	RenderingComponent* flappyAppearance = engine.components.createComponent<RenderingComponent>(flappy);

	flappyPosition->position = {engine.config.get("gameplay.flappy.position.x", 0.f),
								engine.config.get("gameplay.flappy.position.y", 0.f)};

	flappyMovement->oldPosition = flappyPosition->position;
	flappyMovement->addPersistentForce({0, engine.config.get("gameplay.flappy.forces.gravity", 50.f)});
	flappyMovement->addTemporalForce({engine.config.get("gameplay.flappy.forces.forwardConst", 300.f), 0});

	flappyAppearance->drawablesList.push_back(std::move(flappySprite));
}
