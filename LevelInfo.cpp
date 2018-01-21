#include <cmath>
#include <SFML/Graphics/RenderTarget.hpp>
#include "LevelInfo.hpp"
#include "tween/LinearTween.hpp"
#include "ResourceIdentifiers.hpp"

LevelInfo::LevelInfo(const sf::Font &font, const sf::Vector2f view)
  : view(view)
{
  pausedText.setFont(font);
  pausedText.setCharacterSize(130);

  backgroundShape.setSize(sf::Vector2f(view.x, 100));
  backgroundShape.setOrigin(view.x * 0.5f, 50);
  backgroundShape.setPosition(0.5f * view.x, 0.3f * view.y);

  backgroundShape.setFillColor(sf::Color(0, 0, 0, 0));
}

void LevelInfo::show(int level, sf::Time delay) {
  pausedText.setString("Level " + std::to_string(level));

  sf::FloatRect bounds = pausedText.getLocalBounds();
  pausedText.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));

  sf::Vector2f from(view.x + bounds.width, 0.3f * view.y);
  sf::Vector2f to(-bounds.width, 0.3f * view.y);

  auto t = std::make_unique<LinearTween>(sf::seconds(1.5f), [this, from, to](const float& t) {
    showInfo = true;
    sf::Vector2f diff = to - from;
    pausedText.setPosition(diff * t + from);
  });
  t->delay(delay);
  tween(std::move(t));

  auto t2 = std::make_unique<LinearTween>(sf::milliseconds(200), [this](const float& t) {
    backgroundShape.setFillColor(sf::Color(0, 0, 0, (sf::Uint8) (150 * t)));
  });
  t2->delay(delay - sf::milliseconds(100));
  tween(std::move(t2));

  auto t3 = std::make_unique<LinearTween>(sf::milliseconds(200), [this](const float& t) {
    backgroundShape.setFillColor(sf::Color(0, 0, 0, (sf::Uint8) (150 - (150 * t))));
  });
  t3->delay(delay + sf::seconds(1.5f));
  t3->attachObserver([this]() {
    showInfo = false;
  });
  tween(std::move(t3));
}

unsigned int LevelInfo::getCategory() const {
  return Category::LEVEL_INFO;
}

void LevelInfo::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
  if(showInfo) {
    target.draw(backgroundShape, states);
    target.draw(pausedText, states);
  }
}
