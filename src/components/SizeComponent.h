#pragma once
#include <ECS/component.h>
#include <SFML/System.hpp>

COMPONENT(SizeComponent) {
	void init(ArgsMap args) {
		width = boost::lexical_cast<float>(args["width"]);
		height = boost::lexical_cast<float>(args["height"]);
	}

	float width = 0.f;
	float height = 0.f;
};

