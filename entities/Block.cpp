#include "Block.hpp"
#include "../system/Utility.hpp"
#include "../system/ResourceHolder.hpp"
#include "../tween/LinearTween.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

Block::Block(const TextureHolder& textures, Blocks::Type type)
  : Entity(100),
    sprite(textures.get(Textures::BLOCK)),
    breakAnimation(textures.get(Textures::EXPLOSION)),
    textures(textures),
    type(type)
{
  breakAnimation.setFrameSize(sf::Vector2i(126, 116));
  breakAnimation.setNumFrames(9);
  breakAnimation.setDuration(sf::seconds(0.4));

  if(type == Blocks::SMALL) sprite.setTexture(textures.get(Textures::BLOCK_SMALL), true);
  if(type == Blocks::DOUBLE_HP) setHP(200);
  if(type == Blocks::SELF_HEALING) timers.emplace(Blocks::SELF_HEALING, sf::Clock());
  if(type == Blocks::SWAY) timers.emplace(Blocks::SWAY, sf::Clock());
  if(type == Blocks::INVISIBLE) {
    hidden = true;
    timers.emplace(Blocks::INVISIBLE, sf::Clock());
  }

  centerOrigin(sprite);
  centerOrigin(breakAnimation);

  defaultCategory = Category::BLOCK;
}

void Block::updateCurrent(sf::Time dt, CommandQueue &commands) {
  if(isDestroyed()) {
    breakAnimation.update(dt);
  } else {
    Entity::updateCurrent(dt, commands);
    if(type == Blocks::SELF_HEALING && timers.at(Blocks::SELF_HEALING).getElapsedTime().asSeconds() >= 2.0f) {
      heal(static_cast<int>(getMaxHitpoints() * 0.1f));
      timers.at(Blocks::SELF_HEALING).restart();
    }
    if(type == Blocks::INVISIBLE && timers.at(Blocks::INVISIBLE).getElapsedTime().asSeconds() >= 3.0f - Random::decimal(-0.5f, 0.5f)) {
      hidden = false;
      tween(std::make_unique<LinearTween>(sf::milliseconds(150), [this](const float& t) {
        const sf::Color& c = sprite.getColor();
        sprite.setColor({ c.r, c.g, c.b, static_cast<sf::Uint8>(255 * t) });
      }));
      timers.at(Blocks::INVISIBLE).restart();
    }
    if(!hidden && type == Blocks::INVISIBLE && timers.at(Blocks::INVISIBLE).getElapsedTime().asSeconds() >= 0.3f) {
      hidden = true;
      tween(std::make_unique<LinearTween>(sf::milliseconds(150), [this](const float& t) {
        const sf::Color& c = sprite.getColor();
        sprite.setColor({ c.r, c.g, c.b, static_cast<sf::Uint8>(255 - (255 * t)) });
      }));
      timers.at(Blocks::INVISIBLE).restart();
    }

    // sway block a bit
    if(type == Blocks::SWAY) {
      float offsetX = (float) std::sin(timers.at(Blocks::SWAY).getElapsedTime().asSeconds()) * .3f + Random::decimal(-0.2f, 0.2f);
      float offsetY = (float) std::cos(timers.at(Blocks::SWAY).getElapsedTime().asSeconds()) * .3f + Random::decimal(-0.2f, 0.2f);
      move(offsetX, offsetY);
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
