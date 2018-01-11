#include "Block.hpp"
#include "../system/Utility.hpp"
#include "../system/ResourceHolder.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

Block::Block(const TextureHolder& textures, Blocks::Type type)
  : Entity(100),
    sprite(textures.get(Textures::BLOCK)),
    breakAnimation(textures.get(Textures::EXPLOSION)),
    textures(textures),
    type(type),
    timer()
{
  breakAnimation.setFrameSize(sf::Vector2i(256, 256));
  breakAnimation.setNumFrames(16);
  breakAnimation.setDuration(sf::seconds(0.75));

  switch (type) {
    case Blocks::SMALL : sprite.setTexture(textures.get(Textures::BLOCK_SMALL), true);
    case Blocks::DOUBLE_HP: setHP(200);
    case Blocks::INVISIBLE: ;
    case Blocks::SELF_HEALING: ;
    case Blocks::NORMAL: ;
  }

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
    if(type == Blocks::SELF_HEALING && timer.getElapsedTime().asSeconds() == 2.0f) {
      timer.restart();
      heal(getHP() * 0.1f);
    }
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

void Block::damage(int points) {
  Entity::damage(points);
}
