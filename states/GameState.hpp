#ifndef SFML_TEMPLATE_GAMESTATE_HPP
#define SFML_TEMPLATE_GAMESTATE_HPP

#include "../system/State.hpp"
#include "../World.hpp"
#include "../Player.hpp"

class GameState : public State {
  public:
    GameState(StateStack &stack, const Context &context);

    void draw() override;
    bool update(sf::Time dt) override;
    bool handleEvent(const sf::Event &event) override;

  private:
    World world;
    Player& player;
    int worldLevel{-1};
};


#endif //SFML_TEMPLATE_GAMESTATE_HPP
