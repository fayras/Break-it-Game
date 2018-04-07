#include "DuplicateBallSkill.hpp"
#include "../entities/Ball.hpp"

DuplicateBallSkill::DuplicateBallSkill(const sf::Texture &texture, const sf::Font &font)
    : Skill(texture, font, 30.0f)
{
  defaultCategory = Category::DUPLICATE_SKILL;
  command.category = Category::BALL;
  command.action = derivedAction<Ball>([](Ball& ball, sf::Time) {
    ball.duplicate();
  });
}
