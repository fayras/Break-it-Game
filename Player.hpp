#ifndef SFML_TEMPLATE_PLAYER_HPP
#define SFML_TEMPLATE_PLAYER_HPP

#include <SFML/Window/Event.hpp>
#include <map>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include "system/Command.hpp"

class CommandQueue;

class Player : public sf::Drawable {
  public:
    enum Action {
      MOVE_LEFT,
      MOVE_RIGHT,
      DUPLICATE_BALL,
      SLOWDOWN_BALL
    };

    Player();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void handleEvent(const sf::Event& event, CommandQueue& commands);
    void handleRealtimeInput(CommandQueue& commands);
    void assignKey(Action action, sf::Keyboard::Key key);
    sf::Keyboard::Key getAssignedKey(Action action) const;
    void setScore(int score);
    int getScore() const;
    void setLevel(int level);
    int getLevel() const;

  private:
    void initializeActions();
    static bool isRealtimeAction(Action action);

    std::map<sf::Keyboard::Key, Action> keyBinding;
    std::map<Action, Command> actionBinding;
    int score{0};
    int level{0};

    bool mouseDrag{false};
    sf::Vector2f dragStart;
    sf::Vector2f dragEnd;
};

#endif //SFML_TEMPLATE_PLAYER_HPP
