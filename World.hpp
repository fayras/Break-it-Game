#ifndef SFML_TEMPLATE_WORLD_HPP
#define SFML_TEMPLATE_WORLD_HPP

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include "system/CommandQueue.hpp"
#include "ResourceIdentifiers.hpp"
#include "system/SoundPlayer.hpp"
#include "nodes/SceneNode.hpp"
#include "entities/Ball.hpp"
#include "entities/Paddle.hpp"
#include "entities/Block.hpp"
#include "Wall.hpp"
#include "nodes/SpriteNode.hpp"
#include "Score.hpp"
#include "nodes/ParticleNode.hpp"
#include "Level.hpp"
#include "Life.hpp"
#include "system/CollisionManager.hpp"
#include "system/SettingsParser.hpp"
#include "shaders/BloomEffect.hpp"

namespace sf {
  class RenderTarget;
}

class World : private sf::NonCopyable {
  public:
    World(sf::RenderTarget& outputTarget, FontHolder& fonts, SoundPlayer& sounds, SettingsParser& settings);
    void update(sf::Time dt);
    void draw();

    CommandQueue& getCommandQueue();

    bool reachedEnd() const;
    bool finishedLevel() const;
    int getCurrentLevel() const;
    int getScore() const;
    void setLevel(int level);
    void setScore(int score);

    bool destroyed();

  private:
    void loadTextures();
    void adaptPlayerPosition();
    void handleCollisions(sf::Time dt);
    void updateSounds();

    void buildScene();
    sf::FloatRect getViewBounds() const;

    sf::RenderTarget& target;
    sf::RenderTexture sceneTexture;
    sf::View worldView;
    TextureHolder textures;
    FontHolder& fonts;
    SoundPlayer& sounds;
    SettingsParser& settings;

    BloomEffect bloomEffect;

    CommandQueue commandQueue;

    sf::FloatRect worldBounds;
    sf::Vector2f spawnPosition;

    Paddle* paddle;
    Score* score;
    Level* currentLevel;
    SceneNode sceneGraph;
    CollisionManager collisions;

    bool shakeScreen{false};
    sf::Time shakeTimer{sf::Time::Zero};
    bool shakeDirection{false};
    float shakeOffsetX{0};
    float shakeOffsetY{0};
};

#endif //SFML_TEMPLATE_WORLD_HPP
