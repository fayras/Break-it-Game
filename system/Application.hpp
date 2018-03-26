#ifndef SFML_TEMPLATE_GAME_HPP
#define SFML_TEMPLATE_GAME_HPP

#include <SFML/Graphics.hpp>
#include "../nodes/SceneNode.hpp"
#include "CommandQueue.hpp"
#include "../ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"
#include "StateStack.hpp"
#include "MusicPlayer.hpp"
#include "SoundPlayer.hpp"
#include "../Player.hpp"
#include "Statistics.hpp"

class Application {
  public:
    Application();
    void run();

  private:
    void processEvents();
    void update(sf::Time dt);
    void render();

    void registerStates();

    bool isPaused;

    sf::RenderWindow window;

    TextureHolder textures;
    FontHolder fonts;
    MusicPlayer	music;
    SoundPlayer	sounds;
    StateStack stateStack;
    Player player;
    ScoreBoard scoreBoard;
    SettingsParser settings;
    SettingsParser saveData;
    Statistics stats;

    static const sf::Time	TimePerFrame;

    void loadAssets();
};


#endif //SFML_TEMPLATE_GAME_HPP
