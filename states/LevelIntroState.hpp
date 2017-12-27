#ifndef BREAK_IT_LEVELINTROSTATE_HPP
#define BREAK_IT_LEVELINTROSTATE_HPP

#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "../system/State.hpp"
#include "../tween/Tweenable.hpp"

class LevelIntroState : public State, public Tweenable {
  public:
    LevelIntroState(StateStack& stack, Context context);

    void draw() override;
    bool update(sf::Time dt) override;
    bool handleEvent(const sf::Event& event) override;

  private:
    sf::RectangleShape backgroundShape;
    sf::Text pausedText;
    sf::Clock timer;
};

#endif //BREAK_IT_LEVELINTROSTATE_HPP
