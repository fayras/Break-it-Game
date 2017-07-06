#include "SceneNode.hpp"

#include "../system/Command.hpp"
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
