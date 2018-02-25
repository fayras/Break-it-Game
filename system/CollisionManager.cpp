#include "CollisionManager.hpp"

CollisionManager::CollisionManager(SceneNode &rootNode)
  : nodes(rootNode)
{}

std::map<SceneNode::Pair, CollisionSide>& CollisionManager::check(sf::Time dt) {
  pairs.clear();

  std::list<Entity*> entities;
  nodes.getAllEntities(entities);

  // check collisions

  return pairs;
}
