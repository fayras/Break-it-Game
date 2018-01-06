#include "Button.hpp"
#include "../tween/LinearTween.hpp"
#include "../system/Utility.hpp"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <cmath>

gui::Button::Button(State::Context context)
  : deco({ 3, 45 }),
    hover({ 0, 45 }),
    text("", context.fonts->get(Fonts::ID::PIXEL), 35),
    shortcutText("", context.fonts->get(Fonts::ID::NARROW), 30),
    isToggle(false),
    shortcut(sf::Keyboard::Unknown)
{
  deco.setFillColor(sf::Color(53, 92, 125, 200));
  hover.setFillColor({ 210, 230, 255, 50 });
  text.setFillColor(sf::Color::White);
  shortcutText.setFillColor({ 246, 114, 128 });
  shortcutText.move(2, 4);
  updatePositions();
}

void gui::Button::setCallback(gui::Button::Callback callback) {
  this->callback = std::move(callback);
}

void gui::Button::setText(const std::string &text) {
  this->text.setString(text);
  sf::FloatRect bounds = this->text.getLocalBounds();
  maxTextWidth = bounds.width + 30;
  // this->text.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top));
}

void gui::Button::setText(const std::wstring &text) {
  this->text.setString(text);
  sf::FloatRect bounds = this->text.getLocalBounds();
  maxTextWidth = bounds.width + 30;
}

void gui::Button::setToggle(bool flag) {
  isToggle = flag;
}

bool gui::Button::selectable() const {
  return true;
}

void gui::Button::select() {
  if(!selected()) {
    Component::select();
    tween(std::make_unique<LinearTween>(sf::milliseconds(100), [this](const float& t) {
      hover.setSize({ maxTextWidth * t, hover.getSize().y });
    }));
  }
}

void gui::Button::deselect() {
  if(selected()) {
    Component::deselect();
    tween(std::make_unique<LinearTween>(sf::milliseconds(100), [this](const float& t) {
      hover.setSize({ maxTextWidth - maxTextWidth * t, hover.getSize().y });
    }));
  }
}

void gui::Button::activate() {
  Component::activate();

  // If we are toggle then we should show that the button is pressed and thus "toggled".
  if (isToggle) {
    hover.setFillColor(sf::Color::White);
    text.setFillColor(sf::Color::Black);
  }

  if (callback) callback();

  // If we are not a toggle then deactivate the button since we are just momentarily activated.
  if (!isToggle) deactivate();
}

void gui::Button::deactivate() {
  Component::deactivate();
  if (isToggle) {
    // Reset texture to right one depending on if we are selected or not.
    hover.setFillColor({ 255, 255, 255, 70 });
    text.setFillColor(sf::Color::White);
    if (selected()) hover.setSize({ maxTextWidth, hover.getSize().y });
    else hover.setSize({ 0, hover.getSize().y });
  }
}

void gui::Button::handleEvent(const sf::Event &event) {
  if(event.type == sf::Event::MouseMoved) {
    if(getBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
      select();
    } else {
      deselect();
    }
  }

  if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
    if(getBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
      activate();
    }
  }

  if(selected() && event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Return) {
    activate();
  }

  if(event.type == sf::Event::KeyReleased && event.key.code != sf::Keyboard::Unknown && event.key.code == shortcut) {
    activate();
  }
}

void gui::Button::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  states.transform *= getTransform();
  target.draw(deco, states);
  target.draw(hover, states);
  target.draw(text, states);
  if(shortcut != sf::Keyboard::Unknown) {
    target.draw(shortcutText, states);
  }
}

sf::FloatRect gui::Button::getBounds() const {
  sf::Transform transform = parent ? getTransform() * parent->getTransform() : getTransform();
  return transform.transformRect(text.getGlobalBounds());
}

void gui::Button::update(sf::Time dt) {
  Tweenable::update(dt);
}

void gui::Button::setShortcut(sf::Keyboard::Key key) {
  shortcut = key;
  if(key != sf::Keyboard::Unknown) {
    shortcutText.setString(String::from(key));
  } else {
    shortcutText.setString("");
  }
  updatePositions();
}

void gui::Button::updatePositions() {
  auto bounds = shortcutText.getLocalBounds();
  deco.setSize({ bounds.width + 7, deco.getSize().y });
  text.setPosition(bounds.width + 20, -4);
  hover.setPosition(bounds.width + 7, hover.getPosition().y);
}
