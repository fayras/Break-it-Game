#include "Entity.hpp"
#include <cassert>

Entity::Entity(int hitpoints)
  : velocity(), hitpoints(hitpoints), maxHitpoints(hitpoints)
{}

void Entity::setVelocity(sf::Vector2f velocity) {
  this->velocity = velocity;
}

void Entity::setVelocity(float vx, float vy) {
  velocity.x = vx;
  velocity.y = vy;
}

void Entity::accelerate(sf::Vector2f velocity) {
  this->velocity += velocity;
}

void Entity::accelerate(float vx, float vy) {
  velocity.x += vx;
  velocity.y += vy;
}

sf::Vector2f Entity::getVelocity() const {
  return velocity;
}

int Entity::getHitpoints() const {
  return hitpoints;
}

int Entity::getMaxHitpoints() const {
  return maxHitpoints;
}

void Entity::heal(int points) {
  assert(points > 0);
  hitpoints = std::min(hitpoints + points, maxHitpoints);
}

void Entity::damage(int points) {
  assert(points > 0);
  hitpoints -= points;
}

void Entity::destroy() {
  hitpoints = 0;
}

void Entity::remove() {
  destroy();
}

bool Entity::isDestroyed() const {
  return hitpoints <= 0;
}

void Entity::updateCurrent(sf::Time dt, CommandQueue &commands) {
  if(recieveEvents) {
    move(velocity * dt.asSeconds());
  }
}

void Entity::setHP(int points) {
  hitpoints = points;
}
