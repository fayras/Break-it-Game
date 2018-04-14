#include "Ball.hpp"
#include "../system/Utility.hpp"
#include "../nodes/ParticleNode.hpp"
#include "../nodes/EmitterNode.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <cmath>

Ball::Ball(const sf::Texture& texture)
  : Entity(1),
    sprite(texture)
{
  centerOrigin(sprite);
  isStatic = false;
  defaultCategory = Category::BALL;

  std::unique_ptr<EmitterNode> trail(new EmitterNode(Particle::Propellant, 0.f));
  trail->setPosition(0, 0);
  this->trail = trail.get();
  attachChild(std::move(trail));
}

sf::FloatRect Ball::getBoundingRect() const {
  return getWorldTransform().transformRect(sprite.getGlobalBounds());
}

void Ball::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(sprite, states);
}

void Ball::reset(sf::Vector2f pos) {
  setPosition(pos.x, pos.y - getBoundingRect().height - 20);
}

void Ball::duplicate() {
  float radians = toRadian(30.f);
  auto ball = std::make_unique<Ball>(*sprite.getTexture());
  ball->setPosition(getPosition());
  ball->setVelocity(getVelocity());
  ball->showDebugInfo = showDebugInfo;

  rotateBy(radians);
  ball->rotateBy(-radians);

  parent->attachChild(std::move(ball));
}

void Ball::rotateBy(float angle) {
  float cosA = std::cos(angle);
  float sinA = std::sin(angle);
  setVelocity(cosA * getVelocity().x - sinA * getVelocity().y, sinA * getVelocity().x + cosA * getVelocity().y);
}

void Ball::updateChildren(sf::Time dt, CommandQueue &commands) {
  if(recieveEvents) SceneNode::updateChildren(dt, commands);
}

void Ball::updateCurrent(sf::Time dt, CommandQueue &commands) {
  Entity::updateCurrent(dt, commands);
  const float speed = Vector::length(getVelocity()) / SPEED * 100.f;
  trail->setEmissionRate(speed);
}
