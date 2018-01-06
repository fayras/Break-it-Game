#ifndef BREAK_IT_STATISTICS_HPP
#define BREAK_IT_STATISTICS_HPP

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Text.hpp>

class Statistics : public sf::Drawable {
  public:
    explicit Statistics();

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    void update(sf::Time dt);
    void setFont(const sf::Font& font);

  private:
    sf::Time time;
    int numFrames;
    sf::Text statisticsText;
};

#endif //BREAK_IT_STATISTICS_HPP
