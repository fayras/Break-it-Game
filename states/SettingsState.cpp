#include "SettingsState.hpp"
#include "../system/Utility.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include "../system/MusicPlayer.hpp"
#include "../system/SoundPlayer.hpp"

float clamp(float n, float lower, float upper) {
  return std::max(lower, std::min(n, upper));
}

SettingsState::SettingsState(StateStack &stack, State::Context context)
    : State(stack, context),
      background(sf::Vector2f(context.window->getSize().x, context.window->getSize().y)),
      col(0), row(0)
{
  background.setFillColor(sf::Color(0, 0, 0, 170));

  addOption("Musik", [context](const sf::Event& event, gui::Button* button) {
    if(event.type == sf::Event::KeyReleased) {
      float newVolume = context.music->getVolume();
      if(event.key.code == sf::Keyboard::Left) {
        newVolume -= 10;
      } else if(event.key.code == sf::Keyboard::Right) {
        newVolume += 10;
      } else if(event.key.code == sf::Keyboard::Return) {
        context.settings->set("music_volume", context.music->getVolume());
        button->deactivate();
      } else if(event.key.code == sf::Keyboard::Escape) {
        context.music->setVolume(context.settings->get("music_volume", 100.0f));
        return;
      }

      context.music->setVolume(clamp(newVolume, 0.0f, 100.0f));
    }
  }, [context](gui::Label* label) {
    label->setText(std::to_string((int) context.music->getVolume()));
  });

  addOption("Sounds", [context](const sf::Event& event, gui::Button* button) {
    if(event.type == sf::Event::KeyReleased) {
      float newVolume = context.sounds->getMasterVolume();
      if(event.key.code == sf::Keyboard::Left) {
        newVolume -= 10;
      } else if(event.key.code == sf::Keyboard::Right) {
        newVolume += 10;
      } else if(event.key.code == sf::Keyboard::Return) {
        context.settings->set("sounds_volume", context.sounds->getMasterVolume());
        button->deactivate();
      } else if(event.key.code == sf::Keyboard::Escape) {
        context.sounds->setMasterVolume(context.settings->get("sounds_volume", 100.0f));
        return;
      }
      context.sounds->setMasterVolume(clamp(newVolume, 0.0f, 100.0f));
    }
  }, [context](gui::Label* label) {
    label->setText(std::to_string((int) context.sounds->getMasterVolume()));
  });

  addOption("Nach Updates suchen", [context]() {
      context.settings->set("checkForUpdates", !context.settings->get("checkForUpdates", true));
  }, [context](gui::Label* label) {
      if(context.settings->get("checkForUpdates", true)) {
          label->setText("Ja");
      } else {
          label->setText("Nein");
      }
  });

  newColumn();

  addOption("Links", [context](const sf::Event& event, gui::Button* button) {
      if(event.key.code != sf::Keyboard::Escape) {
        context.player->assignKey(Player::MOVE_LEFT, event.key.code);
        context.settings->set("key_left", (int) event.key.code);
      }
      button->deactivate();
  }, [context](gui::Label* label) {
      label->setText(String::from(context.player->getAssignedKey(Player::MOVE_LEFT)));
  });

  addOption("Rechts", [context](const sf::Event& event, gui::Button* button) {
      if(event.key.code != sf::Keyboard::Escape) {
        context.player->assignKey(Player::MOVE_RIGHT, event.key.code);
        context.settings->set("key_right", (int) event.key.code);
      }
      button->deactivate();
  }, [context](gui::Label* label) {
      label->setText(String::from(context.player->getAssignedKey(Player::MOVE_RIGHT)));
  });

  addOption("Skill 1", [context](const sf::Event& event, gui::Button* button) {
      if(event.key.code != sf::Keyboard::Escape) {
        context.player->assignKey(Player::DUPLICATE_BALL, event.key.code);
        context.settings->set("key_skill_1", (int) event.key.code);
      }
      button->deactivate();
  }, [context](gui::Label* label) {
      label->setText(String::from(context.player->getAssignedKey(Player::DUPLICATE_BALL)));
  });

  auto backButton = std::make_shared<gui::Button>(context);
  backButton->setPosition(80.0f, 600.0f);
  backButton->setText(L"Zurück");
  backButton->setShortcut(sf::Keyboard::Escape);
  backButton->setCallback(std::bind(&SettingsState::requestStackPop, this));

  auto settingsLabel = std::make_shared<gui::Label>("Einstellungen", context.fonts->get(Fonts::MAIN));
  settingsLabel->setSize(70);
  settingsLabel->setPosition(50, 100);

  guiContainer.pack(backButton);
  guiContainer.pack(settingsLabel);
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

bool SettingsState::update(sf::Time dt) {
  guiContainer.update(dt);
  return false;
}

bool SettingsState::handleEvent(const sf::Event &event) {
  bool isKeyBinding = false;

  for(const auto& option : options) {
    if(option.first->active()) {
      isKeyBinding = true;
      if(event.type == sf::Event::KeyReleased) {
        option.second(event, option.first.get());
        if(event.key.code == sf::Keyboard::Escape) {
          option.first->deactivate();
        }
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
  button->setText(title);
  button->setToggle(true);

  auto label = std::make_shared<gui::Label>("", *getContext().fonts);

  options.insert(std::make_pair(button, func));
  optionLabels.insert(std::make_pair(label, func2));
  setPositionsAndPack(button, label);
}

void SettingsState::addOption(
        const std::string &title,
        const std::function<void()>& func,
        const std::function<void(gui::Label*)>& func2
) {
    auto button = std::make_shared<gui::Button>(context);
    button->setText(title);
    button->setCallback([this, func] () {
        func();
        updateLabels();
    });

    auto label = std::make_shared<gui::Label>("", *getContext().fonts);
    optionLabels.insert(std::make_pair(label, func2));
    setPositionsAndPack(button, label);
}

void SettingsState::updateLabels() {
  for(const auto& label : optionLabels) {
    label.second(label.first.get());
  }
}

void SettingsState::newColumn() {
  col++;
  row = 0;
}

void SettingsState::setPositionsAndPack(std::shared_ptr<gui::Button> &button, std::shared_ptr<gui::Label> &label) {
    button->setPosition(80.0f + col * 450.f, row * 60.f + 240.f);
    label->setPosition(420.0f + col * 450.f, row * 58.f + 240.f);
    guiContainer.pack(button);
    guiContainer.pack(label);
    row++;
}
