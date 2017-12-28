#include "LinearTween.hpp"

float LinearTween::calculate(const sf::Time &dt) {
  this->time += dt;
  return (float) this->time.asMilliseconds() / (float) this->duration.asMilliseconds();
}