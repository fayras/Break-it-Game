#ifndef BREAK_IT_BALL_HPP
#define BREAK_IT_BALL_HPP

#include <SFML/Graphics/Sprite.hpp>
#include "Entity.hpp"
#include "../ResourceIdentifiers.hpp"

class Ball : public Entity {
  public:
    static constexpr float SPEED = 300.0f;

    explicit Ball(const TextureHolder& textures);
    unsigned int getCategory() const override;
    sf::FloatRect getBoundingRect() const override;

    void duplicate();
    void rotateBy(float angle);
    void reset(sf::Vector2f pos);

  protected:
    void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;

  private:
    sf::Sprite sprite;
    const TextureHolder& textures;
};


#endif //BREAK_IT_BALL_HPP
