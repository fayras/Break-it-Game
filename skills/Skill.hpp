#ifndef BREAK_IT_SKILL_HPP
#define BREAK_IT_SKILL_HPP

#include <SFML/Graphics/Text.hpp>
#include "../nodes/SpriteNode.hpp"

class Skill : public SpriteNode {
  public:
    explicit Skill(const sf::Texture &texture, const sf::Font &font, float cooldownInSeconds);
    void activate();

  protected:
    void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;
    void updateCurrent(sf::Time dt, CommandQueue &commands) override;

    Command command;

  private:
    sf::Time cooldown;
    sf::Time cooldownTime;
    sf::Text cooldownText;
    bool activateSkill{false};
};

#endif //BREAK_IT_SKILL_HPP
