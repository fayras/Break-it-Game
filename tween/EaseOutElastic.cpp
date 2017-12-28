#include "EaseOutElastic.hpp"

void EaseOutElastic::update(sf::Time dt) {
  if(time == sf::Time::Zero) callback(0.0f);
  float t = time / duration;
  if(t == 1.0f) this->callback(1.0f);

  float p = this->duration.asMilliseconds() *  0.3f;
  float a = 1.0f;
  float s = p / 4.0f;

  float val = a * (float) std::pow(2, -10.0f * t)
              * (float) std::sin((t * (float) this->duration.asMilliseconds() - s) * (2 * 3.1415927) / p)
              + a;

  this->time += dt;
  this->callback(val);
}
