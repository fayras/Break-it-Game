#ifndef BREAK_IT_SKILL_HPP
#define BREAK_IT_SKILL_HPP

#include "../nodes/SpriteNode.hpp"

class Skill : public SpriteNode {
  public:
    explicit Skill(const sf::Texture &texture, float cooldownInSeconds);
    void activate();

  protected:
    void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;
    void updateCurrent(sf::Time dt, CommandQueue &commands) override;

    Command command;

  private:
    sf::Time cooldownTime;
    sf::Time cooldown;
    bool activateSkill{false};
};

#endif //BREAK_IT_SKILL_HPP
