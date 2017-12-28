#ifndef BREAK_IT_EASEOUTELASTIC_HPP
#define BREAK_IT_EASEOUTELASTIC_HPP

#include <cmath>
#include "Tween.hpp"

class EaseOutElastic : public Tween {
  public:
    using Tween::Tween;
    float calculate(const sf::Time &dt) override;
};

#endif //BREAK_IT_EASEOUTELASTIC_HPP
