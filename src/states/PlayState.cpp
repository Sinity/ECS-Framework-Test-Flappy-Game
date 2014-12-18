#include "PlayState.h"
#include <algorithm>

//events
#include "events/system/ApplicationClosed.h"
#include "events/system/MouseButtonPressed.h"
#include "events/CollisionEvent.h"

//components
#include "components/PositionComponent.h"
#include "components/SizeComponent.h"
#include "components/OrientationComponent.h"
#include "components/MovementComponent.h"
#include "components/CollisionComponent.h"
#include "components/GraphicsComponent.h"
#include "components/GUITextComponent.h"

//tasks
#include "tasks/SFMLInputProxy.h"
#include "tasks/Renderer.h"
#include "tasks/AttachedCameraController.h"
#include "tasks/VerletIntegrator.h"
#include "tasks/CollisionDetector.h"

PlayState::PlayState(Engine& engine) :
    engine(engine) {
	engine.events.connect<ApplicationClosedEvent>(*this);
	engine.events.connect<MouseButtonPressed>(*this);
    engine.events.connect<CollisionEvent>(*this);

	//boot all core tasks
	engine.tasks.addTask<Renderer>(window);
	engine.tasks.addTask<SFMLInputProxy>(window);
	engine.tasks.addTask<VerletIntegrator>();
    engine.tasks.addTask<CollisionDetector>(window);

	//load textures
	pipeTex->loadFromFile(engine.config.get("gameplay.files.pipeTexture"));
	flappyTex->loadFromFile(engine.config.get("gameplay.files.flappyTexture"));

	createFlappy();
	engine.tasks.addTask<AttachedCameraController>(window, flappy,
	                                               sf::Vector2f{engine.config.get<float>("camera.offset.x"),
			                                                    engine.config.get<float>("camera.offset.y")},
	                                               engine.config.get("camera.follow.x", 1),
	                                               engine.config.get("camera.follow.y", 0));

	//setup sample pipe segments.
    pipeSpacing = engine.config.get<float>("gameplay.spaceBetweenPipes");
    auto initEmptySpace = engine.config.get<float>("gameplay.initialEmptySpace");
    auto screenBoundary = window.getView().getSize().x;
	for(auto pos = initEmptySpace; pos <= initEmptySpace + screenBoundary; pos += pipeSpacing) {
		createPipeSegment(pos);
        lastPipePosition = pos;
    }
    createScoreCounter();
}

void PlayState::receive(ApplicationClosedEvent& closeRequest) {
	(void)closeRequest;
	engine.logger.info("Application close request received in Controller, stopping engine...");
	engine.stop();
}

void PlayState::receive(MouseButtonPressed& buttonPress) {
	if(buttonPress.button.button == 0) {
		auto flappyMovement = engine.components.getComponent<MovementComponent>(flappy);
		auto flappyPosition = engine.components.getComponent<PositionComponent>(flappy);
		flappyMovement->oldPosition.y = flappyPosition->position.y; //flappy universe logic here
		flappyMovement->addTemporalForce({0, engine.config.get<float>("gameplay.flappy.forces.lift")});
	}
}

void PlayState::receive(CollisionEvent& collision) {
    Entity colliding = collision.firstBody == flappy ? collision.secondBody : collision.firstBody;
    if(colliding != collision.secondBody) {
        collision.minimumTranslationVector = -collision.minimumTranslationVector;
    }

    //colliding with score trigger
    if(std::find(begin(holes), end(holes), colliding) != end(holes)) { 
        if(currentlyCollidingHole != colliding) { 
            currentlyCollidingHole = colliding;
            score++; 
            auto text = engine.components.getComponent<GUITextComponent>(scoreCounter); 
            if(text) { 
                text->text.setString(std::to_string(score)); 
            } 
            //update pipes; create new and remove old(if can't be seen)
            createPipeSegment(lastPipePosition + pipeSpacing);
            lastPipePosition += pipeSpacing;
            auto leftScreenBoundary = window.getView().getCenter().x - window.getView().getSize().x / 2;
            auto oldestPipePosition = engine.components.getComponent<PositionComponent>(pipes[0])->position.x;
            if(oldestPipePosition < leftScreenBoundary) {
                engine.components.deleteEntity(pipes[0]);    
                engine.components.deleteEntity(pipes[1]);    
                pipes.erase(begin(pipes));
                pipes.erase(begin(pipes));
            }
        }     
    }
    //colliding with obstacle
    else { 
        auto flappyPos = engine.components.getComponent<PositionComponent>(flappy);
        auto flappyMovement = engine.components.getComponent<MovementComponent>(flappy);

        if(flappyPos) {
            flappyPos->position += collision.minimumTranslationVector;
        }
        if(flappyMovement) {
            flappyMovement->oldPosition.y = flappyPos->position.y;
            flappyMovement->oldPosition.x = flappyPos->position.x;
        }
    } 
}

void PlayState::createFlappy() {
	flappy = engine.components.createEntity();

	auto flappyPosition = engine.components.createComponent<PositionComponent>(flappy);
	auto flappySize = engine.components.createComponent<SizeComponent>(flappy);
	auto flappyMovement = engine.components.createComponent<MovementComponent>(flappy);
    auto flappyCollision = engine.components.createComponent<CollisionComponent>(flappy);
	auto flappyAppearance = engine.components.createComponent<GraphicsComponent>(flappy);
	engine.components.createComponent<OrientationComponent>(flappy);

	flappyPosition->position.x = engine.config.get<float>("gameplay.flappy.position.x");
	flappyPosition->position.y = engine.config.get<float>("gameplay.flappy.position.y");
	flappySize->width = engine.config.get<float>("gameplay.flappy.size.width");
	flappySize->height = engine.config.get<float>("gameplay.flappy.size.height");

	flappyMovement->oldPosition = flappyPosition->position;
	flappyMovement->addPersistentForce({0, engine.config.get<float>("gameplay.flappy.forces.gravity")});
	flappyMovement->addTemporalForce({engine.config.get<float>("gameplay.flappy.forces.forwardConst"), 0});

    flappyCollision->emitEvent = true;
	flappyAppearance->texture = flappyTex;
}

void PlayState::createPipeSegment(float positionX) {
    //get parameters from configuration
    auto segmentWidth = engine.config.get<float>("gameplay.pipeSegmentWidth");
    auto holeHeight = engine.config.get<float>("gameplay.hole.height");
    auto holeUpperMargin = engine.config.get<float>("gameplay.hole.upperMargin");
    auto holeLowerMargin = engine.config.get<float>("gameplay.hole.lowerMargin");
    auto invisibleSkyHeight = engine.config.get<float>("gameplay.invisibleSkyHeight");
    auto floorHeight = engine.config.get<float>("gameplay.floorHeight");

    //calculate screen boundaries
	sf::View view = window.getView();
	auto screenUpperBoundary = view.getCenter().y - view.getSize().y / 2;
	auto screenLowerBoundary = view.getCenter().y + view.getSize().y / 2;

    //roll random hole position
	auto holeMinPosition = screenUpperBoundary + holeUpperMargin;
	auto holeMaxPosition = screenLowerBoundary - floorHeight - holeHeight - holeLowerMargin;
	auto holePositionsDistribution = std::uniform_real_distribution<float>{holeMinPosition, holeMaxPosition};
	static std::random_device randomDevice;
	auto holeYPosition = holePositionsDistribution(randomDevice);

    //create entities building pipe segment
	auto hole = engine.components.createEntity();
	auto upperPipe = engine.components.createEntity();
	auto lowerPipe = engine.components.createEntity();
	holes.push_back(hole);
    pipes.push_back(upperPipe);
    pipes.push_back(lowerPipe);

    //setup positions of pipe segment elements
	auto holePosComponent = engine.components.createComponent<PositionComponent>(hole);
	auto upperPipePosition = engine.components.createComponent<PositionComponent>(upperPipe);
	auto lowerPipePosition = engine.components.createComponent<PositionComponent>(lowerPipe);
	holePosComponent->position = {positionX, holeYPosition};
	upperPipePosition->position = {positionX, screenUpperBoundary - invisibleSkyHeight};
	lowerPipePosition->position = {positionX, holeYPosition + holeHeight};

    //calculate sizes of pipe segment elements
	auto holeSize = engine.components.createComponent<SizeComponent>(hole);
	auto upperPipeSize = engine.components.createComponent<SizeComponent>(upperPipe);
	auto lowerPipeSize = engine.components.createComponent<SizeComponent>(lowerPipe);
	holeSize->width = segmentWidth;
	upperPipeSize->width = segmentWidth;
	lowerPipeSize->width = segmentWidth;
	holeSize->height = holeHeight;
	upperPipeSize->height = holeYPosition - upperPipePosition->position.y;
	lowerPipeSize->height = (screenLowerBoundary - floorHeight) - lowerPipePosition->position.y;

    //create collision components
    engine.components.createComponent<CollisionComponent>(hole);
    engine.components.createComponent<CollisionComponent>(upperPipe);
    engine.components.createComponent<CollisionComponent>(lowerPipe);

    //bind textures to visible elements of pipe segment
	auto upperPipeAppearance = engine.components.createComponent<GraphicsComponent>(upperPipe);
	auto lowerPipeAppearance = engine.components.createComponent<GraphicsComponent>(lowerPipe);
	upperPipeAppearance->texture = pipeTex;
	lowerPipeAppearance->texture = pipeTex;
}

void PlayState::createScoreCounter() {
    scoreCounter = engine.components.createEntity();
    auto counterAppearance = engine.components.createComponent<GUITextComponent>(scoreCounter);
    auto counterPosition = engine.components.createComponent<PositionComponent>(scoreCounter);

    counterFont.loadFromFile(engine.config.get("gameplay.files.font"));
    counterAppearance->text.setFont(counterFont);
    counterAppearance->text.setColor(sf::Color::White);
    counterAppearance->text.setCharacterSize(engine.config.get<int>("gameplay.score.fontSize"));
    counterAppearance->text.setString("0");

    counterPosition->position.x = window.getDefaultView().getCenter().x;
}

