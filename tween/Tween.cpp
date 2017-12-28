#include "Tween.hpp"

Tween::Tween(sf::Time duration, Tween::Callback cb)
    : duration(duration), callback(std::move(cb)), time(sf::Time::Zero)
{}

bool Tween::done() const {
  return time > duration;
}

Tween::Callback &Tween::getCallback() const {
  return callback;
}
