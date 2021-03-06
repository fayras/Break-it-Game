#include "TextInput.hpp"
#include "../system/ResourceHolder.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

gui::TextInput::TextInput(const FontHolder &fonts)
  : hasFocus(false), inputString(),
    text(inputString, fonts.get(Fonts::PIXEL), 35),
    placeholder("", fonts.get(Fonts::PIXEL), 35),
    background(), caret(sf::Vector2f(1, 20))
{
  background.setFillColor(sf::Color(0, 0, 0, 150));
  background.setOutlineColor(sf::Color(255, 255, 255, 150));
  background.setOutlineThickness(1.0f);
  caret.setFillColor(sf::Color::White);
  text.setFillColor(sf::Color::White);
  placeholder.setFillColor(sf::Color(255, 255, 255, 210));

  sf::FloatRect bounds = background.getLocalBounds();
  text.setPosition(bounds.left + 10, bounds.top - 5);
  placeholder.setPosition(bounds.left + 10, bounds.top - 5);
  caret.setPosition(bounds.left + 10, bounds.top + 6);
}

bool gui::TextInput::selectable() const {
  return true;
}

void gui::TextInput::select() {
  Component::select();
  background.setOutlineColor(sf::Color::Blue);
}

void gui::TextInput::deselect() {
  Component::deselect();
  background.setOutlineColor(sf::Color(255, 255, 255, 100));
}

void gui::TextInput::activate() {
  if(callback) {
    callback(inputString);
    inputString = "";
    text.setString(inputString);
  }
}

sf::FloatRect gui::TextInput::getBounds() const {
  sf::Transform transform = parent ? getTransform() * parent->getTransform() : getTransform();
  return transform.transformRect(background.getGlobalBounds());;
}

void gui::TextInput::setCallback(gui::TextInput::Callback callback) {
  this->callback = std::move(callback);
}

void gui::TextInput::setSize(float width, float height) {
  background.setSize(sf::Vector2f(width, height));
}

void gui::TextInput::setPlaceholder(std::string placeholder) {
  this->placeholder.setString(placeholder);
}

void gui::TextInput::handleEvent(const sf::Event &event) {
  if(event.type == sf::Event::MouseButtonPressed) {
    if(event.mouseButton.button == sf::Mouse::Left) {
      if(getBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
        select();
      } else {
        deselect();
      }
    }
  }

  if(selected()) {
    if(event.type == sf::Event::TextEntered) {
      if(event.text.unicode == '\b') {
        if(!inputString.empty()) {
          inputString.pop_back();
        }
      } else if(event.text.unicode > 31 && event.text.unicode < 128) {
        inputString.push_back(static_cast<char>(event.text.unicode));
      }
      text.setString(inputString);
      auto textBounds = text.getGlobalBounds();
      caret.setPosition(textBounds.left + textBounds.width + 2, caret.getPosition().y);
    }
    if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Return) {
      activate();
    }
  }
}

void gui::TextInput::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  states.transform *= getTransform();
  target.draw(background, states);
  if(inputString.empty()) {
    target.draw(placeholder, states);
  } else {
    target.draw(text, states);
  }
  if(selected()) target.draw(caret, states);
}
