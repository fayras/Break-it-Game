#include "Paddle.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

Paddle::Paddle(const TextureHolder& textures)
  : Entity(0)
{}

unsigned int Paddle::getCategory() const {
  return Category::PADDLE;
}

void Paddle::updateCurrent(sf::Time dt, CommandQueue &commands) {
  Entity::updateCurrent(dt, commands);
}

void Paddle::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(sprite, states);
}
