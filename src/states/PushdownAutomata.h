#pragma once
#include <vector>
#include <memory>
#include "State.h"

class PushdownAutomata {
public:
    void update();
    void push(std::unique_ptr<State>&& state);
    void pop();
       
private:                            
    std::vector<std::unique_ptr<State>> states;
};
