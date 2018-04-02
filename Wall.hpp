#ifndef BREAK_IT_WALL_HPP
#define BREAK_IT_WALL_HPP

#include <SFML/Graphics/RectangleShape.hpp>
#include "entities/Entity.hpp"

class Wall : public Entity {
  public:
    Wall(float width, float height);
    sf::FloatRect getBoundingRect() const override;
    void setDeadly(bool deadly);
    bool isDeadly() const;

  protected:
    void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;

  private:
    sf::RectangleShape rect;
    bool deadly{false};
};

#endif //BREAK_IT_WALL_HPP
