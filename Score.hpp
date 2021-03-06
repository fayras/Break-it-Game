#ifndef BREAK_IT_SCORE_HPP
#define BREAK_IT_SCORE_HPP

#include <SFML/Graphics/Text.hpp>
#include "nodes/SpriteNode.hpp"

class Score : public SceneNode {
  public:
    explicit Score(const sf::Font& font);
    void increaseMultiplier();
    void resetMultiplier();
    void increase(int amount = 10);
    void set(int amount);
    int get() const;

  protected:
    void updateCurrent(sf::Time dt, CommandQueue &commands) override;
    void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;

  private:
    int score{0};
    int currentVisibleScore{0};
    float multiplier{1.0f};
    sf::Text scoreText;
};

#endif //BREAK_IT_SCORE_HPP
