#include "Statistics.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

Statistics::Statistics()
  : statisticsText()
{
  statisticsText.setPosition(10, 10);
}

void Statistics::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(statisticsText, states);
}

void Statistics::update(sf::Time dt) {
  time += dt;
  numFrames += 1;
  if(time >= sf::seconds(1.0f)) {
    statisticsText.setString(std::to_string(numFrames));
    time -= sf::seconds(1.0f);
    numFrames = 0;
  }
}

void Statistics::setFont(const sf::Font &font) {
  statisticsText.setFont(font);
}
