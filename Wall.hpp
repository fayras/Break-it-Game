#ifndef BREAK_IT_WALL_HPP
#define BREAK_IT_WALL_HPP

#include <SFML/Graphics/RectangleShape.hpp>
#include "nodes/SceneNode.hpp"

class Wall : public SceneNode {
  public:
    Wall(float width, float height);
    unsigned int getCategory() const override;
    sf::FloatRect getBoundingRect() const override;
    void highlight(bool flag);

  protected:
    void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;

  private:
    sf::RectangleShape rect;
};

#endif //BREAK_IT_WALL_HPP
