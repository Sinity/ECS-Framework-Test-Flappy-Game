#pragma once
#include <ECS/component.h>
#include <SFML/Graphics.hpp>

COMPONENT(GraphicsComponent) {
	void init(ArgsMap args) {
		plane = boost::lexical_cast<int>(args["plane"]);
	}

	int plane = 0;  //Planes are drawn from high to low. INT_MAX is drawn first, INT_MIN last.
                    //Drawing order within a plane is undefined.
                    
    std::shared_ptr<sf::Texture> texture = nullptr; //for sprite
};
