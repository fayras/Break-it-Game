#ifndef SFML_TEMPLATE_ENTITY_HPP
#define SFML_TEMPLATE_ENTITY_HPP

#include "../nodes/SceneNode.hpp"

class Entity : public SceneNode {
  public:
    explicit Entity(int hitpoints);

    void setVelocity(sf::Vector2f velocity);
    void setVelocity(float vx, float vy);
    void accelerate(sf::Vector2f velocity);
    void accelerate(float vx, float vy);
    sf::Vector2f getVelocity() const;

    int getHitpoints() const;
    void heal(int points);
    void damage(int points);
    void destroy();

    virtual void remove();
    bool isDestroyed() const override;

  protected:
    void updateCurrent(sf::Time dt, CommandQueue& commands) override;

  private:
    sf::Vector2f velocity;
    int hitpoints;
};


#endif //SFML_TEMPLATE_ENTITY_HPP
