#include "Block.hpp"
#include "system/Utility.hpp"
#include "system/ResourceHolder.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

Block::Block(const TextureHolder& textures, Textures::ID textureID)
  : Entity(100),
    sprite(textures.get(textureID)),
    breakAnimation(textures.get(Textures::EXPLOSION))
{
  breakAnimation.setFrameSize(sf::Vector2i(256, 256));
  breakAnimation.setNumFrames(16);
  breakAnimation.setDuration(sf::seconds(0.75));

  centerOrigin(sprite);
  centerOrigin(breakAnimation);
}

unsigned int Block::getCategory() const {
  return Category::BLOCK;
}

void Block::updateCurrent(sf::Time dt, CommandQueue &commands) {
  if(isDestroyed()) {
    breakAnimation.update(dt);
  } else {
    Entity::updateCurrent(dt, commands);
  }
}

void Block::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
  if (isDestroyed()) {
    target.draw(breakAnimation, states);
  } else {
    target.draw(sprite, states);
  }
}

sf::FloatRect Block::getBoundingRect() const {
  if(isDestroyed()) {
    return {};
  }
  return getWorldTransform().transformRect(sprite.getGlobalBounds());
}

bool Block::isMarkedForRemoval() const {
  return isDestroyed() && breakAnimation.isFinished();
}

void Block::setColor(const sf::Color &color) {
  sprite.setColor(color);
}
