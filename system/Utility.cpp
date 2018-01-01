#include "Utility.hpp"
#include "Animation.hpp"

#include <SFML/Graphics/Text.hpp>
#include <random>
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

std::string String::from(sf::Keyboard::Key key) {
#define SFML_KEY_TO_STRING(KEY) case sf::Keyboard::KEY: return #KEY;

  switch (key) {
    SFML_KEY_TO_STRING(Unknown)
    SFML_KEY_TO_STRING(A)
    SFML_KEY_TO_STRING(B)
    SFML_KEY_TO_STRING(C)
    SFML_KEY_TO_STRING(D)
    SFML_KEY_TO_STRING(E)
    SFML_KEY_TO_STRING(F)
    SFML_KEY_TO_STRING(G)
    SFML_KEY_TO_STRING(H)
    SFML_KEY_TO_STRING(I)
    SFML_KEY_TO_STRING(J)
    SFML_KEY_TO_STRING(K)
    SFML_KEY_TO_STRING(L)
    SFML_KEY_TO_STRING(M)
    SFML_KEY_TO_STRING(N)
    SFML_KEY_TO_STRING(O)
    SFML_KEY_TO_STRING(P)
    SFML_KEY_TO_STRING(Q)
    SFML_KEY_TO_STRING(R)
    SFML_KEY_TO_STRING(S)
    SFML_KEY_TO_STRING(T)
    SFML_KEY_TO_STRING(U)
    SFML_KEY_TO_STRING(V)
    SFML_KEY_TO_STRING(W)
    SFML_KEY_TO_STRING(X)
    SFML_KEY_TO_STRING(Y)
    SFML_KEY_TO_STRING(Z)
    SFML_KEY_TO_STRING(Num0)
    SFML_KEY_TO_STRING(Num1)
    SFML_KEY_TO_STRING(Num2)
    SFML_KEY_TO_STRING(Num3)
    SFML_KEY_TO_STRING(Num4)
    SFML_KEY_TO_STRING(Num5)
    SFML_KEY_TO_STRING(Num6)
    SFML_KEY_TO_STRING(Num7)
    SFML_KEY_TO_STRING(Num8)
    SFML_KEY_TO_STRING(Num9)
    SFML_KEY_TO_STRING(Escape)
    SFML_KEY_TO_STRING(LControl)
    SFML_KEY_TO_STRING(LShift)
    SFML_KEY_TO_STRING(LAlt)
    SFML_KEY_TO_STRING(LSystem)
    SFML_KEY_TO_STRING(RControl)
    SFML_KEY_TO_STRING(RShift)
    SFML_KEY_TO_STRING(RAlt)
    SFML_KEY_TO_STRING(RSystem)
    SFML_KEY_TO_STRING(Menu)
    SFML_KEY_TO_STRING(LBracket)
    SFML_KEY_TO_STRING(RBracket)
    SFML_KEY_TO_STRING(SemiColon)
    SFML_KEY_TO_STRING(Comma)
    SFML_KEY_TO_STRING(Period)
    SFML_KEY_TO_STRING(Quote)
    SFML_KEY_TO_STRING(Slash)
    SFML_KEY_TO_STRING(BackSlash)
    SFML_KEY_TO_STRING(Tilde)
    SFML_KEY_TO_STRING(Equal)
    SFML_KEY_TO_STRING(Dash)
    SFML_KEY_TO_STRING(Space)
    SFML_KEY_TO_STRING(Return)
    SFML_KEY_TO_STRING(BackSpace)
    SFML_KEY_TO_STRING(Tab)
    SFML_KEY_TO_STRING(PageUp)
    SFML_KEY_TO_STRING(PageDown)
    SFML_KEY_TO_STRING(End)
    SFML_KEY_TO_STRING(Home)
    SFML_KEY_TO_STRING(Insert)
    SFML_KEY_TO_STRING(Delete)
    SFML_KEY_TO_STRING(Add)
    SFML_KEY_TO_STRING(Subtract)
    SFML_KEY_TO_STRING(Multiply)
    SFML_KEY_TO_STRING(Divide)
    SFML_KEY_TO_STRING(Left)
    SFML_KEY_TO_STRING(Right)
    SFML_KEY_TO_STRING(Up)
    SFML_KEY_TO_STRING(Down)
    SFML_KEY_TO_STRING(Numpad0)
    SFML_KEY_TO_STRING(Numpad1)
    SFML_KEY_TO_STRING(Numpad2)
    SFML_KEY_TO_STRING(Numpad3)
    SFML_KEY_TO_STRING(Numpad4)
    SFML_KEY_TO_STRING(Numpad5)
    SFML_KEY_TO_STRING(Numpad6)
    SFML_KEY_TO_STRING(Numpad7)
    SFML_KEY_TO_STRING(Numpad8)
    SFML_KEY_TO_STRING(Numpad9)
    SFML_KEY_TO_STRING(F1)
    SFML_KEY_TO_STRING(F2)
    SFML_KEY_TO_STRING(F3)
    SFML_KEY_TO_STRING(F4)
    SFML_KEY_TO_STRING(F5)
    SFML_KEY_TO_STRING(F6)
    SFML_KEY_TO_STRING(F7)
    SFML_KEY_TO_STRING(F8)
    SFML_KEY_TO_STRING(F9)
    SFML_KEY_TO_STRING(F10)
    SFML_KEY_TO_STRING(F11)
    SFML_KEY_TO_STRING(F12)
    SFML_KEY_TO_STRING(F13)
    SFML_KEY_TO_STRING(F14)
    SFML_KEY_TO_STRING(F15)
    SFML_KEY_TO_STRING(Pause)
  }

  return "";
}
