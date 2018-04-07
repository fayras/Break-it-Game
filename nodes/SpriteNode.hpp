#ifndef SFML_TEMPLATE_SPRITENODE_HPP
#define SFML_TEMPLATE_SPRITENODE_HPP

#include <SFML/Graphics/Sprite.hpp>

#include "SceneNode.hpp"

class SpriteNode : public SceneNode {
  public:
    explicit SpriteNode(const sf::Texture& texture);
    SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect);
    void setColor(sf::Color color);
    sf::FloatRect getBoundingRect() const;

  protected:
    void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Sprite sprite;
};

#endif //SFML_TEMPLATE_SPRITENODE_HPP
