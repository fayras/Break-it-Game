#include "SlowmotionSkill.hpp"
#include "../entities/Ball.hpp"

SlowmotionSkill::SlowmotionSkill(const sf::Texture &texture, const sf::Font &font, sf::Keyboard::Key assignedKey)
        : Skill(texture, {320, 0, 40, 40}, font, assignedKey, 30.f, 5.f)
{
    defaultCategory = Category::SLOWMOTION_SKILL;
    command.category = Category::BALL;
    command.action = derivedAction<Ball>([](Ball& ball, sf::Time) {
        ball.setVelocity(ball.getVelocity() / 2.f);
    });

    deactivateCommand.category = Category::BALL;
    deactivateCommand.action = derivedAction<Ball>([](Ball& ball, sf::Time) {
        ball.setVelocity(ball.getVelocity() * 2.f);
    });
}
