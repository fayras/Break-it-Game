#include "Ball.hpp"
#include "../system/ResourceHolder.hpp"
#include "../system/Utility.hpp"
#include "../nodes/ParticleNode.hpp"
#include "../nodes/EmitterNode.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

Ball::Ball(const TextureHolder& textures)
  : Entity(1),
    sprite(textures.get(Textures::BALL))
{
  centerOrigin(sprite);
  std::unique_ptr<EmitterNode> trail(new EmitterNode(Particle::Propellant));
  trail->setPosition(0, 0);
  attachChild(std::move(trail));
}

unsigned int Ball::getCategory() const {
  return Category::BALL;
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
