#ifndef BREAK_IT_TWEEN_HPP
#define BREAK_IT_TWEEN_HPP

#include <SFML/System/Time.hpp>
#include <functional>
#include <memory>
#include "TweenDummy.hpp"

template <typename T>
class Tween : public TweenDummy {
  public:
    typedef std::function<void(const T&)> Callback;

    Tween(T from, T to, sf::Time duration, Callback cb);
    ~Tween() override = default;

    void update(sf::Time dt) override = 0;
    bool done() const final;

  protected:
    T from;
    T to;
    sf::Time duration;
    sf::Time time;
    Callback callback;
};

template<typename T>
Tween<T>::Tween(T from, T to, sf::Time duration, Tween::Callback cb)
    : from(from), to(to), duration(duration), callback(cb), time(sf::Time::Zero)
{}

template<typename T>
bool Tween<T>::done() const {
  return time > duration;
}

#endif //BREAK_IT_TWEEN_HPP
