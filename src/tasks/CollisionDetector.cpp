#include "CollisionDetector.h"
#include <algorithm>                                                                 
#include "../components/MovementComponent.h"
#include "../events/CollisionEvent.h"

//class representing 2D projection of figure on axis. Invalid until contain at least one point
class Projection {
public:
    float min = std::numeric_limits<float>::max();
    float max = std::numeric_limits<float>::lowest();

    void addPoint(float projectedPoint) {
        min = projectedPoint < min ? projectedPoint : min; 
        max = projectedPoint > max ? projectedPoint : max; 
    }
};

void CollisionDetector::update() {
    auto collisionComponents = std::vector<CollisionComponent*>{};
    auto positions = std::vector<PositionComponent*>{};
    auto sizes = std::vector<SizeComponent*>{};
    engine.components.intersection(collisionComponents, positions, sizes);

    //check each body-body pair for collision
    for(auto i = 0u; i < collisionComponents.size(); i++) {
        for(auto j = i + 1; j < collisionComponents.size(); j++) {
            bool emitEvent = collisionComponents[i]->emitEvent || collisionComponents[j]->emitEvent;
            bool pushAnything = collisionComponents[i]->pushFromCollision || collisionComponents[j]->pushFromCollision;
            bool bothStatic = collisionComponents[i]->isStatic && collisionComponents[j]->isStatic;
            bool checkRequired = emitEvent || pushAnything || !bothStatic;
            if(!checkRequired) {
                continue;
            }

            auto firstBodyVertices = getVertices(*positions[i], *sizes[i]);
            auto secondBodyVertices = getVertices(*positions[j], *sizes[j]);
            auto MTV = calculateCollision(firstBodyVertices, secondBodyVertices);

            auto bodiesCollide = MTV.x != 0.f || MTV.y != 0.f;
            if(bodiesCollide) {
                auto firstBodyTranslation = sf::Vector2f{};
                auto secondBodyTranslation = sf::Vector2f{};

                if(collisionComponents[i]->pushFromCollision) {
                    //if both are to be pushed, push each by 1/2 of MTV in opposite directions.
                    if(collisionComponents[j]->pushFromCollision) {
                        firstBodyTranslation = {MTV.x / 2.f, MTV.y / 2.f};
                        secondBodyTranslation = {-MTV.x / 2.f, -MTV.y / 2.f};
                    } else {
                        firstBodyTranslation = MTV;
                    }
                } else if(collisionComponents[j]->pushFromCollision) {
                    secondBodyTranslation = -MTV;
                }
                positions[i]->position += firstBodyTranslation;
                positions[j]->position += secondBodyTranslation;

                if(collisionComponents[i]->emitEvent || collisionComponents[j]->emitEvent) {
                    CollisionEvent event;
                    event.firstBody = collisionComponents[i]->owner;
                    event.secondBody = collisionComponents[j]->owner;
                    event.firstBodyTranslation = firstBodyTranslation;
                    event.secondBodyTranslation = secondBodyTranslation;
                    event.minimumTranslationVector = MTV;
                    engine.events.push(std::move(event));
                }

                if(auto movementComponent = engine.components.getComponent<MovementComponent>(positions[i]->owner)) {
                    movementComponent->oldPosition += firstBodyTranslation;
                }
                if(auto movementComponent = engine.components.getComponent<MovementComponent>(positions[j]->owner)) {
                    movementComponent->oldPosition += secondBodyTranslation;
                }
            }
        }
    }
}

//returns MTV that you can apply to first. MTV == 0.f if they don't collide
sf::Vector2f CollisionDetector::calculateCollision(const std::array<sf::Vector2f, 4>& first,
                                                   const std::array<sf::Vector2f, 4>& second) {
    std::vector<sf::Vector2f> axes;
    appendAxes(axes, first);
    appendAxes(axes, second);

    auto minOverlap = std::numeric_limits<float>::max();
    auto minOverlapAxis = axes[0];
    auto minOverlapSecondProjection = Projection{};

    for(auto axis : axes) {
        //project bodies
        auto projection = Projection{};
        auto projection2 = Projection{};
        for(auto vertice : first) {
            projection.addPoint(vertice.x*axis.x + vertice.y*axis.y);
        }
        for(auto vertice : second) {
            projection2.addPoint(vertice.x*axis.x + vertice.y*axis.y);
        }

        auto currentOverlap = overlap(projection, projection2);

        //by SAT we know that if projections don't overlap on at least one axis, then they don't collide
        if(!currentOverlap) {
            return {0.f, 0.f};
        }

        if(currentOverlap < minOverlap) {
            minOverlap = currentOverlap;
            minOverlapAxis = axis;
            minOverlapSecondProjection = projection2;
        }
    }

    auto MTV = sf::Vector2f(minOverlapAxis.x * minOverlap, minOverlapAxis.y * minOverlap);

    //Ensure MTV have good sign by testing if applied on first body it really separates them
    auto testProjection = Projection{};
    for(auto vertice : first) {
        testProjection.addPoint((vertice.x + MTV.x)*minOverlapAxis.x + (vertice.y + MTV.y)*minOverlapAxis.y);
    }
    auto newOverlap = overlap(testProjection, minOverlapSecondProjection);
    if(newOverlap != 0.f) {
        MTV = -MTV;
    }

    return MTV;
}

float CollisionDetector::overlap(Projection first, Projection second) {
    return std::max(0.f, std::min(first.max, second.max) - std::max(first.min, second.min));
}

std::array<sf::Vector2f, 4> CollisionDetector::getVertices(const PositionComponent& pos, const SizeComponent& size) {
    auto transform = sf::Transform{};
    auto orientation = engine.components.getComponent<OrientationComponent>(pos.owner);
    if(orientation) {
        transform.rotate(orientation->rotation,
                         pos.position.x + size.width / 2,
                         pos.position.y + size.height / 2);
    }
    transform.translate(pos.position);

    return {transform.transformPoint({0, 0}),
            transform.transformPoint({size.width, 0}),
            transform.transformPoint({size.width, size.height}),
            transform.transformPoint({0, size.height})};
}

void CollisionDetector::appendAxes(std::vector<sf::Vector2f>& where, const std::array<sf::Vector2f, 4>& srcVertices) {
    sf::Vector2f firstSide = srcVertices[0] - srcVertices[1];
    sf::Vector2f secondSide = srcVertices[0] - srcVertices[3];

    //normalize axes
    auto len = (float)sqrt(firstSide.x*firstSide.x + firstSide.y*firstSide.y);
    firstSide = {firstSide.x / len, firstSide.y / len};

    len = (float)sqrt(secondSide.x*secondSide.x + secondSide.y*secondSide.y);
    secondSide = {secondSide.x / len, secondSide.y / len};

    where.push_back({-firstSide.y, firstSide.x});
    where.push_back({-secondSide.y, secondSide.x});
}

