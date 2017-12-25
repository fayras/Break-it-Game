#ifndef SFML_TEMPLATE_PAUSESTATE_HPP
#define SFML_TEMPLATE_PAUSESTATE_HPP

#include "../system/State.hpp"
#include "../gui/Container.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class PauseState : public State {
  public:
    PauseState(StateStack& stack, Context context);
    ~PauseState() override;

    void draw() override;
    bool update(sf::Time dt) override;
    bool handleEvent(const sf::Event& event) override;

  private:
    sf::Text pausedText;
    gui::Container guiContainer;
};

#endif //SFML_TEMPLATE_PAUSESTATE_HPP
