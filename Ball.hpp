#ifndef BREAK_IT_BALL_HPP
#define BREAK_IT_BALL_HPP

#include <SFML/Graphics/Sprite.hpp>
#include "entities/Entity.hpp"
#include "ResourceIdentifiers.hpp"

class Ball : public Entity {
  public:
    static constexpr float SPEED = 300.0f;

    explicit Ball(const TextureHolder& textures);
    unsigned int getCategory() const override;
    sf::FloatRect getBoundingRect() const override;

    void reset(sf::Vector2f pos);
    void wait(sf::Time wait);

  protected:
    void updateCurrent(sf::Time dt, CommandQueue &commands) override;
    void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;

  private:
    sf::Sprite sprite;
    sf::Time waitingTime{sf::Time::Zero};
};


#endif //BREAK_IT_BALL_HPP
