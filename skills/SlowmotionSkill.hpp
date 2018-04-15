
#ifndef BREAK_IT_SLOWMOTIONSKILL_HPP
#define BREAK_IT_SLOWMOTIONSKILL_HPP


#include <SFML/Window/Keyboard.hpp>
#include "Skill.hpp"

class SlowmotionSkill : public Skill {
public:
    explicit SlowmotionSkill(const sf::Texture &texture, const sf::Font &font, sf::Keyboard::Key assignedKey);
};


#endif //BREAK_IT_SLOWMOTIONSKILL_HPP
