#include "Tweenable.hpp"

void Tweenable::tween(std::unique_ptr<Tween> tween, sf::Time delay) {
  delayTween = delay;
  tweenObject = std::move(tween);
}

void Tweenable::update(sf::Time dt) {
  delayTween -= dt;
  if(tweenObject && delayTween <= sf::Time::Zero) {
    tweenObject->update(dt);
    if(tweenObject->done()) {
      tweenObject.reset(nullptr);
    }
  }
}
