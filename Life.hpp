#ifndef BREAK_IT_LIFE_HPP
#define BREAK_IT_LIFE_HPP

#include "nodes/SpriteNode.hpp"

class Life : public SpriteNode {
  public:
    explicit Life(const sf::Texture &texture);
    int getLives() const;

  protected:
    void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;

  public:
    void decrease();
    unsigned int getCategory() const override;

  private:
    int lives{3};
    sf::IntRect originalRect;
};

#endif //BREAK_IT_LIFE_HPP
