#include "LinearTween.hpp"

void LinearTween::update(sf::Time dt) {
  this->time += dt;
  this->callback((float) this->time.asMilliseconds() / (float) this->duration.asMilliseconds());
}