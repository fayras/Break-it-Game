#ifndef BREAK_IT_EASEOUTELASTIC_HPP
#define BREAK_IT_EASEOUTELASTIC_HPP

#include <cmath>
#include "Tween.hpp"

template <typename T>
class EaseOutElastic : public Tween<T> {
  public:
    typedef std::function<void(T)> Callback;

    EaseOutElastic(T from, T to, sf::Time duration, Callback cb) : Tween<T>(from, to, duration, cb) {}

    void update(sf::Time dt) override;
};

template<typename T>
void EaseOutElastic<T>::update(sf::Time dt) {
  if(this->time == sf::Time::Zero) this->callback(this->from);
  float t = this->time / this->duration;
  if(t == 1.0f) this->callback(this->to);

  float p = this->duration.asMilliseconds() *  0.3f;
  T diff = this->to - Tween<T>::from;
  T a = diff;
  float s = p / 4.0f;

  T val = a * (float) std::pow(2, -10.0f * t)
          * (float) std::sin((t * (float) this->duration.asMilliseconds() - s) * (2 * 3.1415927) / p)
          + diff + this->from;

  this->time += dt;
  this->callback(val);
}


#endif //BREAK_IT_EASEOUTELASTIC_HPP
