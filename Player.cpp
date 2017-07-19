#include "Player.hpp"
#include "system/CommandQueue.hpp"
#include "Paddle.hpp"
#include <map>
#include <string>
#include <algorithm>

using namespace std::placeholders;

Player::Player() {
  // Set initial key bindings
  keyBinding[sf::Keyboard::Left] = Action::MOVE_LEFT;
  keyBinding[sf::Keyboard::Right] = Action::MOVE_RIGHT;

  // Set initial action bindings
  initializeActions();

  // Assign all categories to player's aircraft
  for(auto& pair : actionBinding) {
    pair.second.category = Category::PADDLE;
  }
}

void Player::handleEvent(const sf::Event &event, CommandQueue &commands) {
  if (event.type == sf::Event::KeyPressed) {
    // Check if pressed key appears in key binding, trigger command if so
    auto found = keyBinding.find(event.key.code);
    if (found != keyBinding.end() && !isRealtimeAction(found->second))
      commands.push(actionBinding[found->second]);
  }
}

void Player::handleRealtimeInput(CommandQueue &commands) {
// Traverse all assigned keys and check if they are pressed
  for(auto pair : keyBinding) {
    // If key is pressed, lookup action and trigger corresponding command
    if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
      commands.push(actionBinding[pair.second]);
  }
}

void Player::assignKey(Player::Action action, sf::Keyboard::Key key) {
// Remove all keys that already map to action
  for (auto itr = keyBinding.begin(); itr != keyBinding.end(); ) {
    if (itr->second == action) keyBinding.erase(itr++);
    else ++itr;
  }

  // Insert new binding
  keyBinding[key] = action;
}

sf::Keyboard::Key Player::getAssignedKey(Player::Action action) const {
  for(auto pair : keyBinding) {
    if (pair.second == action) return pair.first;
  }

  return sf::Keyboard::Unknown;
}

void Player::initializeActions() {
  actionBinding[MOVE_LEFT].action = derivedAction<Paddle>([] (Paddle& p, sf::Time) {
    p.accelerate(-1 * p.getSpeed(), 0);
  });
  actionBinding[MOVE_RIGHT].action = derivedAction<Paddle>([] (Paddle& p, sf::Time) {
    p.accelerate(1 * p.getSpeed(), 0);
  });
}

bool Player::isRealtimeAction(Player::Action action) {
  switch (action) {
    case MOVE_LEFT:
    case MOVE_RIGHT:
      return true;
    default:
      return false;
  }
}

void Player::setScore(int score) {
  this->score = score;
}

int Player::getScore() const {
  return score;
}
