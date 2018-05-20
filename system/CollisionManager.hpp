#ifndef BREAK_IT_COLLISIONMANAGER_HPP
#define BREAK_IT_COLLISIONMANAGER_HPP


#include <set>
#include "../nodes/SceneNode.hpp"

class CollisionManager {
  public:
    explicit CollisionManager(SceneNode& rootNode);

    std::set<SceneNode::Pair>& check(sf::Time dt);

  private:
    sf::FloatRect getBroadphasingRect(const Entity& entity, sf::Time dt);
    float sweptAABB(const Entity& b1, const Entity& b2, float& normalX, float& normalY, sf::Time dt);

    SceneNode& nodes;
    std::set<SceneNode::Pair> pairs;

    void doCollisionChecking(sf::Time time, Entity *entity, const std::list<Entity*>& entities,
                                 std::set<SceneNode::Pair>& pairs, int depth = 0);
};


#endif //BREAK_IT_COLLISIONMANAGER_HPP
