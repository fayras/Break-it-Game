#include "SettingsState.hpp"

SettingsState::SettingsState(StateStack &stack, State::Context context) : State(stack, context) {

}

SettingsState::~SettingsState() {

}

void SettingsState::draw() {

}

bool SettingsState::update(sf::Time dt) {
  return false;
}

bool SettingsState::handleEvent(const sf::Event &event) {
  return false;
}
