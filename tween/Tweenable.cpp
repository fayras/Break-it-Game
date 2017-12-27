#include "Tweenable.hpp"

void Tweenable::tween(Tween::Ptr tween) {
  tweenObject = std::move(tween);
}

void Tweenable::update(sf::Time dt) {
  if(tweenObject != nullptr) {
    if(tweenObject->done()) {
      tweenObject = nullptr;
    }
    tweenObject->update(dt);
  }
}
