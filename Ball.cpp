#include "Ball.hpp"
#include "system/ResourceHolder.hpp"
#include "system/Utility.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

Ball::Ball(const TextureHolder& textures)
  : Entity(1),
    sprite(textures.get(Textures::BALL))
{
  centerOrigin(sprite);
}

unsigned int Ball::getCategory() const {
  return Category::BALL;
}

sf::FloatRect Ball::getBoundingRect() const {
  return getWorldTransform().transformRect(sprite.getGlobalBounds());
}

void Ball::updateCurrent(sf::Time dt, CommandQueue &commands) {
  Entity::updateCurrent(dt, commands);
}

void Ball::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(sprite, states);
}
