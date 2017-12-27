#ifndef BREAK_IT_TWEENABLE_HPP
#define BREAK_IT_TWEENABLE_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include "Tween.hpp"

class Tweenable {
  public:
    void tween(Tween::Ptr tween);
    void update(sf::Time dt);

  private:
    Tween::Ptr tweenObject{nullptr};
};

#endif //BREAK_IT_TWEENABLE_HPP
