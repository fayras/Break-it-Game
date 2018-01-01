#include "SettingsState.hpp"
#include "../system/Utility.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

SettingsState::SettingsState(StateStack &stack, State::Context context)
    : State(stack, context)
{
  addOption("Links", [context](const sf::Event& event, gui::Button* button, gui::Label* label) {
    // sf::Keyboard::Key key = context.player->getAssignedKey(Player::MOVE_LEFT);
    context.player->assignKey(Player::MOVE_LEFT, event.key.code);
    button->deactivate();
    label->setText(String::from(context.player->getAssignedKey(Player::MOVE_LEFT)));
  });

  addOption("Rechts", [context](const sf::Event& event, gui::Button* button, gui::Label* label) {
    // sf::Keyboard::Key key = context.player->getAssignedKey(Player::MOVE_LEFT);
    context.player->assignKey(Player::MOVE_RIGHT, event.key.code);
    button->deactivate();
    label->setText(String::from(context.player->getAssignedKey(Player::MOVE_RIGHT)));
  });

  auto backButton = std::make_shared<gui::Button>(context);
  backButton->setPosition(80.0f, 600.0f);
  backButton->setText("Zurück");
  backButton->setCallback(std::bind(&SettingsState::requestStackPop, this));

  guiContainer.pack(backButton);
}

SettingsState::~SettingsState() = default;

void SettingsState::draw() {
  sf::RenderWindow& window = *getContext().window;
  window.draw(guiContainer);
}

bool SettingsState::update(sf::Time) {
  return false;
}

bool SettingsState::handleEvent(const sf::Event &event) {
  bool isKeyBinding = false;

  for(const auto& option : options) {
    if(option.first.first->active()) {
      isKeyBinding = true;
      if(event.type == sf::Event::KeyReleased) {
        if(event.key.code == sf::Keyboard::Escape) {
          option.first.first->deactivate();
          break;
        }
        option.second(event, option.first.first.get(), option.first.second.get());
      }
      break;
    }
  }

  if(isKeyBinding) {
    // update labels
  } else {
    guiContainer.handleEvent(event);
  }

  return false;
}

void SettingsState::addOption(const std::string &title, const std::function<void(const sf::Event&, gui::Button*, gui::Label*)>& func) {
  auto button = std::make_shared<gui::Button>(context);
  button->setPosition(80.0f, options.size() * 60 + 300);
  button->setText(title);
  button->setToggle(true);

  auto label = std::make_shared<gui::Label>("", *getContext().fonts);
  label->setPosition(300.0f, options.size() * 60 + 300);

  options.insert(std::make_pair(std::make_pair(button, label), func));
  guiContainer.pack(button);
  guiContainer.pack(label);
}
