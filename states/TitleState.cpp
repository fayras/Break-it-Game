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
  btPlay->setText("Spielen");
  btPlay->setCallback([this] () {
    requestStackPop();
    requestStackPush(States::ID::GAME);
  });

  auto btSettings = std::make_shared<gui::Button>(context);
  btSettings->setPosition(200, 0);
  btSettings->setText("Einstellungen");
  btSettings->setCallback([this] () {
    requestStackPush(States::ID::SETTINGS);
  });

  auto btExit = std::make_shared<gui::Button>(context);
  btExit->setPosition(400, 0);
  btExit->setText("Beenden");
  btExit->setCallback([this] () {
    requestStackPop();
  });

  guiContainer.move(300, 500);
  guiContainer.pack(btPlay);
  guiContainer.pack(btSettings);
  guiContainer.pack(btExit);
}

void TitleState::draw() {
  sf::RenderWindow& window = *getContext().window;
  window.setView(window.getDefaultView());
  window.draw(background);
  window.draw(guiContainer);
}

bool TitleState::update(sf::Time dt) {
  return true;
}

bool TitleState::handleEvent(const sf::Event &event) {
  guiContainer.handleEvent(event);
  return true;
}
