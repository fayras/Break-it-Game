#include "LinearTween.hpp"

float LinearTween::calculate(const sf::Time &dt) {
  return time / duration;
}