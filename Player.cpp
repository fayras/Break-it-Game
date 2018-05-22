#include "Player.hpp"
#include "system/CommandQueue.hpp"
#include "entities/Paddle.hpp"
#include "entities/Ball.hpp"
#include "skills/DuplicateBallSkill.hpp"
#include "skills/SlowmotionSkill.hpp"
#include "system/Utility.hpp"
#include <map>
#include <string>
#include <algorithm>

using namespace std::placeholders;

Player::Player() {
  // Set initial key bindings
  keyBinding[sf::Keyboard::Left] = Action::MOVE_LEFT;
  keyBinding[sf::Keyboard::Right] = Action::MOVE_RIGHT;
  keyBinding[sf::Keyboard::Q] = Action::DUPLICATE_BALL;
  keyBinding[sf::Keyboard::W] = Action::SLOWDOWN_BALL;

  // Set initial action bindings
  initializeActions();
}

void Player::handleEvent(const sf::Event &event, CommandQueue &commands) {
  if (event.type == sf::Event::KeyPressed) {
    // Check if pressed key appears in key binding, trigger command if so
    auto found = keyBinding.find(event.key.code);
    if (found != keyBinding.end() && !isRealtimeAction(found->second))
      commands.push(actionBinding[found->second]);

    if (event.key.code == sf::Keyboard::F12) {
      Command command;
      command.category = Category::SCENE;
      command.action = [](SceneNode& node, sf::Time) {
        node.showDebugInfo = !node.showDebugInfo;
      };
      commands.push(command);
    }
  }

  if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
    dragStart.x = event.mouseButton.x;
    dragStart.y = event.mouseButton.y;

    auto ds = dragStart;

    Command command;
    command.category = Category::BALL;
    command.action = derivedAction<Ball>([this, ds](Ball& ball, sf::Time) {
        if(ball.showDebugInfo) {
          ball.setPosition(ds.x, ds.y);
          ball.recieveEvents = false;
          mouseDrag = true;
        }
    });
    commands.push(command);
  }

  if(mouseDrag && event.type == sf::Event::MouseMoved) {
    dragEnd.x = event.mouseMove.x;
    dragEnd.y = event.mouseMove.y;
  }

  if (mouseDrag && event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right) {
    mouseDrag = false;
    auto diff = sf::Vector2f(event.mouseButton.x, event.mouseButton.y) - dragStart;

    if(Vector::length(diff) > 2.f) {
      Command command;
      command.category = Category::BALL;
      command.action = derivedAction<Ball>([diff](Ball& ball, sf::Time) {
          if(ball.showDebugInfo) {
            ball.setVelocity(diff * 3.5f);
            ball.recieveEvents = true;
          }
      });
      commands.push(command);
    }
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
  actionBinding[MOVE_LEFT].category = Category::PADDLE;
  actionBinding[MOVE_LEFT].action = derivedAction<Paddle>([] (Paddle& p, sf::Time) {
    p.accelerate(-1 * p.getMovementSpeed(), 0.f);
  });
  actionBinding[MOVE_RIGHT].category = Category::PADDLE;
  actionBinding[MOVE_RIGHT].action = derivedAction<Paddle>([] (Paddle& p, sf::Time) {
    p.accelerate(1 * p.getMovementSpeed(), 0.f);
  });
  actionBinding[DUPLICATE_BALL].category = Category::DUPLICATE_SKILL;
  actionBinding[DUPLICATE_BALL].action = derivedAction<DuplicateBallSkill>([] (DuplicateBallSkill& skill, sf::Time) {
    skill.activate();
  });

  actionBinding[SLOWDOWN_BALL].category = Category::SLOWMOTION_SKILL;
  actionBinding[SLOWDOWN_BALL].action = derivedAction<SlowmotionSkill>([] (SlowmotionSkill& skill, sf::Time) {
      skill.activate();
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

void Player::setLevel(int level) {
  this->level = level;
}

int Player::getLevel() const {
  return level;
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  if(mouseDrag) {
    sf::Vertex line[] = {
            sf::Vertex(dragStart),
            sf::Vertex(dragEnd)
    };
    target.draw(line, 2, sf::Lines);
  }
}
