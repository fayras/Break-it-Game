#include <cmath>
#include <SFML/Graphics/RenderTarget.hpp>
#include "LevelInfo.hpp"
#include "tween/LinearTween.hpp"
#include "ResourceIdentifiers.hpp"

LevelInfo::LevelInfo(const sf::Font &font)
{
  pausedText.setFont(font);
  pausedText.setCharacterSize(70);

  backgroundShape.setFillColor(sf::Color(0, 0, 0, 0));
}

void LevelInfo::show(int level, sf::Time delay) {
  pausedText.setString("Level " + std::to_string(level));
  backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));

  sf::FloatRect bounds = pausedText.getLocalBounds();
  pausedText.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));

  sf::Vector2f from(view.x + bounds.width / 2, 0.3f * view.y);
  sf::Vector2f to(-bounds.width / 2, 0.3f * view.y);

  auto t = std::make_unique<LinearTween>(sf::seconds(1.5f), [this, from, to](const float& t) {
    sf::Vector2f diff = to - from;
    pausedText.setPosition(diff * t + from);
  });
  t->delay(delay);
  tween(std::move(t));
}

unsigned int LevelInfo::getCategory() const {
  return Category::LEVEL_INFO;
}

void LevelInfo::updateCurrent(sf::Time dt, CommandQueue &commands) {
  Tweenable::update(dt);
}

void LevelInfo::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(backgroundShape);
  target.draw(pausedText);
}

void LevelInfo::setView(sf::Vector2f view) {
  this->view = view;

  backgroundShape.setSize(sf::Vector2f(view.x, 100));
  backgroundShape.setOrigin(view.x * 0.5f, 50);
  backgroundShape.setPosition(0.5f * view.x, 0.3f * view.y);
}
