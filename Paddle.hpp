#ifndef BREAK_IT_PADDLE_HPP
#define BREAK_IT_PADDLE_HPP

#include <SFML/Graphics/Sprite.hpp>
#include "entities/Entity.hpp"
#include "ResourceIdentifiers.hpp"

class Paddle : public Entity {
  public:
    Paddle(const TextureHolder& textures);
    unsigned int getCategory() const override;

  protected:
    void updateCurrent(sf::Time dt, CommandQueue &commands) override;
    void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;

  private:
    sf::Sprite sprite;
};

#endif //BREAK_IT_PADDLE_HPP
