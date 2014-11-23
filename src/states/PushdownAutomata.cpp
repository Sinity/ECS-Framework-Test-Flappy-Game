#include "PushdownAutomata.h"

void PushdownAutomata::update() {
    if(!states.empty()) {
        states.back()->update();
    }
}

void PushdownAutomata::push(std::unique_ptr<State>&& state) {
    if(!states.empty()) {
        states.back()->pause();
    }
    states.push_back(std::move(state));
}

void PushdownAutomata::pop() {
    if(!states.empty()) {
        states.pop_back();
    }

    if(!states.empty()) {
        states.back()->resume();
    }
}

