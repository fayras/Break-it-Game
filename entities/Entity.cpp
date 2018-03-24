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
  sf::Vector2f vel = velocity;
  for(const Direction& d : directions) {
    vel.x *= d.dir.x;
    vel.y *= d.dir.y;
  }
  return vel;
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
  if(!isStatic && recieveEvents) {
    if(directions.empty()) {
      move(velocity * dt.asSeconds());
    } else {
      for(const Direction& d : directions) {
        setVelocity(velocity.x * d.dir.x, velocity.y * d.dir.y);
        move(velocity * (d.distance * d.deltaTime));
      }
      directions.clear();
    }
  }
}

void Entity::setHP(int points) {
  hitpoints = points;
}

void Entity::pushDirection(const Entity::Direction dir) {
  directions.emplace_back(dir);
}

sf::Vector2f Entity::getPosition() const {
  sf::Vector2f pos = Transformable::getPosition();
  sf::Vector2f vel = velocity;
  for(const Direction& d : directions) {
    vel.x *= d.dir.x;
    vel.y *= d.dir.y;
    pos += vel * d.distance;
  }
  return pos;
}
