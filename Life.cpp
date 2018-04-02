#include "Life.hpp"

Life::Life(const sf::Texture &texture)
  : SpriteNode(texture),
    originalRect(sprite.getTextureRect())
{
  defaultCategory = Category::LIFE;

  sf::IntRect rect(originalRect);
  rect.width *= lives;
  sprite.setTextureRect(rect);
}

int Life::getLives() const {
  return lives;
}

void Life::decrease() {
  lives -= 1;
  sf::IntRect rect(originalRect);
  rect.width *= lives;
  sprite.setTextureRect(rect);
}

void Life::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
  states.transform = sf::Transform::Identity * getTransform();
  SpriteNode::drawCurrent(target, states);
}
