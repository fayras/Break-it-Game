#ifndef BREAK_IT_BLOCK_HPP
#define BREAK_IT_BLOCK_HPP

#include <SFML/Graphics/Sprite.hpp>
#include "entities/Entity.hpp"
#include "ResourceIdentifiers.hpp"

class Block : public Entity {
  public:
    Block(const TextureHolder& textures);
    unsigned int getCategory() const override;

  protected:
  public:
    sf::FloatRect getBoundingRect() const override;

  protected:
    void updateCurrent(sf::Time dt, CommandQueue &commands) override;
    void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;

  private:
    sf::Sprite sprite;
};


#endif //BREAK_IT_BLOCK_HPP
