#include "NewHighscoreState.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <cmath>
#include "../system/ResourceHolder.hpp"
#include "../gui/TextInput.hpp"

NewHighscoreState::NewHighscoreState(StateStack &stack, State::Context context)
    : State(stack, context)
{
  sf::Font& font = context.fonts->get(Fonts::MAIN);
  sf::Vector2f windowSize(context.window->getSize());

  pausedText.setFont(font);
  pausedText.setString("Du hast einen neuen Highscore erreicht: " + std::to_string(context.player->getScore()));
  pausedText.setCharacterSize(35);
  sf::FloatRect bounds = pausedText.getLocalBounds();
  pausedText.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
  pausedText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);

  backgroundShape.setFillColor(sf::Color(0, 0, 0, 200));
  backgroundShape.setSize(sf::Vector2f(windowSize.x, windowSize.y));

  auto nameInput = std::make_shared<gui::TextInput>(*context.fonts);
  nameInput->setPlaceholder("Name eingeben...");
  nameInput->setSize(200, 30);
  nameInput->setPosition(0.5f * windowSize.x - 100, 0.6f * windowSize.y);
  nameInput->setCallback([this, context] (std::string input) {
    if(input.empty()) {
      return;
    }

    context.scoreBoard->addEntry(input, context.player->getScore());
    context.scoreBoard->save();
    requestStackPop();
  });

  guiContainer.pack(nameInput);
}

void NewHighscoreState::draw() {
  sf::RenderWindow& window = *getContext().window;
  window.setView(window.getDefaultView());

  window.draw(backgroundShape);
  window.draw(pausedText);
  window.draw(guiContainer);
}

bool NewHighscoreState::update(sf::Time dt) {
  guiContainer.update(dt);
  return false;
}

bool NewHighscoreState::handleEvent(const sf::Event &event) {
  guiContainer.handleEvent(event);
  if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
    requestStackPop();
  }
  return false;
}
