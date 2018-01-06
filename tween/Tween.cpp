#include "Tween.hpp"

Tween::Tween(sf::Time duration, Tween::Callback cb)
    : duration(duration), callback(std::move(cb)), time(sf::Time::Zero), delayTween(sf::Time::Zero)
{}

bool Tween::done() const {
  return time > duration;
}

void Tween::delay(sf::Time delay) {
  delayTween = delay;
}

void Tween::update(const sf::Time &dt) {
  delayTween -= dt;

  if(delayTween > sf::Time::Zero) {
    return;
  }

  callback(calculate(dt));
  time += dt;
}

void Tween::finalUpdate() {
  callback(1.0f);
}
