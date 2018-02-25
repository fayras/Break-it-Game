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

namespace sf {
  class RenderTarget;
}

class World : private sf::NonCopyable {
  public:
    World(sf::RenderTarget& outputTarget, FontHolder& fonts, SoundPlayer& sounds);
    void update(sf::Time dt);
    void draw();

    CommandQueue& getCommandQueue();

    bool reachedEnd() const;
    int getScore() const;
    void resetPositions();

    bool destroyed();

  private:
    void loadTextures();
    void adaptPlayerPosition();
    void handleCollisions();
    void updateSounds();

    void buildScene();
    sf::FloatRect getViewBounds() const;

    sf::RenderTarget&	target;
    sf::RenderTexture sceneTexture;
    sf::View worldView;
    TextureHolder textures;
    FontHolder& fonts;
    SoundPlayer& sounds;

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
