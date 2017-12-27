#ifndef BREAK_IT_BLOCK_HPP
#define BREAK_IT_BLOCK_HPP

#include <SFML/Graphics/Sprite.hpp>
#include "entities/Entity.hpp"
#include "ResourceIdentifiers.hpp"
#include "system/Animation.hpp"
#include "system/Tweenable.hpp"

class Block : public Entity, public virtual Tweenable {
  public:
    typedef std::unique_ptr<Block> Ptr;

    explicit Block(const TextureHolder& textures);
    Block(const TextureHolder& textures, sf::Color color);
    unsigned int getCategory() const override;
    sf::FloatRect getBoundingRect() const override;
    bool isMarkedForRemoval() const override;

  protected:
    void updateCurrent(sf::Time dt, CommandQueue &commands) override;
    void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;

  private:
    sf::Sprite sprite;
    Animation breakAnimation;
};


#endif //BREAK_IT_BLOCK_HPP
