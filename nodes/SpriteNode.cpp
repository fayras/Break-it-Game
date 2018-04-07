#include "SpriteNode.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

SpriteNode::SpriteNode(const sf::Texture &texture)
  : sprite(texture)
{}

SpriteNode::SpriteNode(const sf::Texture &texture, const sf::IntRect &textureRect)
  : sprite(texture, textureRect)
{}

void SpriteNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(sprite, states);
}

void SpriteNode::setColor(sf::Color color) {
  sprite.setColor(color);
}

sf::FloatRect SpriteNode::getBoundingRect() const {
  return getWorldTransform().transformRect(sprite.getGlobalBounds());
}
