#include "TitleState.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include "../system/ResourceHolder.hpp"
#include "../gui/Button.hpp"

TitleState::TitleState(StateStack &stack, State::Context context)
    : State(stack, context),
      guiContainer()
{
  auto btPlay = std::make_shared<gui::Button>(context);
  btPlay->setPosition(100, 100);
  btPlay->setText("Spielen");
  btPlay->setCallback([this] () {
    requestStackPop();
    requestStackPush(States::ID::GAME);
  });

  auto btExit = std::make_shared<gui::Button>(context);
  btExit->setPosition(100, 200);
  btExit->setText("Beenden");
  btExit->setCallback([this] () {
    requestStackPop();
  });

  guiContainer.pack(btPlay);
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
