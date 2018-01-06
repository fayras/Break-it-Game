#include "Score.hpp"
#include "system/Utility.hpp"
#include "tween/LinearTween.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>

Score::Score(const sf::Font& font)
  : scoreText("Score: " + std::to_string(score), font, 41)
{
  scoreText.setOrigin(scoreText.getGlobalBounds().width, 0);
}

void Score::updateCurrent(sf::Time dt, CommandQueue &commands) {

}

void Score::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(scoreText, states);
}

void Score::increaseMultiplier() {
  multiplier += 0.1f;
}

void Score::resetMultiplier() {
  multiplier = 1.0f;
}

void Score::increase(int amount) {
  score += amount * multiplier;
  auto tw = std::make_unique<LinearTween>(sf::seconds(1.0f), [this](const float& t) {
    currentVisibleScore += (score - currentVisibleScore) * t;
    scoreText.setString("Score: " + std::to_string(currentVisibleScore));
    scoreText.setOrigin(scoreText.getGlobalBounds().width, 0);
  });
  tween(std::move(tw));

  // scoreText.setString("Score: " + std::to_string(score));
  // scoreText.setOrigin(scoreText.getGlobalBounds().width, 0);
}

int Score::get() const {
  return score;
}

unsigned int Score::getCategory() const {
  return Category::SCORE;
}
