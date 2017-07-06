#include "State.hpp"
#include "StateStack.hpp"

State::Context::Context(
  sf::RenderWindow& window, 
  TextureHolder& textures, 
  FontHolder& fonts,
  MusicPlayer& music,
  SoundPlayer& sounds,
  Player& player
) 
  : window(&window), textures(&textures), fonts(&fonts), music(&music), sounds(&sounds), player(&player)
{} 

State::State(StateStack& stack, Context context) 
  : stack(&stack), context(context)
{}

State::~State() {}

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
