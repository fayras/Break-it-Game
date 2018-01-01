#ifndef SFML_TEMPLATE_UTILITY_HPP
#define SFML_TEMPLATE_UTILITY_HPP

#include <string>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>

class Animation;
namespace sf {
  class Sprite;
  class Text;
}

// Call setOrigin() with the center of the object
void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);
void centerOrigin(Animation& animation);

// Degree/radian conversion
float toDegree(float radian);
float toRadian(float degree);

namespace Random {
  int integer(int min, int max);
  int integer(int max);
  float decimal(float min, float max);
}

namespace Vector {
  float	length(sf::Vector2f vector);
  sf::Vector2f unit(sf::Vector2f vector);
}

namespace String {
  std::string from(sf::Keyboard::Key key);
}

#endif //SFML_TEMPLATE_UTILITY_HPP
