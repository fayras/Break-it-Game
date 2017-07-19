#include <iostream>
#include "Application.hpp"
#include "../states/GameState.hpp"
#include "../states/TitleState.hpp"
#include "../states/PauseState.hpp"
#include "../states/GameOverState.hpp"

const int FPS = 60;
const sf::Time Application::TimePerFrame = sf::seconds(1.f / FPS);

Application::Application()
  : window(sf::VideoMode(1000, 740), "Break it!"),
    isPaused(false),
    stateStack(State::Context(window, textures, fonts, music, sounds, player))
{
  // window.setFramerateLimit(FPS);

  loadAssets();
  registerStates();
  stateStack.push(States::ID::TITLE);
}

void Application::run() {
  sf::Clock clock;
  sf::Time timeSinceLastUpdate = sf::Time::Zero;
  while (window.isOpen()) {
    sf::Time dt = clock.restart();
    timeSinceLastUpdate += dt;
    while (timeSinceLastUpdate > TimePerFrame) {
      timeSinceLastUpdate -= TimePerFrame;

      processEvents();
      if(!isPaused) update(TimePerFrame);
      if(stateStack.empty()) window.close();
    }
    render();
  }
}

void Application::processEvents() {
  sf::Event event;
  while (window.pollEvent(event)) {
    if(!isPaused) stateStack.handleEvent(event);

    switch (event.type) {
      case sf::Event::GainedFocus:
        isPaused = false;
        break;
      case sf::Event::LostFocus:
        isPaused = true;
        break;
      case sf::Event::Closed:
        window.close();
        break;
      default:
        break;
    }
  }
}

void Application::update(sf::Time dt) {
  stateStack.update(dt);
}

void Application::render() {
  window.clear();
  stateStack.draw();
  window.setView(window.getDefaultView());
  window.display();
}

void Application::registerStates() {
  stateStack.registerState<TitleState>(States::ID::TITLE);
  stateStack.registerState<GameState>(States::ID::GAME);
  stateStack.registerState<PauseState>(States::ID::PAUSE);
  stateStack.registerState<GameOverState>(States::ID::GAME_OVER);
}

void Application::loadAssets() {
  fonts.load(Fonts::ID::MAIN, "./assets/fonts/Roboto.ttf");
  fonts.load(Fonts::ID::ARCADE, "./assets/fonts/Arcade.ttf");
  textures.load(Textures::BUTTON_NORMAL, "assets/textures/buttonDefault.png");
  textures.load(Textures::BUTTON_PRESSED, "assets/textures/buttonDefault.png");
  textures.load(Textures::BUTTON_SELECTED, "assets/textures/buttonSelected.png");
  textures.load(Textures::TITLE, "assets/textures/title.png");
  textures.load(Textures::GAME_OVER_SCREEN, "assets/textures/game_over_screen.png");
}
