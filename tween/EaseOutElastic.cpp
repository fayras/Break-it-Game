#include "EaseOutElastic.hpp"

float EaseOutElastic::calculate(const sf::Time &dt) {
  if(time == sf::Time::Zero) return 0.0f;

  float t = time / duration;
  if(t == 1.0f) return 1.0f;

  float p = duration.asMilliseconds() *  0.3f;
  float a = 1.0f;
  float s = p / 4.0f;

  float val = a * (float) std::pow(2, -10.0f * t)
              * (float) std::sin((t * (float) duration.asMilliseconds() - s) * (2 * 3.1415927) / p)
              + a;

  return val;
}
