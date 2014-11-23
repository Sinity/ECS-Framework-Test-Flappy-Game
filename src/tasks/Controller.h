#pragma once
#include <ECS/task.h>
#include <SFML/Graphics.hpp>
#include "states/PushdownAutomata.h"

class Controller : public Task {
public:
	Controller(Engine& engine);
	void update() override;

private:
    PushdownAutomata states;
};
