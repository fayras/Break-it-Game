#include "PauseState.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../gui/Button.hpp"
#include "../system/MusicPlayer.hpp"
#include "../system/ResourceHolder.hpp"
#include <cmath>

PauseState::PauseState(StateStack &stack, State::Context context)
  : State(stack, context), pausedText(), guiContainer()
{
  sf::Font& font = context.fonts->get(Fonts::MAIN);
  sf::Vector2f windowSize(context.window->getSize());

  pausedText.setFont(font);
  pausedText.setString("Pause");
  pausedText.setCharacterSize(70);
  sf::FloatRect bounds = pausedText.getLocalBounds();
  pausedText.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
  pausedText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);

  auto returnButton = std::make_shared<gui::Button>(context);
  returnButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 75);
  returnButton->setText("Fortsetzen");
  returnButton->setCallback([this] () {
    requestStackPop();
  });

  auto restartLevelButton = std::make_shared<gui::Button>(context);
  restartLevelButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 125);
  restartLevelButton->setText("Level neustarten");
  restartLevelButton->setCallback([this] () {
    requestStackClear();
    requestStackPush(States::ID::GAME);
  });

  auto backToMenuButton = std::make_shared<gui::Button>(context);
  backToMenuButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 175);
  backToMenuButton->setText("Beenden");
  backToMenuButton->setCallback([this] () {
    requestStackClear();
  });

  guiContainer.pack(returnButton);
  guiContainer.pack(restartLevelButton);
  guiContainer.pack(backToMenuButton);

  getContext().music->setPaused(true);
}

PauseState::~PauseState() {
  getContext().music->setPaused(false);
}

void PauseState::draw() {
  sf::RenderWindow& window = *getContext().window;
  window.setView(window.getDefaultView());

  sf::RectangleShape backgroundShape;
  backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
  backgroundShape.setSize(window.getView().getSize());

  window.draw(backgroundShape);
  window.draw(pausedText);
  window.draw(guiContainer);
}

bool PauseState::update(sf::Time dt) {
  guiContainer.update(dt);
  return false;
}

bool PauseState::handleEvent(const sf::Event &event) {
  if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
    requestStackPop();
    return false;
  }
  guiContainer.handleEvent(event);
  return false;
}
