#include "Label.hpp"
#include "../system/ResourceHolder.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

gui::Label::Label(const std::string &text, const FontHolder &fonts)
  : text(text, fonts.get(Fonts::PIXEL), 35)
{}

gui::Label::Label(const std::string &text, const sf::Font & font)
  : text(text, font, 35)
{}

gui::Label::Label(const std::wstring &text, const sf::Font &font)
  : text(text, font, 35)
{}

bool gui::Label::selectable() const {
  return false;
}

void gui::Label::setText(const std::string &text) {
  this->text.setString(text);
}

void gui::Label::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  states.transform *= getTransform();
  target.draw(text, states);
}

void gui::Label::handleEvent(const sf::Event &event) {

}

void gui::Label::setSize(unsigned int size) {
  text.setCharacterSize(size);
}
