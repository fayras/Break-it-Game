#ifndef BREAK_IT_LEVELINTROSTATE_HPP
#define BREAK_IT_LEVELINTROSTATE_HPP

#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "../system/State.hpp"

class LevelIntroState : public State {
  public:
    LevelIntroState(StateStack& stack, Context context);

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);

  private:
    sf::RectangleShape backgroundShape;
    sf::Text pausedText;
    sf::Clock timer;
};

#endif //BREAK_IT_LEVELINTROSTATE_HPP
