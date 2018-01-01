#include "SettingsState.hpp"
#include "../system/Utility.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include "../system/MusicPlayer.hpp"
#include "../system/SoundPlayer.hpp"
#include <algorithm>

SettingsState::SettingsState(StateStack &stack, State::Context context)
    : State(stack, context), background(sf::Vector2f(context.window->getSize().x, context.window->getSize().y))
{
  background.setFillColor(sf::Color(0, 0, 0, 170));

  addOption("Links", [context](const sf::Event& event, gui::Button* button) {
    context.player->assignKey(Player::MOVE_LEFT, event.key.code);
    context.settings->set("key_left", (int) event.key.code);
    button->deactivate();
  }, [context](gui::Label* label) {
    label->setText(String::from(context.player->getAssignedKey(Player::MOVE_LEFT)));
  });

  addOption("Rechts", [context](const sf::Event& event, gui::Button* button) {
    context.player->assignKey(Player::MOVE_RIGHT, event.key.code);
    context.settings->set("key_right", (int) event.key.code);
    button->deactivate();
  }, [context](gui::Label* label) {
    label->setText(String::from(context.player->getAssignedKey(Player::MOVE_RIGHT)));
  });

  addOption("Musik", [context](const sf::Event& event, gui::Button* button) {
    if(event.type == sf::Event::KeyReleased) {
      float newVolume = context.music->getVolume();
      if(event.key.code == sf::Keyboard::Left) {
        newVolume -= 10;
      } else if(event.key.code == sf::Keyboard::Right) {
        newVolume += 10;
      } else if(event.key.code == sf::Keyboard::Escape) {
        button->deactivate();
      }
      context.music->setVolume(std::clamp(newVolume, 0.0f, 100.0f));
    }
    context.settings->set("music_volume", context.music->getVolume());
  }, [context](gui::Label* label) {
    label->setText(std::to_string(context.music->getVolume()));
  });

  addOption("Sounds", [context](const sf::Event& event, gui::Button* button) {
    if(event.type == sf::Event::KeyReleased) {
      float newVolume = context.sounds->getMasterVolume();
      if(event.key.code == sf::Keyboard::Left) {
        newVolume -= 10;
      } else if(event.key.code == sf::Keyboard::Right) {
        newVolume += 10;
      } else if(event.key.code == sf::Keyboard::Escape) {
        button->deactivate();
      }
      context.sounds->setMasterVolume(std::clamp(newVolume, 0.0f, 100.0f));
    }
    context.settings->set("sounds_volume", context.sounds->getMasterVolume());
  }, [context](gui::Label* label) {
    label->setText(std::to_string(context.sounds->getMasterVolume()));
  });

  auto backButton = std::make_shared<gui::Button>(context);
  backButton->setPosition(80.0f, 600.0f);
  backButton->setText("Zurück");
  backButton->setCallback(std::bind(&SettingsState::requestStackPop, this));

  guiContainer.pack(backButton);
  updateLabels();
}

SettingsState::~SettingsState() {
  context.settings->saveToFile();
};

void SettingsState::draw() {
  sf::RenderWindow& window = *getContext().window;
  window.draw(background);
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
        option.second(event, option.first.get());
      }
      break;
    }
  }

  if(isKeyBinding) {
    updateLabels();
  } else {
    guiContainer.handleEvent(event);
  }

  return false;
}

void SettingsState::addOption(
    const std::string &title,
    const std::function<void(const sf::Event&, gui::Button*)>& func,
    const std::function<void(gui::Label*)>& func2
) {
  auto button = std::make_shared<gui::Button>(context);
  button->setPosition(80.0f, options.size() * 60 + 300);
  button->setText(title);
  button->setToggle(true);

  auto label = std::make_shared<gui::Label>("", *getContext().fonts);
  label->setPosition(300.0f, options.size() * 60 + 300 + 15);

  options.insert(std::make_pair(button, func));
  optionLabels.insert(std::make_pair(label, func2));
  guiContainer.pack(button);
  guiContainer.pack(label);
}

void SettingsState::updateLabels() {
  for(const auto& label : optionLabels) {
    label.second(label.first.get());
  }
}
