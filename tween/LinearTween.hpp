#ifndef BREAK_IT_LINEARTWEEN_HPP
#define BREAK_IT_LINEARTWEEN_HPP

#include "Tween.hpp"

class LinearTween : public Tween {
  public:
    LinearTween(sf::Time duration, Callback cb) : Tween(duration, std::move(cb)) {}

    void update(sf::Time dt) override;
};

#endif //BREAK_IT_LINEARTWEEN_HPP
