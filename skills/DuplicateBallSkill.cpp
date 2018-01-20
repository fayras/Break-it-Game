#include "DuplicateBallSkill.hpp"
#include "../entities/Ball.hpp"

DuplicateBallSkill::DuplicateBallSkill(const sf::Texture &texture)
    : Skill(texture, 5.0f)
{
  defaultCategory = Category::DUPLICATE_SKILL;
  command.category = Category::BALL;
  command.action = derivedAction<Ball>([](Ball& ball, sf::Time) {
    ball.duplicate();
  });
}
