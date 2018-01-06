#ifndef BREAK_IT_TWEEN_HPP
#define BREAK_IT_TWEEN_HPP

#include <SFML/System/Time.hpp>
#include <functional>
#include <memory>
#include "../system/Observable.hpp"

class Tween : public Observable {
  public:
    typedef std::function<void(const float&)> Callback;

    Tween(sf::Time duration, Callback cb);
    ~Tween() = default;

    virtual float calculate(const sf::Time &dt) = 0;
    void update(const sf::Time &dt);
    void finalUpdate();
    void delay(sf::Time delay);
    bool done() const;

  protected:
    sf::Time duration;
    sf::Time delayTween;
    sf::Time time;
    Callback callback;
};

#endif //BREAK_IT_TWEEN_HPP
