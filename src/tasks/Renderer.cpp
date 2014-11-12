#include "Renderer.h"
#include "components/PositionComponent.h"
#include "components/SizeComponent.h"
#include "components/OrientationComponent.h"
#include "components/GraphicsComponent.h"

void Renderer::update() {
	window.clear(fillColor);

	std::vector<PositionComponent*> positions;
	std::vector<SizeComponent*> sizes;
	std::vector<GraphicsComponent*> graphics;
	engine.components.intersection(graphics, sizes, positions);

	int maxPlane = std::numeric_limits<int>::min();
	int minPlane = std::numeric_limits<int>::max();
	for(unsigned int i = 0; i < graphics.size(); i++) {
		maxPlane = (maxPlane < graphics[i]->plane ? graphics[i]->plane : maxPlane);
		minPlane = (minPlane > graphics[i]->plane ? graphics[i]->plane : minPlane);
	}

	for(int currentPlane = maxPlane; currentPlane >= minPlane; currentPlane--) {
		for(size_t i = 0; i < graphics.size(); i++) {
			if(graphics[i]->plane == currentPlane) {
				//prepare transform
				sf::Transform transform;
				auto* orientation = engine.components.getComponent<OrientationComponent>(graphics[i]->owner);
				if(orientation) {
					transform.rotate(orientation->rotation,
					                 positions[i]->position.x + sizes[i]->width/2,
					                 positions[i]->position.y + sizes[i]->height/2);
				}
				transform.translate(positions[i]->position);

				//prepare sprite vertices
				sf::Vertex vertices[4];
				vertices[0].position = {0, 0};
				vertices[1].position = {sizes[i]->width, 0};
				vertices[2].position = {sizes[i]->width, sizes[i]->height};
				vertices[3].position = {0, sizes[i]->height};

				vertices[0].texCoords = {0, 0};
				vertices[1].texCoords = {graphics[i]->texture->getSize().x, 0};
				vertices[2].texCoords = {graphics[i]->texture->getSize().x, graphics[i]->texture->getSize().y};
				vertices[3].texCoords = {0, graphics[i]->texture->getSize().y};

				//draw it
				sf::RenderStates state;
				state.texture = graphics[i]->texture;
				state.transform = transform;
				window.draw(vertices, 4, sf::Quads, state);
			}
		}
	}

	window.display();
}

Renderer::Renderer(Engine& engine, sf::RenderWindow& window) :
		Task(engine),
		window(window) {
	std::string winTitle = engine.config.get("tasks.renderer.windowTitle");
	unsigned int resX = engine.config.get("tasks.renderer.resolution.x", 1600u);
	unsigned int resY = engine.config.get("tasks.renderer.resolution.y", 900u);
	unsigned int clearColorR = engine.config.get<unsigned int>("tasks.renderer.fillColor.red");
	unsigned int clearColorG = engine.config.get<unsigned int>("tasks.renderer.fillColor.green");
	unsigned int clearColorB = engine.config.get<unsigned int>("tasks.renderer.fillColor.blue");
	bool fullscreen = engine.config.get("tasks.renderer.fullscreen", std::string("false")) == "true";

	if(fullscreen) {
		window.create(sf::VideoMode::getFullscreenModes()[0], winTitle, sf::Style::Fullscreen);
	}
	else {
		window.create(sf::VideoMode(resX, resY, 32), winTitle);
	}
	fillColor = sf::Color(clearColorR, clearColorG, clearColorB);

	float left = engine.config.get("tasks.renderer.initialView.left", 0.0f);
	float top = engine.config.get("tasks.renderer.initialView.top", 0.0f);
	float width = engine.config.get("tasks.renderer.initialView.width", (float)resX);
	float height = engine.config.get("tasks.renderer.initialView.height", (float)resY);
	window.setView(sf::View({left, top, width, height}));
}
