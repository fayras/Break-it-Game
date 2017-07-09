#include "Block.hpp"
#include "system/Utility.hpp"
#include "system/ResourceHolder.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

Block::Block( const TextureHolder& textures)
  : Entity(100),
    sprite(textures.get(Textures::BLOCK))
{
  centerOrigin(sprite);
}

Block::Block( const TextureHolder& textures, sf::Color color)
  : Entity(100),
    sprite(textures.get(Textures::BLOCK))
{
  sprite.setColor(color);
  centerOrigin(sprite);
}

unsigned int Block::getCategory() const {
  return Category::BLOCK;
}

void Block::updateCurrent(sf::Time dt, CommandQueue &commands) {
  Entity::updateCurrent(dt, commands);
}

void Block::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(sprite, states);
}

sf::FloatRect Block::getBoundingRect() const {
  return getWorldTransform().transformRect(sprite.getGlobalBounds());
}
