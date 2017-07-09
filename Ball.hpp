#ifndef BREAK_IT_BALL_HPP
#define BREAK_IT_BALL_HPP

#include <SFML/Graphics/Sprite.hpp>
#include "entities/Entity.hpp"
#include "ResourceIdentifiers.hpp"

class Ball : public Entity {
  public:
    Ball(const TextureHolder& textures);
    unsigned int getCategory() const override;
    sf::FloatRect getBoundingRect() const override;

  protected:
    void updateCurrent(sf::Time dt, CommandQueue &commands) override;
    void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;

  private:
    sf::Sprite sprite;
};


#endif //BREAK_IT_BALL_HPP
