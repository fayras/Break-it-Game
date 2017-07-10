#include "SceneNode.hpp"

#include "../system/Command.hpp"
#include "../system/Utility.hpp"
#include <cassert>

SceneNode::SceneNode(Category::Type category)
  : children(), parent(nullptr), defaultCategory(category)
{}

void SceneNode::attachChild(SceneNode::Ptr child) {
  child->parent = this;
  children.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode &node) {
  auto found = std::find_if(children.begin(), children.end(), [&] (Ptr& p) {
    return p.get() == &node;
  });

  assert(found != children.end());

  Ptr result = std::move(*found);
  result->parent = nullptr;
  children.erase(found);
  return result;
}

sf::Vector2f SceneNode::getWorldPosition() const {
  return getWorldTransform() * sf::Vector2f();
}

sf::Transform SceneNode::getWorldTransform() const {
  sf::Transform transform = sf::Transform::Identity;

  for(const SceneNode* node = this; node != nullptr; node = node->parent) {
    transform = node->getTransform() * transform;
  }

  return transform;
}

void SceneNode::onCommand(const Command &command, sf::Time dt) {
  if(command.category & getCategory()) {
    command.action(*this, dt);
  }

  for(Ptr& child : children) {
    child->onCommand(command, dt);
  }
}

unsigned int SceneNode::getCategory() const {
  return defaultCategory;
}

void SceneNode::update(sf::Time dt, CommandQueue& commands) {
  updateCurrent(dt, commands);
  updateChildren(dt, commands);
}

void SceneNode::updateCurrent(sf::Time dt, CommandQueue& commands) {
  // do nothing by default
}

void SceneNode::updateChildren(sf::Time dt, CommandQueue& commands) {
  for(Ptr& child : children) {
    child->update(dt, commands);
  }
}

void SceneNode::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  states.transform *= getTransform();

  drawCurrent(target, states);
  drawChildren(target, states);
}

void SceneNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
  // do nothing by default
}

void SceneNode::drawChildren(sf::RenderTarget &target, sf::RenderStates states) const {
  for(const Ptr& child : children) {
    child->draw(target, states);
  }
}

sf::FloatRect SceneNode::getBoundingRect() const {
  return sf::FloatRect();
}

void SceneNode::checkSceneCollision(SceneNode &sceneGraph, std::set<SceneNode::Pair> &collisionPairs) {
  checkNodeCollision(sceneGraph, collisionPairs);

  for(Ptr& child : sceneGraph.children) {
    checkSceneCollision(*child, collisionPairs);
  }
}

void SceneNode::checkNodeCollision(SceneNode &node, std::set<SceneNode::Pair> &collisionPairs) {
  CollisionSide side = collision(*this, node);
  if (this != &node && side != CollisionSide::NONE && !isDestroyed() && !node.isDestroyed())
    collisionPairs.insert(std::make_tuple(this, &node, side));

  for(Ptr& child : children) {
    child->checkNodeCollision(node, collisionPairs);
  }
}

void SceneNode::removeWrecks() {
  // Remove all children which request so
  auto wreckfieldBegin = std::remove_if (children.begin(), children.end(), std::mem_fn(&SceneNode::isMarkedForRemoval));
  children.erase(wreckfieldBegin, children.end());

  // Call function recursively for all remaining children
  std::for_each(children.begin(), children.end(), std::mem_fn(&SceneNode::removeWrecks));
}

bool SceneNode::isMarkedForRemoval() const {
  // By default, remove node if entity is destroyed
  return isDestroyed();
}

bool SceneNode::isDestroyed() const {
  // By default, scene node needn't be removed
  return false;
}

CollisionSide collision(const SceneNode& lhs, const SceneNode& rhs) {
  sf::FloatRect r1 = lhs.getBoundingRect();
  sf::FloatRect r2 = rhs.getBoundingRect();
  float w = 0.5f * (r1.width + r2.width);
  float h = 0.5f * (r1.height + r2.height);
  float dx = (r1.left + r1.width * 0.5f) - (r2.left + r2.width * 0.5f);
  float dy = (r1.top + r1.height * 0.5f) - (r2.top + r2.height * 0.5f);

  if(std::abs(dx) <= w && std::abs(dy) <= h) {
    float wy = w * dy;
    float hx = h * dx;
    if(wy > hx) {
      if(wy > -hx) return CollisionSide::TOP;
      else return CollisionSide::LEFT;
    } else {
      if(wy > -hx) return CollisionSide::RIGHT;
      else CollisionSide::BOTTOM;
    }
  }

  return CollisionSide::NONE;
}

float distance(const SceneNode& lhs, const SceneNode& rhs) {
  return Vector::length(lhs.getWorldPosition() - rhs.getWorldPosition());
}
