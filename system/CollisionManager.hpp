#ifndef BREAK_IT_COLLISIONMANAGER_HPP
#define BREAK_IT_COLLISIONMANAGER_HPP


#include "../nodes/SceneNode.hpp"

class CollisionManager {
  public:
    explicit CollisionManager(SceneNode& rootNode);

    void update(sf::Time dt);

  private:
    SceneNode& nodes;
};


#endif //BREAK_IT_COLLISIONMANAGER_HPP
