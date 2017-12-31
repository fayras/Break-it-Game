#include "SettingsState.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

SettingsState::SettingsState(StateStack &stack, State::Context context)
    : State(stack, context)
{
  auto buttonMoveLeft = std::make_shared<gui::Button>(context);
  buttonMoveLeft->setPosition(80.0f, 300.0f);
  buttonMoveLeft->setText("Links");
  buttonMoveLeft->setToggle(true);
  gui::Button* buttonMoveLeftPointer = buttonMoveLeft.get();

  options.insert(std::make_pair(
    buttonMoveLeft,
    [context, buttonMoveLeftPointer](const sf::Event& event) {
      // sf::Keyboard::Key key = context.player->getAssignedKey(Player::MOVE_LEFT);
      context.player->assignKey(Player::MOVE_LEFT, event.key.code);
      buttonMoveLeftPointer->deactivate();
    }
  ));

  auto backButton = std::make_shared<gui::Button>(context);
  backButton->setPosition(80.0f, 600.0f);
  backButton->setText("Zurück");
  backButton->setCallback(std::bind(&SettingsState::requestStackPop, this));

  guiContainer.pack(buttonMoveLeft);
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
    if(option.first->active()) {
      isKeyBinding = true;
      if(event.type == sf::Event::KeyReleased) {
        if(event.key.code == sf::Keyboard::Escape) {
          option.first->deactivate();
          break;
        }
        option.second(event);
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
