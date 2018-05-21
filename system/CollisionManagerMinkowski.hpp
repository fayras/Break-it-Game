//
// Created by Dimi on 21.05.2018.
//

#ifndef BREAK_IT_COLLISIONMANAGERMINKOWSKI_HPP
#define BREAK_IT_COLLISIONMANAGERMINKOWSKI_HPP

#include <set>
#include "../nodes/SceneNode.hpp"

class CollisionManagerMinkowski {
public:
    explicit CollisionManagerMinkowski(SceneNode& rootNode);

    std::set<SceneNode::Pair>& check(sf::Time dt);

private:
    enum CollisionSide {
        UNKNOWN,
        LEFT,
        RIGHT,
        TOP,
        BOTTOM
    };

    sf::FloatRect getBroadphasingRect(const Entity& entity, sf::Time dt);
    sf::FloatRect minkowskiDifference(const sf::FloatRect& r1, const sf::FloatRect& r2) const;
    float getRayIntersectionFraction(sf::Vector2f origin, sf::Vector2f direction, const sf::FloatRect& rect, CollisionSide& side);
    float getRayIntersectionFractionOfFirstRay(sf::Vector2f originA, sf::Vector2f endA, sf::Vector2f originB, sf::Vector2f endB);
    sf::Vector2f closestPointOnBoundsToPoint(sf::FloatRect rect, sf::Vector2f point);

    void doCollisionChecking(sf::Time time, Entity *entity, const std::list<Entity*>& entities,
                             std::set<SceneNode::Pair>& pairs, int depth = 0);

    SceneNode& nodes;
    std::set<SceneNode::Pair> pairs;

};


#endif //BREAK_IT_COLLISIONMANAGERMINKOWSKI_HPP
