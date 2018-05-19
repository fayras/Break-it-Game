#ifndef SFML_TEMPLATE_TITLESTATE_HPP
#define SFML_TEMPLATE_TITLESTATE_HPP

#include "../system/State.hpp"
#include "../gui/Container.hpp"
// #include "../system/Patcher.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

namespace gui {
    class Button;
}

class TitleState : public State {
  public:
    TitleState(StateStack& stack, Context context);

    void draw() override;
    bool update(sf::Time dt) override;
    bool handleEvent(const sf::Event& event) override;

  private:
    sf::Sprite background;
    gui::Container guiContainer;
    // Patcher patcher;
    gui::Button* updateButton;
};


#endif //SFML_TEMPLATE_TITLESTATE_HPP
