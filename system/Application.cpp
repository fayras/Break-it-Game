#include <iostream>
#include "Application.hpp"
#include "../states/GameState.hpp"
#include "../states/TitleState.hpp"
#include "../states/PauseState.hpp"

const sf::Time Application::TimePerFrame = sf::seconds(1.f/60.f);

Application::Application()
  : window(sf::VideoMode(1000, 740), "Hangman"),
    isPaused(false),
    stateStack(State::Context(window, textures, fonts, music, sounds))
{
  window.setFramerateLimit(30);

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
}
