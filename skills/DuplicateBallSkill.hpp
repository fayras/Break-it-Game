#ifndef BREAK_IT_DUPLICATEBALLSKILL_HPP
#define BREAK_IT_DUPLICATEBALLSKILL_HPP

#include <SFML/Window/Keyboard.hpp>
#include "Skill.hpp"

class DuplicateBallSkill : public Skill {
  public:
    explicit DuplicateBallSkill(const sf::Texture &texture, const sf::Font &font, sf::Keyboard::Key assignedKey);
};

#endif //BREAK_IT_DUPLICATEBALLSKILL_HPP
