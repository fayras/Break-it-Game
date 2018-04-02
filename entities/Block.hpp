#ifndef BREAK_IT_BLOCK_HPP
#define BREAK_IT_BLOCK_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <unordered_map>
#include "Entity.hpp"
#include "../ResourceIdentifiers.hpp"
#include "../system/Animation.hpp"
#include "BlockTypes.hpp"

class Block : public Entity {
  public:
    typedef std::unique_ptr<Block> Ptr;

    explicit Block(const TextureHolder& textures, Blocks::Type type);
    void setColor(const sf::Color& color);
    sf::FloatRect getBoundingRect() const override;
    bool isMarkedForRemoval() const override;
    void damage(int points) override;

  protected:
    void updateCurrent(sf::Time dt, CommandQueue &commands) override;
    void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;

  private:
    sf::Sprite sprite;
    Animation breakAnimation;
    Blocks::Type type;
    const TextureHolder& textures;
    std::unordered_map<Blocks::Type, sf::Clock> timers;
    bool hidden{false};
};


#endif //BREAK_IT_BLOCK_HPP
