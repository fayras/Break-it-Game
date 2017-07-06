#include "Utility.hpp"
#include "Animation.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <random>
#include <cmath>
#include <ctime>
#include <cassert>

namespace {
  std::default_random_engine createRandomEngine() {
    auto seed = static_cast<unsigned long>(std::time(nullptr));
    return std::default_random_engine(seed);
  }

  auto RandomEngine = createRandomEngine();
}

void centerOrigin(sf::Sprite& sprite) {
  sf::FloatRect bounds = sprite.getLocalBounds();
  sprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}

void centerOrigin(sf::Text& text) {
  sf::FloatRect bounds = text.getLocalBounds();
  text.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}

void centerOrigin(Animation& animation) {
  sf::FloatRect bounds = animation.getLocalBounds();
  animation.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}

float toDegree(float radian) {
  return 180.f / 3.141592653589793238462643383f * radian;
}

float toRadian(float degree) {
  return 3.141592653589793238462643383f / 180.f * degree;
}

int Random::integer(int min, int max) {
  std::uniform_int_distribution<> distr(min, max);
  return distr(RandomEngine);
}

int Random::integer(int max) {
  return Random::integer(0, max);
}

float Random::decimal(float min, float max) {
  std::uniform_real_distribution<float> distr(min, max);
  return distr(RandomEngine);
}

float Vector::length(sf::Vector2f vector) {
  return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

sf::Vector2f Vector::unit(sf::Vector2f vector) {
  assert(vector != sf::Vector2f(0.f, 0.f));
  return vector / length(vector);
}
