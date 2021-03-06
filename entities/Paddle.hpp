#ifndef BREAK_IT_PADDLE_HPP
#define BREAK_IT_PADDLE_HPP

#include <SFML/Graphics/Sprite.hpp>
#include "Entity.hpp"
#include "../ResourceIdentifiers.hpp"

class Paddle : public Entity {
  public:
    explicit Paddle(const TextureHolder& textures);
    int getMovementSpeed() const;
    sf::FloatRect getBoundingRect() const override;
    bool isMarkedForRemoval() const override;
    CollisionResponse getCollisionResponse() const override;

    void bounce(float height);

  protected:
    void updateCurrent(sf::Time dt, CommandQueue &commands) override;
    void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;

  private:
    sf::Sprite sprite;
    bool bouncing{false};
};

#endif //BREAK_IT_PADDLE_HPP
