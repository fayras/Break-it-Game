#ifndef BREAK_IT_LEVEL_HPP
#define BREAK_IT_LEVEL_HPP

#include "nodes/SceneNode.hpp"
#include "entities/Block.hpp"
#include "DataTables.hpp"

class Level : public SceneNode {
  public:
    explicit Level(const TextureHolder& textures, const FontHolder& fonts, sf::Vector2f view);
    void loadNext();
    void load(int level);
    void setBounds(sf::FloatRect* bounds);

    bool done() const;
    bool ballLost() const;
    bool isLast() const;
    bool isLoading() const;
    int getID() const;
    float getBallSpeedMultiplier() const;

  protected:
    void updateCurrent(sf::Time dt, CommandQueue &commands) override;

  private:
    void load();

    bool loading{false};
    bool objectsNeedUpdate{false};
    int currentID{-1};
    LevelData levelData;
    sf::FloatRect* bounds;
    const TextureHolder& textures;
    const FontHolder& fonts;
    SceneNode* blocksLayer;
    SceneNode* ballsLayer;

    void resetObjects(CommandQueue &commands);
};

#endif //BREAK_IT_LEVEL_HPP
