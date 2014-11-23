#pragma once

class State {
public:
    virtual void pause() {};
    virtual void resume() {};
    virtual void update() {};
    virtual ~State() {}
};
