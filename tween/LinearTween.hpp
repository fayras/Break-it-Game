#ifndef BREAK_IT_LINEARTWEEN_HPP
#define BREAK_IT_LINEARTWEEN_HPP

#include "Tween.hpp"

class LinearTween : public Tween {
  public:
    using Tween::Tween;
    float calculate(const sf::Time &dt) override;
};

#endif //BREAK_IT_LINEARTWEEN_HPP
