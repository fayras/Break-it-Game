#include "DuplicateBallSkill.hpp"
#include "../entities/Ball.hpp"

DuplicateBallSkill::DuplicateBallSkill(const sf::Texture &texture, const sf::Font &font, sf::Keyboard::Key assignedKey)
    : Skill(texture, font, 3.0f, assignedKey)
{
  defaultCategory = Category::DUPLICATE_SKILL;
  command.category = Category::BALL;
  command.action = derivedAction<Ball>([](Ball& ball, sf::Time) {
    ball.duplicate();
  });
}
