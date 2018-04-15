#include "DuplicateBallSkill.hpp"
#include "../entities/Ball.hpp"

DuplicateBallSkill::DuplicateBallSkill(const sf::Texture &texture, const sf::Font &font, sf::Keyboard::Key assignedKey)
    : Skill(texture, {120, 120, 40, 40}, font, assignedKey, 30.0f)
{
  defaultCategory = Category::DUPLICATE_SKILL;
  command.category = Category::BALL;
  command.action = derivedAction<Ball>([](Ball& ball, sf::Time) {
    ball.duplicate();
  });
}
