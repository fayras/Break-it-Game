#include "Tweenable.hpp"

void Tweenable::tween(std::unique_ptr<Tween> tween) {
  tweens.push_back(std::move(tween));
}

void Tweenable::update(sf::Time dt) {
  for(auto tweenIterator = tweens.begin(); tweenIterator != tweens.end();) {
    (*tweenIterator)->update(dt);
    if((*tweenIterator)->done()) {
      (*tweenIterator)->notify();
      tweenIterator = tweens.erase(tweenIterator);
    } else {
      ++tweenIterator;
    }
  }
}
