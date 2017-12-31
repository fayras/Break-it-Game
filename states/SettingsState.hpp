#ifndef BREAK_IT_SETTINGSSTATE_HPP
#define BREAK_IT_SETTINGSSTATE_HPP

#include <SFML/Graphics/Text.hpp>
#include "../system/State.hpp"
#include "../gui/Container.hpp"
#include "../gui/Button.hpp"
#include "../gui/Label.hpp"

class SettingsState : public State {
  public:
    SettingsState(StateStack& stack, Context context);
    ~SettingsState() override;

    void draw() override;
    bool update(sf::Time dt) override;
    bool handleEvent(const sf::Event& event) override;

  private:
    sf::Text pausedText;
    gui::Container guiContainer;
    std::map<gui::Button::Ptr, std::function<void(const sf::Event&)>> options;
    std::vector<gui::Label::Ptr> bindingLabels;
};

#endif //BREAK_IT_SETTINGSSTATE_HPP
