#ifndef BREAK_IT_TWEENDUMMY_HPP
#define BREAK_IT_TWEENDUMMY_HPP

#include <SFML/System/Time.hpp>
#include <iostream>

struct TweenDummy {
  virtual ~TweenDummy() = default;
  virtual bool done() const = 0;
  virtual void update(sf::Time dt) = 0;
};

#endif //BREAK_IT_TWEENDUMMY_HPP
