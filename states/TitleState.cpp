#include "TitleState.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include "../system/ResourceHolder.hpp"
#include "../gui/Button.hpp"
#include "../system/MusicPlayer.hpp"

TitleState::TitleState(StateStack &stack, State::Context context)
    : State(stack, context),
      guiContainer(),
      background(context.textures->get(Textures::TITLE))
{
  context.music->play(Music::TITLE);

  auto btPlay = std::make_shared<gui::Button>(context);
  btPlay->setPosition(0, 0);
  btPlay->setText("Neues Spiel");
  btPlay->setCallback([this] () {
    this->context.saveData->set("player_level", 0);
    this->context.saveData->set("player_score", 0);
    requestStackPop();
    requestStackPush(States::ID::GAME);
  });

  auto btSettings = std::make_shared<gui::Button>(context);
  btSettings->setPosition(0, 60);
  btSettings->setText("Einstellungen");
  btSettings->setCallback([this] () {
    requestStackPush(States::ID::SETTINGS);
  });

  auto btExit = std::make_shared<gui::Button>(context);
  btExit->setPosition(0, 120);
  btExit->setText("Beenden");
  btExit->setShortcut(sf::Keyboard::Escape);
  btExit->setCallback([this] () {
    requestStackPop();
  });

  guiContainer.move(120, 450);
  guiContainer.pack(btPlay);
  guiContainer.pack(btSettings);
  guiContainer.pack(btExit);

  if(context.settings->get("checkForUpdates", true)) {
    patcher.fetch();
    auto btUpdate = std::make_shared<gui::Button>(context);
    updateButton = btUpdate.get();
    btUpdate->setPosition(440, 220);
    btUpdate->setText("");
    btUpdate->setCallback([this]() {
        if (patcher.getStatus() == Patcher::Status::READY_TO_DOWNLOAD) {
          patcher.download();
        } else if (patcher.getStatus() == Patcher::Status::DONE) {
          patcher.patch();
          requestStackClear();
        }
    });
    guiContainer.pack(btUpdate);
  }

  // Wenn es ein Savegame gibt
  if(context.saveData->exists()) {
    auto btContinue = std::make_shared<gui::Button>(context);
    btContinue->setPosition(0, -60);
    btContinue->setText("Fortsetzen");
    btContinue->setCallback([this]() {
      requestStackPop();
      requestStackPush(States::ID::GAME);
    });
    guiContainer.pack(btContinue);
  }
  // }

}

void TitleState::draw() {
  sf::RenderWindow& window = *getContext().window;
  window.setView(window.getDefaultView());
  window.draw(background);
  window.draw(guiContainer);
}

bool TitleState::update(sf::Time dt) {
  if(updateButton != nullptr && context.settings->get("checkForUpdates", true)) {
    switch(patcher.getStatus()) {
      case Patcher::Status::FAILED:
        updateButton->setText("Update fehlgeschlagen");
            break;
      case Patcher::Status::NO_UPDATE:
        updateButton->setText("Kein Update gefunden");
            break;
      case Patcher::Status::FETCHING_INFO:
        updateButton->setText("Suche Aktualisierungen...");
            break;
      case Patcher::Status::READY_TO_DOWNLOAD:
        updateButton->setText("Update herunterladen");
            break;
      case Patcher::Status::DOWNLOADING:
        updateButton->setText("Update wird heruntergeladen...");
            break;
      case Patcher::Status::DOWNLOAD_DONE:
        updateButton->setText("Update heruntergeladen");
            break;
      case Patcher::Status::UPDATING:
        updateButton->setText("Update wird extrahiert...");
            break;
      case Patcher::Status::DONE:
        updateButton->setText("Update installieren");
            break;
      default: break;
    }
  }

  guiContainer.update(dt);
  return true;
}

bool TitleState::handleEvent(const sf::Event &event) {
  guiContainer.handleEvent(event);
  return true;
}
