#ifndef BREAK_IT_TWEENABLE_HPP
#define BREAK_IT_TWEENABLE_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include "Tween.hpp"

class Tweenable {
  public:
    void tween(std::unique_ptr<Tween> tween);
    void update(sf::Time dt);

  private:
    std::vector<std::unique_ptr<Tween>> tweens;
    // std::unique_ptr<Tween> tweenObject{nullptr};
};

#endif //BREAK_IT_TWEENABLE_HPP
