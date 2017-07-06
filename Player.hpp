#ifndef SFML_TEMPLATE_PLAYER_HPP
#define SFML_TEMPLATE_PLAYER_HPP

#include <SFML/Window/Event.hpp>
#include <map>
#include "system/Command.hpp"

class CommandQueue;

class Player {
  public:
    enum Action {
      MOVE_LEFT,
      MOVE_RIGHT
    };

    Player();

    void handleEvent(const sf::Event& event, CommandQueue& commands);
    void handleRealtimeInput(CommandQueue& commands);
    void assignKey(Action action, sf::Keyboard::Key key);
    sf::Keyboard::Key getAssignedKey(Action action) const;

  private:
    void initializeActions();
    static bool isRealtimeAction(Action action);

    std::map<sf::Keyboard::Key, Action> keyBinding;
    std::map<Action, Command> actionBinding;
};

#endif //SFML_TEMPLATE_PLAYER_HPP
