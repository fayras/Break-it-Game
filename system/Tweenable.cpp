#include "Tweenable.hpp"

void Tweenable::tweenRotationTo(float angle) {

}

void Tweenable::tweenScaleTo(float x, float y) {

}

void Tweenable::tweenPositionTo(sf::Vector2f to) {
  positionFrom = getPosition();
  positionDiff = to - positionFrom;
  time = sf::Time::Zero;
}

void Tweenable::setTweenType(Tweenable::Type type) {
  currentType = type;
}

void Tweenable::update(sf::Time dt) {
  time += dt;
  if(time.asMilliseconds() < 0 || time.asMilliseconds() >= 300 || positionDiff == sf::Vector2f()) {
    return;
  }

  sf::Vector2f q = positionDiff * (float) time.asMilliseconds() / 300.0f + positionFrom;
  setPosition(q);
}

void Tweenable::setTime(sf::Time time) {
  this->time = time;
}

void Tweenable::tweenDelay(sf::Time delay) {
  time -= delay;
}
