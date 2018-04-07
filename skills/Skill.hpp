#ifndef BREAK_IT_SKILL_HPP
#define BREAK_IT_SKILL_HPP

#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "../nodes/SpriteNode.hpp"

class Skill : public SpriteNode {
  public:
    explicit Skill(const sf::Texture &texture, const sf::Font &font, float cooldownInSeconds, sf::Keyboard::Key key);
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
    sf::Text assignedKey;
};

#endif //BREAK_IT_SKILL_HPP
