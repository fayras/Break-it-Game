#ifndef BREAK_IT_TWEEN_HPP
#define BREAK_IT_TWEEN_HPP

#include <SFML/System/Time.hpp>
#include <functional>
#include <memory>

template <typename T>
class Tween {
  public:
    typedef std::unique_ptr<Tween> Ptr;
    typedef std::function<void(T)> Callback;

    Tween(T from, T to, sf::Time duration, Callback cb);
    bool done() const;

    virtual void update(sf::Time dt) = 0;

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
