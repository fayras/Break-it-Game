#include "GameState.hpp"
#include "../system/ResourceHolder.hpp"
#include "../nodes/SoundNode.hpp"
#include "../system/MusicPlayer.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

GameState::GameState(StateStack &stack, const State::Context &context)
    : State(stack, context),
      world(*context.window, *context.fonts, *context.sounds),
      player(*context.player)
{
  context.music->play(Music::GAME);
  world.setLevel(context.saveData->get("playerLevel", 0));
  world.setScore(context.saveData->get("playerScore", 0));
}

void GameState::draw() {
  world.draw();
}

bool GameState::update(sf::Time dt) {
  world.update(dt);
  CommandQueue& commands = world.getCommandQueue();
  player.handleRealtimeInput(commands);

  if(world.destroyed()) {
    player.setScore(world.getScore());
    requestStackPush(States::GAME_OVER);
  }

  if(world.reachedEnd()) {
    player.setScore(world.getScore());
    player.setLevel(-1);
    requestStackPush(States::GAME_OVER);
  }

  return true;
}

bool GameState::handleEvent(const sf::Event &event) {
  // Game input handling
  CommandQueue& commands = world.getCommandQueue();
  player.handleEvent(event, commands);

  // Escape pressed, trigger the pause screen
  if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    requestStackPush(States::PAUSE);

  return true;
}
