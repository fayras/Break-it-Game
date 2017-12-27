#ifndef BREAK_IT_LINEARTWEEN_HPP
#define BREAK_IT_LINEARTWEEN_HPP

#include "Tween.hpp"

template <typename T>
class LinearTween : public Tween<T> {
  public:
    typedef std::function<void(T)> Callback;

    LinearTween(T from, T to, sf::Time duration, Callback cb) : Tween<T>(from, to, duration, cb) {}

    void update(sf::Time dt) override;
};

template<typename T>
void LinearTween<T>::update(sf::Time dt) {
  this->time += dt;
  T diff = this->to - Tween<T>::from;
  this->callback(diff * (float) this->time.asMilliseconds() / (float) this->duration.asMilliseconds() + this->from);
}

#endif //BREAK_IT_LINEARTWEEN_HPP
