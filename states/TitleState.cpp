#include "TitleState.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include "../system/ResourceHolder.hpp"
#include "../gui/Button.hpp"

TitleState::TitleState(StateStack &stack, State::Context context)
    : State(stack, context),
      guiContainer(),
      background(context.textures->get(Textures::TITLE))
{
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

    auto btUpdate = std::make_shared<gui::Button>(context);
    updateButton = btUpdate.get();
    btUpdate->setPosition(440, 220);
    btUpdate->setText("");
    btUpdate->setCallback([this] () {
        patcher.download();
    });

  guiContainer.move(120, 450);
  guiContainer.pack(btPlay);
  guiContainer.pack(btSettings);
  guiContainer.pack(btExit);
  guiContainer.pack(btUpdate);

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
  switch(patcher.getStatus()) {
    case Patcher::Status::FAILED: break;
    case Patcher::Status::IDLE:break;
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
      updateButton->setText("Update wird installiert...");
      break;
    case Patcher::Status::DONE:
      updateButton->setText("Kein Update gefunden");
      break;
  }
  guiContainer.update(dt);
  return true;
}

bool TitleState::handleEvent(const sf::Event &event) {
  guiContainer.handleEvent(event);
  return true;
}
