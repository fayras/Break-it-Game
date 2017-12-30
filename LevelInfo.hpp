#ifndef BREAK_IT_LEVELINFO_HPP
#define BREAK_IT_LEVELINFO_HPP

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
#include "nodes/SceneNode.hpp"

class LevelInfo : public SceneNode {
  public:
    explicit LevelInfo(const sf::Font& font);
    void setView(sf::Vector2f view);
    void show(int level, sf::Time delay = sf::Time::Zero);
    unsigned int getCategory() const override;

  protected:
    void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;

  private:
    sf::RectangleShape backgroundShape;
    sf::Text pausedText;
    sf::Clock timer;
    sf::Vector2f view;
};

#endif //BREAK_IT_LEVELINFO_HPP
