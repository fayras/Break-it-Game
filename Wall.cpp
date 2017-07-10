#include "Wall.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

Wall::Wall(float width, float height)
  : rect(sf::Vector2f(width, height))
{}

unsigned int Wall::getCategory() const {
  return Category::WALL;
}

sf::FloatRect Wall::getBoundingRect() const {
  return getWorldTransform().transformRect(rect.getGlobalBounds());
}

void Wall::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(rect, states);
}

void Wall::highlight(bool flag) {
  if(flag) rect.setFillColor(sf::Color::Red);
  else rect.setFillColor(sf::Color::White);
}
