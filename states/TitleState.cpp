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
    btUpdate->setPosition(600, 120);
    btUpdate->setText("Update");
    btUpdate->disable();
    btUpdate->setCallback([this] () {
        // requestStackPop();
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
  guiContainer.update(dt);
  return true;
}

bool TitleState::handleEvent(const sf::Event &event) {
  guiContainer.handleEvent(event);
  return true;
}
