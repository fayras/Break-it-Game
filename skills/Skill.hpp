#ifndef BREAK_IT_SKILL_HPP
#define BREAK_IT_SKILL_HPP

#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "../nodes/SpriteNode.hpp"

class Skill : public SpriteNode {
  public:
    Skill(const sf::Texture &texture, const sf::Font &font, sf::Keyboard::Key key, float cooldownInSeconds, float activationTime = 0);
    Skill(const sf::Texture &texture, const sf::IntRect &textureRect, const sf::Font &font, sf::Keyboard::Key key, float cooldownInSeconds, float activationTime = 0);
    void activate();

  protected:
    void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;
    void updateCurrent(sf::Time dt, CommandQueue &commands) override;

    Command command;
    Command deactivateCommand;

  private:
    sf::Time cooldown;
    sf::Time cooldownTime;
    sf::Time activation;
    sf::Time activationTime;
    sf::Text cooldownText;
    bool activateSkill{false};
    sf::Text assignedKey;
};

#endif //BREAK_IT_SKILL_HPP
