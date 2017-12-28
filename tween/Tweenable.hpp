#ifndef BREAK_IT_TWEENABLE_HPP
#define BREAK_IT_TWEENABLE_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include "Tween.hpp"

class Tweenable {
  public:
    void tween(std::unique_ptr<Tween> tween, sf::Time delay = sf::Time::Zero);
    void update(sf::Time dt);

  private:
    std::unique_ptr<Tween> tweenObject{nullptr};
    sf::Time delayTween;
};

#endif //BREAK_IT_TWEENABLE_HPP
