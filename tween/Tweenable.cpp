#include "Tweenable.hpp"

void Tweenable::tween(std::unique_ptr<Tween> tween) {
  tweenObject = std::move(tween);
}

void Tweenable::update(sf::Time dt) {
  if(tweenObject) {
    tweenObject->update(dt);
    if(tweenObject->done()) {
      tweenObject->notify();
      tweenObject.reset(nullptr);
    }
  }
}
