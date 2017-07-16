#include "Block.hpp"
#include "system/Utility.hpp"
#include "system/ResourceHolder.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>

Block::Block( const TextureHolder& textures)
  : Entity(100),
    sprite(textures.get(Textures::BLOCK)),
    breakAnimation(textures.get(Textures::EXPLOSION))
{
  breakAnimation.setFrameSize(sf::Vector2i(256, 256));
  breakAnimation.setNumFrames(16);
  breakAnimation.setDuration(sf::seconds(1));

  centerOrigin(sprite);
  centerOrigin(breakAnimation);
}

Block::Block( const TextureHolder& textures, sf::Color color)
  : Entity(100),
    sprite(textures.get(Textures::BLOCK)),
    breakAnimation(textures.get(Textures::EXPLOSION))
{
  breakAnimation.setFrameSize(sf::Vector2i(256, 256));
  breakAnimation.setNumFrames(16);
  breakAnimation.setDuration(sf::seconds(1));
  sprite.setColor(color);
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
    return sf::FloatRect();
  }
  return getWorldTransform().transformRect(sprite.getGlobalBounds());
}

bool Block::isMarkedForRemoval() const {
  return isDestroyed() && breakAnimation.isFinished();
}
