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
#include "Ball.hpp"
#include "Paddle.hpp"
#include "Block.hpp"
#include "Wall.hpp"
#include "nodes/SpriteNode.hpp"
#include "Score.hpp"
#include "nodes/ParticleNode.hpp"
#include "Level.hpp"
#include "Life.hpp"

namespace sf {
  class RenderTarget;
}

class World : private sf::NonCopyable {
  public:
    World(sf::RenderTarget& outputTarget, FontHolder& fonts, SoundPlayer& sounds);
    void update(sf::Time dt);
    void draw();

    CommandQueue& getCommandQueue();

    bool ballInsideBounds() const;
    bool reachedEnd() const;
    int getScore() const;
    int getLevel() const;
    void resetPositions();

    bool showNewLevelMessage{false};

    bool destroyed();

  private:
    void loadTextures();
    void adaptPlayerPosition();
    void handleCollisions();
    bool collision(sf::FloatRect rectA, sf::FloatRect rectB, sf::Vector2f& ballVelocity) const;
    void updateSounds();

    void buildScene();
    void loadNextLevel();
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

    std::unique_ptr<SpriteNode> background;
    std::unique_ptr<Ball> ball;
    std::unique_ptr<Paddle> paddle;
    std::vector<std::unique_ptr<Wall>> walls;
    std::unique_ptr<Score> score;
    std::unique_ptr<Life> lives;
    std::unique_ptr<ParticleNode> particles;
    std::unique_ptr<Level> currentLevel;

    bool shakeScreen{false};
    sf::Time shakeTimer{sf::Time::Zero};
    bool shakeDirection{false};
    float shakeOffsetX{0};
    float shakeOffsetY{0};
};

#endif //SFML_TEMPLATE_WORLD_HPP
