#pragma once
#include <ECS/task.h>

class VerletIntegrator : public Task {
public:
    VerletIntegrator(Engine& engine);
    void update() override;
};
