#include "Renderer.h"
#include "../components/PositionComponent.h"
#include "../components/SizeComponent.h"
#include "../components/OrientationComponent.h"
#include "../components/GraphicsComponent.h"
#include "../components/GUITextComponent.h"

void Renderer::update() {
    window.clear(fillColor);
    renderSprites();
    renderText();
    window.display();
}

void Renderer::renderSprites() {
    auto graphics = std::vector<GraphicsComponent*>{};
    auto sizes = std::vector<SizeComponent*>{};
    auto positions = std::vector<PositionComponent*>{};
    engine.components.intersection(graphics, sizes, positions);

    //calculate planes range
    auto maxPlane = std::numeric_limits<int>::min();
    auto minPlane = std::numeric_limits<int>::max();
    for(auto gfx : graphics) {
        maxPlane = (maxPlane < gfx->plane ? gfx->plane : maxPlane);
        minPlane = (minPlane > gfx->plane ? gfx->plane : minPlane);
    }

    //This stupid slow algorithm is only temporal, until I get some API that have z-ordering natively
    for(auto currentPlane = maxPlane; currentPlane >= minPlane; currentPlane--) {
        for(auto i = 0u; i < graphics.size(); i++) {
            if(graphics[i]->plane == currentPlane) {
                //prepare transform
                auto transform = sf::Transform{};
                auto orientation = engine.components.getComponent<OrientationComponent>(graphics[i]->owner);
                if(orientation) {
                    transform.rotate(orientation->rotation,
                            positions[i]->position.x + sizes[i]->width/2,
                            positions[i]->position.y + sizes[i]->height/2);
                }
                transform.translate(positions[i]->position);

                //prepare sprite vertices
                sf::Vertex vertices[4];

                //positions are in local(object) coordinate space, it's translated to world space later
                vertices[0].position = {0, 0};
                vertices[1].position = {sizes[i]->width, 0};
                vertices[2].position = {sizes[i]->width, sizes[i]->height};
                vertices[3].position = {0, sizes[i]->height};

                //always use whole texture. I will add support for mapping only part of it, when I will need it
                vertices[0].texCoords = {0, 0};
                vertices[1].texCoords = {(float)graphics[i]->texture->getSize().x, 0};
                vertices[2].texCoords = {(float)graphics[i]->texture->getSize().x, 
                                        (float)graphics[i]->texture->getSize().y};
                vertices[3].texCoords = {0, (float)graphics[i]->texture->getSize().y};

                //draw it
                auto state = sf::RenderStates{};
                state.texture = graphics[i]->texture.get();
                state.transform = transform;
                window.draw(vertices, 4, sf::Quads, state);
            }
        }
    }
}

void Renderer::renderText() {
    auto currView = window.getView();
    window.setView(window.getDefaultView());

    auto texts = std::vector<GUITextComponent*>{};
    auto positions = std::vector<PositionComponent*>{};
    engine.components.intersection(texts, positions);

    for(auto i = 0u; i < texts.size(); i++) {
        texts[i]->text.setPosition(positions[i]->position);
        auto rotation = engine.components.getComponent<OrientationComponent>(texts[i]->owner);
        if(rotation) {
            texts[i]->text.setRotation(rotation->rotation);
        }
        window.draw(texts[i]->text);
    }

    window.setView(currView);
}

Renderer::Renderer(Engine& engine, sf::RenderWindow& window) :
    Task(engine),
    window(window) {
        auto resX = engine.config.get("tasks.renderer.resolution.x", 1600u);
        auto resY = engine.config.get("tasks.renderer.resolution.y", 900u);
        auto winTitle = engine.config.get("tasks.renderer.windowTitle");
        auto fullscreen = engine.config.get("tasks.renderer.fullscreen", std::string("false")) == "true";
        if(fullscreen) {
            window.create(sf::VideoMode::getFullscreenModes()[0], winTitle, sf::Style::Fullscreen);
        }
        else {
            window.create(sf::VideoMode(resX, resY, 32), winTitle);
        }

        fillColor.r = engine.config.get("tasks.renderer.fillColor.red", 0u);
        fillColor.g = engine.config.get("tasks.renderer.fillColor.green", 0u);
        fillColor.b = engine.config.get("tasks.renderer.fillColor.blue", 0u);

        auto left = engine.config.get("tasks.renderer.initialView.left", 0.0f);
        auto top = engine.config.get("tasks.renderer.initialView.top", 0.0f);
        auto width = engine.config.get("tasks.renderer.initialView.width", (float)resX);
        auto height = engine.config.get("tasks.renderer.initialView.height", (float)resY);
        window.setView(sf::View({left, top, width, height}));
    }
