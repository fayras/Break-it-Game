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
    std::map<
        std::pair<gui::Button::Ptr, gui::Label::Ptr>,
        std::function<void(const sf::Event&, gui::Button*, gui::Label*)>
    > options;

    void addOption(
        const std::string &label,
        const std::function<void(const sf::Event&, gui::Button*, gui::Label*)>& func
    );
};

#endif //BREAK_IT_SETTINGSSTATE_HPP
