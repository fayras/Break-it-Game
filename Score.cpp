#include "Score.hpp"
#include "system/Utility.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

Score::Score(const sf::Texture &texture, const sf::Font& font)
  : SpriteNode(texture),
    scoreText("Score: " + std::to_string(score), font, 41)
{
  scoreText.move(20, 0);
}

void Score::updateCurrent(sf::Time dt, CommandQueue &commands) {
  SpriteNode::updateCurrent(dt, commands);
}

void Score::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
  SpriteNode::drawCurrent(target, states);
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
  scoreText.setString("Score: " + std::to_string(score));
}

int Score::get() const {
  return score;
}

unsigned int Score::getCategory() const {
  return Category::SCORE;
}
