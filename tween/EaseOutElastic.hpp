#ifndef BREAK_IT_EASEOUTELASTIC_HPP
#define BREAK_IT_EASEOUTELASTIC_HPP

#include <cmath>
#include "Tween.hpp"

class EaseOutElastic : public Tween {
  public:
    EaseOutElastic(sf::Time duration, Callback cb) : Tween(duration, std::move(cb)) {}

    void update(sf::Time dt) override;
};

#endif //BREAK_IT_EASEOUTELASTIC_HPP
