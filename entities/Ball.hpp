#ifndef BREAK_IT_BALL_HPP
#define BREAK_IT_BALL_HPP

#include <SFML/Graphics/Sprite.hpp>
#include "Entity.hpp"
#include "../ResourceIdentifiers.hpp"

class EmitterNode;

class Ball : public Entity {
  public:
    static constexpr float SPEED = 300.0f;

    explicit Ball(const sf::Texture& texture);
    sf::FloatRect getBoundingRect() const override;

    void duplicate();
    void rotateBy(float angle);
    void reset(sf::Vector2f pos);

  protected:
    void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;
    void updateCurrent(sf::Time dt, CommandQueue &commands) override;
    void updateChildren(sf::Time dt, CommandQueue &commands) override;

private:
    sf::Sprite sprite;
    EmitterNode* trail;
};


#endif //BREAK_IT_BALL_HPP
