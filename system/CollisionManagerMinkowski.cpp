//
// Created by Dimi on 21.05.2018.
//

#include "CollisionManagerMinkowski.hpp"
#include "../entities/Entity.hpp"
#include "Utility.hpp"
#include <cmath>
#include <iostream>

CollisionManagerMinkowski::CollisionManagerMinkowski(SceneNode &rootNode)
    : nodes(rootNode)
{}

std::set<SceneNode::Pair> &CollisionManagerMinkowski::check(sf::Time dt) {
    pairs.clear();

    std::list<Entity*> entities;
    nodes.getAllEntities(entities);
    // std::list<Entity*>::iterator found;

    for(Entity* entity : entities) {
        if(entity->getVelocity().x != 0.f || entity->getVelocity().y != 0.f) {
            doCollisionChecking(dt, entity, entities, pairs);
        }
    }

    return pairs;
}

sf::FloatRect CollisionManagerMinkowski::getBroadphasingRect(const Entity &entity, sf::Time dt) {
    sf::FloatRect rect;
    auto b1 = entity.getBoundingRect();
    auto vel = entity.getVelocity();
    auto offset = vel * dt.asSeconds();
    auto b2 = b1;
    b2.left += offset.x;
    b2.top += offset.y;

    rect.left = std::min(b1.left, b2.left);
    rect.top = std::min(b1.top, b2.top);
    rect.width = std::max(b1.left + b1.width, b2.left + b2.width) - rect.left;
    rect.height = std::max(b1.top + b1.height, b2.top + b2.height) - rect.top;

    return rect;
}

void CollisionManagerMinkowski::doCollisionChecking(sf::Time time, Entity *entity, const std::list<Entity *> &entities,
                                                    std::set<SceneNode::Pair> &pairs, int depth) {
    if(depth > 100) {
        return;
    }

    auto bpBox = getBroadphasingRect(*entity, time);

    std::list<Entity*> found;
    std::copy_if(entities.begin(), entities.end(), std::back_inserter(found), [entity, &bpBox](const Entity *e) {
        return entity != e && bpBox.intersects(e->getBoundingRect());
    });

    if (found.empty()) {
        if(depth > 0) {
            Entity::Direction dir;
            dir.distance = 1.0;
            dir.deltaTime = time.asSeconds();
            dir.dir = sf::Vector2f(1, 1);
            entity->pushDirection(dir);
        }
        return;
    };

    for(auto const& otherEntity : found) {
        auto md = minkowskiDifference(entity->getBoundingRect(), otherEntity->getBoundingRect());
        if(md.left <= 0 && md.left + md.width >= 0 && md.top <= 0 && md.top + md.height >= 0 && depth == 0) {
            // throw std::runtime_error("Dies sollte eigentlich niemals pssieren");
            sf::Vector2f penetrationVector = closestPointOnBoundsToPoint(md, sf::Vector2f(0.f, 0.f));
            entity->move(penetrationVector);

            if(entity->getCollisionResponse() == Entity::CollisionResponse::DEFLECT && depth < 8) {
                doCollisionChecking(time, entity, entities, pairs, ++depth);
            }

            return;
        } else {
            // Berechne die ralative Geschwindigkeit zwischen beiden Rects
            auto relativeMotion = (entity->getVelocity() - otherEntity->getVelocity()) * time.asSeconds();

            CollisionSide side;
            float h = getRayIntersectionFraction(sf::Vector2f(0.f, 0.f), relativeMotion, md, side);

            if(h <= 1.f) {
                pairs.emplace(std::minmax(entity, otherEntity));

                Entity::Direction dir;
                dir.distance = h;
                dir.deltaTime = time.asSeconds();

                if (side == LEFT || side == RIGHT) dir.dir = sf::Vector2f(-1, 1);
                if (side == TOP || side == BOTTOM) dir.dir = sf::Vector2f(1, -1);
                if(entity->getCollisionResponse() == Entity::CollisionResponse::STOP) {
                    dir.dir = sf::Vector2f(0.f, 0.f);
                }

                entity->pushDirection(dir);

                if(entity->getCollisionResponse() == Entity::CollisionResponse::DEFLECT) {
                    sf::Time remaining = time - time * h;
                    doCollisionChecking(remaining, entity, entities, pairs, ++depth);
                }

                return;
            }
        }
    }
}

sf::FloatRect CollisionManagerMinkowski::minkowskiDifference(const sf::FloatRect &r1, const sf::FloatRect &r2) const {
    auto topLeft = sf::Vector2f(r2.left, r2.top) - sf::Vector2f(r1.left + r1.width, r1.top + r1.height);
    auto fullSize = sf::Vector2f(r2.width + r1.width, r2.height + r1.height);

    return { topLeft, fullSize };
}

float CollisionManagerMinkowski::getRayIntersectionFraction(sf::Vector2f origin, sf::Vector2f direction, const sf::FloatRect& rect, CollisionSide &side) {
    auto end = origin + direction;
    float minT, x;

    minT = getRayIntersectionFractionOfFirstRay(origin, end, sf::Vector2f(rect.left, rect.top), sf::Vector2f(rect.left, rect.top + rect.height));
    side = CollisionSide::LEFT;

    x = getRayIntersectionFractionOfFirstRay(origin, end, sf::Vector2f(rect.left, rect.top + rect.height), sf::Vector2f(rect.left + rect.width, rect.top + rect.height));
    if(x < minT) {
        minT = x;
        side = CollisionSide::BOTTOM;
    }

    x = getRayIntersectionFractionOfFirstRay(origin, end, sf::Vector2f(rect.left + rect.width, rect.top + rect.height), sf::Vector2f(rect.left + rect.width, rect.top));
    if(x < minT) {
        minT = x;
        side = CollisionSide::RIGHT;
    }

    x = getRayIntersectionFractionOfFirstRay(origin, end, sf::Vector2f(rect.left + rect.width, rect.top), sf::Vector2f(rect.left, rect.top));
    if(x < minT) {
        minT = x;
        side = CollisionSide::TOP;
    }

    return minT;
}

float CollisionManagerMinkowski::getRayIntersectionFractionOfFirstRay(sf::Vector2f originA, sf::Vector2f endA,
                                                                      sf::Vector2f originB, sf::Vector2f endB) {
    auto r = endA - originA;
    auto s = endB - originB;
    auto d = originB - originA;

    float numerator = d.x * r.y - d.y * r.x;
    float denominator = r.x * s.y - r.y * s.x;

    if(numerator == 0.f && denominator == 0.f) {
        return std::numeric_limits<float>::infinity();
    }

    if(denominator == 0.f) {
        return std::numeric_limits<float>::infinity();
    }

    float u = numerator / denominator;
    float t = (d.x * s.y - d.y * s.x) / denominator;
    if(t >= 0.f && t <= 1.f && u >= 0.f && u <= 1.f) {
        return t;
    }

    return std::numeric_limits<float>::infinity();
}

sf::Vector2f CollisionManagerMinkowski::closestPointOnBoundsToPoint(sf::FloatRect rect, sf::Vector2f point) {
    float minDist = std::abs(point.x - rect.left);
    sf::Vector2f boundsPoint{rect.left, point.y};

    if(std::abs(rect.left + rect.width - point.x) < minDist) {
        minDist = std::abs(rect.left + rect.width - point.x);
        boundsPoint.x = rect.left + rect.width;
        boundsPoint.y = point.y;
    }

    if(std::abs(rect.top + rect.height - point.y) < minDist) {
        minDist = std::abs(rect.top + rect.height - point.y);
        boundsPoint.x = point.x;
        boundsPoint.y = rect.top + rect.height;
    }

    if(std::abs(rect.top - point.y) < minDist) {
        minDist = std::abs(rect.top - point.y);
        boundsPoint.x = point.x;
        boundsPoint.y = rect.top;
    }

    return boundsPoint;
}
