#include "GameOverState.hpp"
#include "../system/ResourceHolder.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "../system/MusicPlayer.hpp"
#include "../gui/Button.hpp"
#include "../gui/TextInput.hpp"
#include <cmath>

GameOverState::GameOverState(StateStack &stack, State::Context context)
  : State(stack, context)
{
  sf::Font& font = context.fonts->get(Fonts::PIXEL);
  sf::Vector2f windowSize(context.window->getSize());

  scoreText.setFont(font);

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

void GameOverState::updateScore() {
  sf::Vector2f windowSize(context.window->getSize());
  std::string scoreString;
  for(const auto& score : context.scoreBoard->getEntries()) {
    scoreString += score.name + " " + std::to_string(score.score) + "\n";
  }
  scoreText.setString(scoreString);
  scoreText.setCharacterSize(35);
  sf::FloatRect bounds = scoreText.getLocalBounds();
  scoreText.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
  scoreText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);
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
  if(!showedHighscore && context.scoreBoard->worthInsert(context.player->getScore())) {
    requestStackPush(States::NEW_HIGHSCORE);
    showedHighscore = true;
  }
  updateScore();
  return false;
}

bool GameOverState::handleEvent(const sf::Event &event) {
  guiContainer.handleEvent(event);
  return false;
}
