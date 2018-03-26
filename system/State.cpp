#include "State.hpp"
#include "StateStack.hpp"

State::Context::Context(
  sf::RenderWindow& window, 
  TextureHolder& textures, 
  FontHolder& fonts,
  MusicPlayer& music,
  SoundPlayer& sounds,
  Player& player,
  ScoreBoard& scoreBoard,
  SettingsParser& settings,
  SettingsParser& saveData
)
  : window(&window),
    textures(&textures),
    fonts(&fonts),
    music(&music),
    sounds(&sounds),
    player(&player),
    scoreBoard(&scoreBoard),
    settings(&settings),
    saveData(&saveData)
{} 

State::State(StateStack& stack, Context context) 
  : stack(&stack), context(context)
{}

State::~State() = default;

void State::requestStackPush(States::ID id) {
  stack->push(id);
}

void State::requestStackPop() {
  stack->pop();
}

void State::requestStackClear() {
  stack->clear();
}

State::Context State::getContext() const {
  return context;
}
