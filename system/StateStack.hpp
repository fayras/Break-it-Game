#ifndef SFML_TEMPLATE_STATESTACK_HPP
#define SFML_TEMPLATE_STATESTACK_HPP

#include "State.hpp"
#include "../states/StateIdentifiers.hpp"
#include "../ResourceIdentifiers.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include <vector>
#include <utility>
#include <functional>
#include <map>

namespace sf {
  class Event;
  class RenderWindow;
}

class StateStack : private sf::NonCopyable {
  public:
    enum Action {
      PUSH, POP, CLEAR
    };

    explicit StateStack(State::Context context);

    void update(sf::Time dt);
    void draw();
    void handleEvent(const sf::Event& event);

    template<typename T, typename... Args>
    void registerState(States::ID id, Args&&... args);
    void push(States::ID stateID);
    void pop();
    void clear();

    bool empty() const;

  private:
    struct PendingChange {
      explicit PendingChange(Action action, States::ID stateID = States::NONE);
      Action action;
      States::ID stateID;      
    };

    State::Ptr createState(States::ID stateID);
    void applyPendingChanges();

    std::vector<State::Ptr> stack;
    std::vector<PendingChange> pendingList;

    State::Context context;
    std::map<States::ID, std::function<State::Ptr()>> factory;
};

template <typename T, typename... Args>
void StateStack::registerState(States::ID id, Args&&... args) {
  factory[id] = [this, args...] () -> State::Ptr {
    return State::Ptr(new T(*this, context, args...));
  };
}

#endif //SFML_TEMPLATE_STATESTACK_HPP