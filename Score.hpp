#ifndef BREAK_IT_SCORE_HPP
#define BREAK_IT_SCORE_HPP

#include <SFML/Graphics/Text.hpp>
#include "nodes/SpriteNode.hpp"

class Score : public SpriteNode {
  public:
    Score(const sf::Texture &texture, const sf::Font& font);
    void increaseMultiplier();
    void resetMultiplier();
    void increase(int amount = 10);
    int get() const;

  protected:
    void updateCurrent(sf::Time dt, CommandQueue &commands) override;
    void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;

  private:
    int score{0};
    float multiplier{1.0f};
    sf::Text scoreText;
};

#endif //BREAK_IT_SCORE_HPP
