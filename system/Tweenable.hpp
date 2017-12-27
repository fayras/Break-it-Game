#ifndef BREAK_IT_TWEENABLE_HPP
#define BREAK_IT_TWEENABLE_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics/Transformable.hpp>

class Tweenable : public virtual sf::Transformable {
  public:
    enum class Type {
        LINEAR, ELASTIC_IN, ELASTIC_OUT
    };

    void tweenRotationTo(float angle);
    void tweenScaleTo(float x, float y);
    void tweenPositionTo(sf::Vector2f to);
    void setTweenType(Tweenable::Type type);
    void setTime(sf::Time time);
    void tweenDelay(sf::Time delay);

    void update(sf::Time dt);

  private:
    sf::Time time;
    sf::Vector2f positionFrom;
    sf::Vector2f positionDiff;
    sf::Vector2f scaleFrom;
    sf::Vector2f scaleDiff;
    float angleFrom;
    float angleDiff;
    Tweenable::Type currentType{Type::LINEAR};
};

#endif //BREAK_IT_TWEENABLE_HPP
