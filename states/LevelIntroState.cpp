#include "LevelIntroState.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../system/ResourceHolder.hpp"
#include "../tween/LinearTween.hpp"
#include <cmath>

LevelIntroState::LevelIntroState(StateStack &stack, State::Context context) : State(stack, context) {
  sf::Font& font = context.fonts->get(Fonts::MAIN);
  sf::Vector2f windowSize(context.window->getSize());

  pausedText.setFont(font);
  pausedText.setString("Level " + std::to_string(context.player->getLevel()));
  pausedText.setCharacterSize(70);
  sf::FloatRect bounds = pausedText.getLocalBounds();
  pausedText.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
  // pausedText.setPosition(0.5f * windowSize.x, 0.3f * windowSize.y);

  backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
  backgroundShape.setSize(sf::Vector2f(windowSize.x, 100));
  backgroundShape.setOrigin(windowSize.x * 0.5f, 50);
  backgroundShape.setPosition(0.5f * windowSize.x, 0.3f * windowSize.y);

  sf::Vector2f from(windowSize.x + bounds.width / 2, 0.3f * windowSize.y);
  sf::Vector2f to(-bounds.width / 2, 0.3f * windowSize.y);

  tween(std::make_unique<LinearTween>(
      sf::seconds(1.5f),
      [this, from, to](const float& t) {
        sf::Vector2f diff = to - from;
        pausedText.setPosition(diff * t + from);
      }
  ));
}

void LevelIntroState::draw() {
  sf::RenderWindow& window = *getContext().window;
  window.setView(window.getDefaultView());

  window.draw(backgroundShape);
  window.draw(pausedText);
}

bool LevelIntroState::update(sf::Time dt) {
  Tweenable::update(dt);
  if(timer.getElapsedTime().asSeconds() > 1.5f) {
    requestStackPop();
  }
  return false;
}

bool LevelIntroState::handleEvent(const sf::Event &event) {
  return false;
}
