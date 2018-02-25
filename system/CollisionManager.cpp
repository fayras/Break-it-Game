#include "CollisionManager.hpp"

CollisionManager::CollisionManager(SceneNode &rootNode)
  : nodes(rootNode)
{}

std::map<SceneNode::Pair, CollisionSide>& CollisionManager::check(sf::Time dt) {
  pairs.clear();

  // check collisions

  return pairs;
}
