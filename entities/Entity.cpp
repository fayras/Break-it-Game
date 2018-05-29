#include "Entity.hpp"
#include <cassert>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

Entity::Entity(int hitpoints)
  : velocity(), hitpoints(hitpoints), maxHitpoints(hitpoints)
{}

void Entity::setVelocity(sf::Vector2f velocity) {
  this->velocity = velocity;
  directions.clear();
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
        move(velocity * (d.distance * d.deltaTime));
        setVelocity(velocity.x * d.dir.x, velocity.y * d.dir.y);
      }
      if(showDebugInfo) {
        doneDirections.clear();
        doneDirections = directions;
      }
      directions.clear();
    }
  }
}

void Entity::setHP(int points) {
  hitpoints = points;
}

void Entity::pushDirection(const Entity::Direction dir) {
  if(!recieveEvents) {
    return;
  }

  directions.emplace_back(dir);
}

sf::Vector2f Entity::getPosition() const {
  sf::Vector2f pos = Transformable::getPosition();
  sf::Vector2f vel = velocity;
  for(const Direction& d : directions) {
    pos += vel * d.distance * d.deltaTime;
    vel.x *= d.dir.x;
    vel.y *= d.dir.y;
  }
  return pos;
}

void Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  SceneNode::draw(target, states);

  if(showDebugInfo &&!doneDirections.empty()) {
    std::size_t count = doneDirections.size() * 2;
    sf::VertexArray lines(sf::Lines, count);
    auto pos = Transformable::getPosition();
    auto vel = getVelocity();
    std::size_t index = 0;

    for(const Direction& d : doneDirections) {
      lines[index++] = sf::Vertex(pos);
      pos += vel * d.distance * d.deltaTime;
      lines[index++] = sf::Vertex(pos);
      vel.x *= d.dir.x;
      vel.y *= d.dir.y;
    }

    target.draw(lines, states);
  }
}

Entity::CollisionResponse Entity::getCollisionResponse() const {
  return CollisionResponse::DEFLECT;
}
