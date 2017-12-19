#include "GameOverState.hpp"
#include "../system/ResourceHolder.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "../system/MusicPlayer.hpp"
#include "../gui/Button.hpp"
#include <cmath>

GameOverState::GameOverState(StateStack &stack, State::Context context)
  : State(stack, context)
{
  sf::Font& font = context.fonts->get(Fonts::ARCADE);
  sf::Vector2f windowSize(context.window->getSize());

  scoreText.setFont(font);
  scoreText.setString("Dein Score: " + std::to_string(context.player->getScore()));
  scoreText.setCharacterSize(35);
  sf::FloatRect bounds = scoreText.getLocalBounds();
  scoreText.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
  scoreText.setPosition(0.5f * windowSize.x, 0.6f * windowSize.y);

  auto returnButton = std::make_shared<gui::Button>(context);
  returnButton->setPosition(0.5f * windowSize.x - 200, 0.6f * windowSize.y + 125);
  returnButton->setText("Neues Spiel");
  returnButton->setCallback([this] () {
    requestStackClear();
    requestStackPush(States::GAME);
  });

  auto backToMenuButton = std::make_shared<gui::Button>(context);
  backToMenuButton->setPosition(0.5f * windowSize.x, 0.6f * windowSize.y + 125);
  backToMenuButton->setText("Beenden");
  backToMenuButton->setCallback([this] () {
    requestStackClear();
  });

  guiContainer.pack(returnButton);
  guiContainer.pack(backToMenuButton);

  if(context.player->getLevel() < 0) {
    background.setTexture(context.textures->get(Textures::GAME_WIN_SCREEN));
  } else {
    background.setTexture(context.textures->get(Textures::GAME_OVER_SCREEN));
  }

  getContext().music->setPaused(true);
}

GameOverState::~GameOverState() {
  getContext().music->setPaused(false);
}

void GameOverState::draw() {
  sf::RenderWindow& window = *getContext().window;
  window.setView(window.getDefaultView());

  window.draw(background);
  window.draw(scoreText);
  window.draw(guiContainer);
}

bool GameOverState::update(sf::Time dt) {
  return false;
}

bool GameOverState::handleEvent(const sf::Event &event) {
  guiContainer.handleEvent(event);
  return false;
}
