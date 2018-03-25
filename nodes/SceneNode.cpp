#include "SceneNode.hpp"

#include "../system/Command.hpp"
#include "../system/Utility.hpp"
#include "../entities/Entity.hpp"
#include <cassert>
#include <cmath>

SceneNode::SceneNode(Category::Type category)
  : parent(nullptr), defaultCategory(category)
{}

void SceneNode::attachChild(SceneNode::Ptr child) {
  child->parent = this;
  pendingChildren.push_back(std::move(child));
}

void SceneNode::attachChildNow(SceneNode::Ptr child) {
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

void SceneNode::setCategory(const Category::Type category) {
  defaultCategory = category;
}

unsigned int SceneNode::getCategory() const {
  return defaultCategory;
}

void SceneNode::update(sf::Time dt, CommandQueue& commands) {
  if(!pendingChildren.empty()) {
    children.merge(pendingChildren);
  }
  Tweenable::update(dt);
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
  return {};
}

void SceneNode::checkSceneCollision(SceneNode &sceneGraph, std::map<SceneNode::Pair, CollisionSide> &collisionPairs) {
  checkNodeCollision(sceneGraph, collisionPairs);

  for(Ptr& child : sceneGraph.children) {
    checkSceneCollision(*child, collisionPairs);
  }
}

void SceneNode::checkNodeCollision(SceneNode &node, std::map<SceneNode::Pair, CollisionSide> &collisionPairs) {
  CollisionSide side;
  if (
      !(this->isStatic && node.isStatic) &&
      this != &node &&
      !isDestroyed() &&
      !node.isDestroyed() &&
      (side = collision(*this, node))
      ) {
    collisionPairs.try_emplace(std::minmax(this, &node), side);
  }

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

bool SceneNode::hasChildren() const {
  return !children.empty();
}

bool SceneNode::containsNode(Category::Type type) const {
  for(const auto& node : children) {
    if(node->getCategory() & type) return true;
  }

  return false;
}

void SceneNode::getAllEntities(std::list<Entity*> &entities) {
  auto cast = dynamic_cast<Entity*>(this);
  if(cast != nullptr) {
    entities.push_back(cast);
  }

  for(Ptr& child : children) {
    child->getAllEntities(entities);
  }
}

void SceneNode::clearChildren() {
  children.clear();
}

CollisionSide collision(const SceneNode &lhs, const SceneNode &rhs) {
  sf::FloatRect r1 = lhs.getBoundingRect();
  sf::FloatRect r2 = rhs.getBoundingRect();

  if(!r1.intersects(r2)) {
    return CollisionSide::NONE;
  }

  float overlapLeft = r1.left + r1.width - r2.left;
  float overlapRight = r2.left + r2.width - r1.left;
  float overlapTop = r1.top + r1.height - r2.top;
  float overlapBottom = r2.top + r2.height - r1.top;

  bool ballFromLeft = std::abs(overlapLeft) < std::abs(overlapRight);
  bool ballFromTop = std::abs(overlapTop) < std::abs(overlapBottom);

  float minOverlapX = ballFromLeft ? overlapLeft : overlapRight;
  float minOverlapY = ballFromTop ? overlapTop : overlapBottom;

  if(std::abs(minOverlapX) < std::abs(minOverlapY)) {
    return ballFromLeft ? CollisionSide::LEFT : CollisionSide::RIGHT;
  } else {
    return ballFromTop ? CollisionSide::TOP : CollisionSide::BOTTOM;
  }
}

float distance(const SceneNode& lhs, const SceneNode& rhs) {
  return Vector::length(lhs.getWorldPosition() - rhs.getWorldPosition());
}
