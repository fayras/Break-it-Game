#ifndef BREAK_IT_SETTINGSSTATE_HPP
#define BREAK_IT_SETTINGSSTATE_HPP

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
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
    sf::RectangleShape background;
    gui::Container guiContainer;
    std::map<
        gui::Button::Ptr,
        std::function<void(const sf::Event&, gui::Button*)>
    > options;

    std::map<
        gui::Label::Ptr,
        std::function<void(gui::Label*)>
    > optionLabels;

    void addOption(
        const std::string &label,
        const std::function<void(const sf::Event&, gui::Button*)>& func,
        const std::function<void(gui::Label*)>& func2
    );

    void updateLabels();
};

#endif //BREAK_IT_SETTINGSSTATE_HPP
