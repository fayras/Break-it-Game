#ifndef BREAK_IT_TWEEN_HPP
#define BREAK_IT_TWEEN_HPP

#include <SFML/System/Time.hpp>
#include <functional>
#include <memory>

class Tween {
  public:
    typedef std::function<void(const float&)> Callback;

    Tween(sf::Time duration, Callback cb);
    ~Tween() = default;

    virtual float calculate(const sf::Time &dt) = 0;
    bool done() const;
    Callback& getCallback() const;

  protected:
    sf::Time duration;
    sf::Time time;
    Callback callback;
};

#endif //BREAK_IT_TWEEN_HPP
