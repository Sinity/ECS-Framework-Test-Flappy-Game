#include <ECS/engine.h>
#include "tasks/Controller.h"

int main() {
	auto engine = Engine{};

	if(!engine.init("config.cfg", "entities.cfg")) {
		engine.logger.fatal("main: engine initialization failed. Stopping execution.");
		return EXIT_FAILURE;
	}

	engine.tasks.addTask<Controller>();

	engine.run();

	engine.logger.info("Application executed, returning to system");
	return 0;
}

