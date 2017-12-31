#ifndef SFML_TEMPLATE_STATE_HPP
#define SFML_TEMPLATE_STATE_HPP

#include "../states/StateIdentifiers.hpp"
#include "../ResourceIdentifiers.hpp"
#include "../Player.hpp"
#include "../ScoreBoard.hpp"
#include "SettingsParser.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>

namespace sf {
  class RenderWindow;
}

class StateStack;
class MusicPlayer;
class SoundPlayer;

class State {
  public:
    typedef std::unique_ptr<State> Ptr;

    struct Context {
      Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, MusicPlayer& music, SoundPlayer& sounds, Player& player, ScoreBoard& scoreBoard, SettingsParser& setings);
      sf::RenderWindow* window;
      TextureHolder* textures;
      FontHolder* fonts;
      MusicPlayer* music;
      SoundPlayer* sounds;
      Player* player;
      ScoreBoard* scoreBoard;
      SettingsParser* settings;
    };

    State(StateStack& stack, Context context);
    virtual ~State();

    virtual void draw() = 0;
    virtual bool update(sf::Time dt) = 0;
    virtual bool handleEvent(const sf::Event& event) = 0;

  protected:
    void requestStackPush(States::ID stateID);
    void requestStackPop();
    void requestStackClear();
    Context getContext() const;

  protected:
    StateStack* stack;
    Context context;
};

#endif //SFML_TEMPLATE_STATE_HPP