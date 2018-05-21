#include "Paddle.hpp"
#include "../system/ResourceHolder.hpp"
#include "../system/Utility.hpp"
#include "../Life.hpp"
#include "../tween/LinearTween.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

const int SPEED = 500;

Paddle::Paddle(const TextureHolder& textures)
  : Entity(3),
    sprite(textures.get(Textures::PADDLE))
{
  centerOrigin(sprite);
  isStatic = false;
  defaultCategory = Category::PADDLE;

  auto lives = std::make_unique<Life>(textures.get(Textures::LIFE));
  lives->setPosition(10, 10);
  attachChild(std::move(lives));
}

void Paddle::updateCurrent(sf::Time dt, CommandQueue &commands) {
  Entity::updateCurrent(dt, commands);
}

void Paddle::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(sprite, states);
}

int Paddle::getMovementSpeed() const {
  return SPEED;
}

sf::FloatRect Paddle::getBoundingRect() const {
  return getWorldTransform().transformRect(sprite.getGlobalBounds());
}

bool Paddle::isMarkedForRemoval() const {
  return false;
}

Entity::CollisionResponse Paddle::getCollisionResponse() const {
    return CollisionResponse::STOP;
}

void Paddle::bounce(float height) {
    if(bouncing) {
        return;
    }

    float oldY = getPosition().y;
    auto t = std::make_unique<LinearTween>(sf::milliseconds(100), [this, oldY, height](const float& t) {
        float y = oldY - height * (0.5f - std::abs(0.5f - t));
        setPosition(getPosition().x, y);
    });
    t->attachObserver([this]() {
        bouncing = false;
    });
    tween(std::move(t));
    bouncing = true;
}
