#include "Wall.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

Wall::Wall(float width, float height)
  : Entity(1),
    rect(sf::Vector2f(width, height))
{
  rect.setFillColor(sf::Color::Black);
}

unsigned int Wall::getCategory() const {
  return Category::WALL;
}

sf::FloatRect Wall::getBoundingRect() const {
  return getWorldTransform().transformRect(rect.getGlobalBounds());
}

void Wall::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(rect, states);
}

void Wall::setDeadly(bool deadly) {
  this->deadly = deadly;
}

bool Wall::isDeadly() const {
  return deadly;
}
