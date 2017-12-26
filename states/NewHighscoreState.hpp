#ifndef BREAK_IT_NEWHIGHSCORESTATE_HPP
#define BREAK_IT_NEWHIGHSCORESTATE_HPP

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "../system/State.hpp"
#include "../gui/Container.hpp"

class NewHighscoreState : public State {
  public:
    NewHighscoreState(StateStack& stack, Context context);

    void draw() override;
    bool update(sf::Time dt) override;
    bool handleEvent(const sf::Event& event) override;

  private:
    sf::RectangleShape backgroundShape;
    sf::Text pausedText;
    gui::Container guiContainer;
};

#endif //BREAK_IT_NEWHIGHSCORESTATE_HPP
