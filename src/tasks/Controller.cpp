#include "Controller.h"
#include "../states/PlayState.h"

//tasks
#include "../tasks/SFMLInputProxy.h"
#include "../tasks/Renderer.h"
#include "../tasks/VerletIntegrator.h"
#include "../tasks/CollisionDetector.h"


void Controller::update() {
    states.update();
}


Controller::Controller(Engine& engine) :
		Task(engine) {

	//boot all core tasks
	engine.tasks.addTask<Renderer>(window);
	engine.tasks.addTask<SFMLInputProxy>(window);
	engine.tasks.addTask<VerletIntegrator>();
    engine.tasks.addTask<CollisionDetector>(window);

    states.push(std::make_unique<PlayState>(engine, window));
}

