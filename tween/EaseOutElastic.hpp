#ifndef BREAK_IT_EASEOUTELASTIC_HPP
#define BREAK_IT_EASEOUTELASTIC_HPP

#include <cmath>
#include "Tween.hpp"

class EaseOutElastic : public Tween {
  public:
    using Tween::Tween;
    void update(sf::Time dt) override;
};

#endif //BREAK_IT_EASEOUTELASTIC_HPP
