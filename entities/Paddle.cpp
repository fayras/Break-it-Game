#include "Paddle.hpp"
#include "../system/ResourceHolder.hpp"
#include "../system/Utility.hpp"
#include "../Life.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

const int SPEED = 500;

Paddle::Paddle(const TextureHolder& textures)
  : Entity(3),
    sprite(textures.get(Textures::PADDLE))
{
  centerOrigin(sprite);
  isStatic = false;

  auto lives = std::make_unique<Life>(textures.get(Textures::LIFE));
  lives->setPosition(10, 10);
  attachChild(std::move(lives));
}

unsigned int Paddle::getCategory() const {
  return Category::PADDLE;
}

void Paddle::updateCurrent(sf::Time dt, CommandQueue &commands) {
  Entity::updateCurrent(dt, commands);
}

void Paddle::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(sprite, states);
}

int Paddle::getMovementSpeed() const {
  return SPEED;
}

sf::FloatRect Paddle::getBoundingRect() const {
  return getWorldTransform().transformRect(sprite.getGlobalBounds());
}

bool Paddle::isMarkedForRemoval() const {
  return false;
}
