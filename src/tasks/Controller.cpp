#include "Controller.h"
#include "states/PlayState.h"

void Controller::update() {
    states.update();
}


Controller::Controller(Engine& engine) :
		Task(engine) {
    states.push(std::make_unique<PlayState>(engine));
}

