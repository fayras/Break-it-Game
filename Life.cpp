#include "Life.hpp"

Life::Life(const sf::Texture &texture) : SpriteNode(texture), originalRect(sprite.getTextureRect()) {
  sf::IntRect rect = sf::IntRect(originalRect);
  rect.width *= lives;
  sprite.setTextureRect(rect);
}

unsigned int Life::getCategory() const {
  return Category::LIFE;
}

int Life::getLives() const {
  return lives;
}

void Life::decrease() {
  lives -= 1;
  sf::IntRect rect = sf::IntRect(originalRect);
  rect.width *= lives;
  sprite.setTextureRect(rect);
}
