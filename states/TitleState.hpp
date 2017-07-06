#ifndef SFML_TEMPLATE_TITLESTATE_HPP
#define SFML_TEMPLATE_TITLESTATE_HPP

#include "../system/State.hpp"
#include "../gui/Container.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class TitleState : public State {
  public:
    TitleState(StateStack& stack, Context context);
    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);

  private:
    sf::Sprite background;
    gui::Container guiContainer;
};


#endif //SFML_TEMPLATE_TITLESTATE_HPP
