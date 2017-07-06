#include "StateStack.hpp"
#include <cassert>

StateStack::StateStack(State::Context context) 
  : stack(), pendingList(), context(context), factory()
{}

void StateStack::update(sf::Time dt) {
  for(auto itr = stack.rbegin(); itr != stack.rend(); ++itr) {
    if(!(*itr)->update(dt)) { break; }
  }

  applyPendingChanges();
}

void StateStack::draw() {
  for(State::Ptr& state : stack) {
    state->draw();
  }
}

void StateStack::handleEvent(const sf::Event& event) {
  for(auto itr = stack.rbegin(); itr != stack.rend(); ++itr) {
    if(!(*itr)->handleEvent(event)) { break; }
  }

  applyPendingChanges();
}

void StateStack::push(States::ID id) {
  pendingList.push_back(PendingChange(Action::PUSH, id));
}

void StateStack::pop() {
  pendingList.push_back(PendingChange(Action::POP));
}

void StateStack::clear() {
  pendingList.push_back(PendingChange(Action::CLEAR));
}

bool StateStack::empty() const {
  return stack.empty();
}

State::Ptr StateStack::createState(States::ID id) {
  auto found = factory.find(id);
  assert(found != factory.end());
  return found->second();
}

void StateStack::applyPendingChanges() {
  for(PendingChange& change : pendingList) {
    switch(change.action) {
      case Action::PUSH:
        stack.push_back(createState(change.stateID));
        break;
      case Action::POP:
        stack.pop_back();
        break;
      case Action::CLEAR:
        stack.clear();
        break;
    }
  }

  pendingList.clear();
}

StateStack::PendingChange::PendingChange(Action action, States::ID id) 
  : action(action), stateID(id)
{}
