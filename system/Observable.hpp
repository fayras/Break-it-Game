#ifndef BREAK_IT_OBSERVABLE_HPP
#define BREAK_IT_OBSERVABLE_HPP

#include <vector>
#include <memory>
#include <functional>

class Observable {
  public:
    typedef std::function<void()> Callback;

    void attachObserver(const Callback& callback);
    void notify() const;

  protected:
    std::vector<Observable::Callback> observers;
};

#endif //BREAK_IT_OBSERVABLE_HPP
