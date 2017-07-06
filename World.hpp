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

namespace sf {
  class RenderTarget;
}

class World : private sf::NonCopyable {
  public:
    World(sf::RenderTarget& outputTarget, FontHolder& fonts, SoundPlayer& sounds);
    void update(sf::Time dt);
    void draw();

    CommandQueue& getCommandQueue();

    bool hasAlivePlayer() const;
    bool hasPlayerReachedEnd() const;

  private:
    enum Layer {
      Background,
      LowerAir,
      UpperAir,
      LayerCount
    };

    void loadTextures();
    void adaptPlayerPosition();
    void adaptPlayerVelocity();
    void handleCollisions();
    void updateSounds();

    void buildScene();
    sf::FloatRect getViewBounds() const;
    sf::FloatRect getBattlefieldBounds() const;

    sf::RenderTarget&	target;
    sf::RenderTexture sceneTexture;
    sf::View worldView;
    TextureHolder textures;
    FontHolder& fonts;
    SoundPlayer& sounds;

    SceneNode sceneGraph;
    std::array<SceneNode*, LayerCount> sceneLayers;
    CommandQueue commandQueue;

    sf::FloatRect worldBounds;
    sf::Vector2f spawnPosition;
};

#endif //SFML_TEMPLATE_WORLD_HPP
