#include "Observable.hpp"

void Observable::attachObserver(const Callback& callback) {
  observers.push_back(callback);
}

void Observable::notify() const {
  for(const auto& callback : observers) {
    callback();
  }
}
