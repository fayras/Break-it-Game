#include "GameState.hpp"
#include "../system/ResourceHolder.hpp"
#include "../nodes/SoundNode.hpp"
#include "../system/MusicPlayer.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

GameState::GameState(StateStack &stack, const State::Context &context)
    : State(stack, context),
      world(*context.window, *context.fonts, *context.sounds, *context.settings),
      player(*context.player)
{
  context.music->play(Music::GAME);
  world.setLevel(context.saveData->getLevel());
  world.setScore(context.saveData->getScore());
}

void GameState::draw() {
  world.draw();
}

bool GameState::update(sf::Time dt) {
  world.update(dt);
  CommandQueue& commands = world.getCommandQueue();
  player.handleRealtimeInput(commands);

  if(getContext().saveData->skillUnlocked(Skills::ID::DUPLICATE_BALL)) {
    world.unlockSkill(Skills::ID::DUPLICATE_BALL);
  }
  if(getContext().saveData->skillUnlocked(Skills::ID::SLOWMOTION)) {
    world.unlockSkill(Skills::ID::SLOWMOTION);
  }

  if(world.destroyed()) {
    player.setScore(world.getScore());
    context.saveData->setLevel(0);
    context.saveData->setScore(world.getScore());
    context.saveData->save();
    requestStackPush(States::GAME_OVER);
  } else if(world.reachedEnd()) {
    player.setScore(world.getScore());
    player.setLevel(-1);
    context.saveData->setLevel(0);
    context.saveData->setScore(world.getScore());
    context.saveData->save();
    requestStackPush(States::GAME_OVER);
  } else if(worldLevel != world.getCurrentLevel()) {
    worldLevel = world.getCurrentLevel();
    context.saveData->setLevel(worldLevel);
    context.saveData->setScore(world.getScore());
    context.saveData->save();
  } else if(world.finishedLevel()) {
    requestStackPush(States::LEVEL_FINISHED);
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
