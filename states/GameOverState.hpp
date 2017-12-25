#ifndef BREAK_IT_GAMEOVERSTATE_HPP
#define BREAK_IT_GAMEOVERSTATE_HPP

#include "../system/State.hpp"
#include "../gui/Container.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class GameOverState : public State {
  public:
    GameOverState(StateStack& stack, Context context);
    ~GameOverState() override;

    void draw() override;
    bool update(sf::Time dt) override;
    bool handleEvent(const sf::Event& event) override;

  private:
    sf::Sprite background;
    sf::Text pausedText;
    sf::Text scoreText;
    gui::Container guiContainer;
};


#endif //BREAK_IT_GAMEOVERSTATE_HPP
