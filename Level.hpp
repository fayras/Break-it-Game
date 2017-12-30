#ifndef BREAK_IT_LEVEL_HPP
#define BREAK_IT_LEVEL_HPP

#include "nodes/SceneNode.hpp"
#include "Block.hpp"
#include "DataTables.hpp"

class Level : public SceneNode {
  public:
    explicit Level(const TextureHolder& textures, const FontHolder& fonts);
    void loadNext();
    void setBounds(sf::FloatRect* bounds);

    bool done() const;
    bool isLast() const;
    int getID() const;
    float getBallSpeedMultiplier() const;

  private:
    void load();

  protected:
    void updateCurrent(sf::Time dt, CommandQueue &commands) override;

  private:
    int currentID{-1};
    LevelData levelData;
    sf::FloatRect* bounds;
    const TextureHolder& textures;
    SceneNode* blocksLayer;
};

#endif //BREAK_IT_LEVEL_HPP
