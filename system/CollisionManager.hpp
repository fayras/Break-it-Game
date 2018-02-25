#ifndef BREAK_IT_COLLISIONMANAGER_HPP
#define BREAK_IT_COLLISIONMANAGER_HPP


#include "../nodes/SceneNode.hpp"

class CollisionManager {
  public:
    explicit CollisionManager(SceneNode& rootNode);

    std::map<SceneNode::Pair, CollisionSide>& check(sf::Time dt);

  private:
    SceneNode& nodes;
    std::map<SceneNode::Pair, CollisionSide> pairs;
};


#endif //BREAK_IT_COLLISIONMANAGER_HPP
