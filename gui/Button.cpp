#include "Button.hpp"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

gui::Button::Button(State::Context context)
  : callback(),
    normalTexture(context.textures->get(Textures::ID::BUTTON_NORMAL)),
    selectedTexture(context.textures->get(Textures::ID::BUTTON_SELECTED)),
    pressedTexture(context.textures->get(Textures::ID::BUTTON_PRESSED)),
    sprite(),
    text("", context.fonts->get(Fonts::ID::MAIN)),
    isToggle(false)
{
  sprite.setTexture(normalTexture);
  sf::FloatRect bounds = sprite.getLocalBounds();
  text.setPosition(bounds.width / 2.f, bounds.height / 2.f);
}

void gui::Button::setCallback(gui::Button::Callback callback) {
  this->callback = std::move(callback);
}

void gui::Button::setText(const std::string &text) {
  this->text.setString(text);
  sf::FloatRect bounds = this->text.getLocalBounds();
  this->text.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}

void gui::Button::setToggle(bool flag) {
  isToggle = flag;
}

bool gui::Button::selectable() const {
  return true;
}

void gui::Button::select() {
  Component::select();
  sprite.setTexture(selectedTexture);
}

void gui::Button::deselect() {
  Component::deselect();
  sprite.setTexture(normalTexture);
}

void gui::Button::activate() {
  Component::activate();

  // If we are toggle then we should show that the button is pressed and thus "toggled".
  if (isToggle) sprite.setTexture(pressedTexture);

  if (callback) callback();

  // If we are not a toggle then deactivate the button since we are just momentarily activated.
  if (!isToggle) deactivate();
}

void gui::Button::deactivate() {
  Component::deactivate();
  if (isToggle) {
    // Reset texture to right one depending on if we are selected or not.
    if (selected()) sprite.setTexture(selectedTexture);
    else sprite.setTexture(normalTexture);
  }
}

void gui::Button::handleEvent(const sf::Event &event) {

}

void gui::Button::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  states.transform *= getTransform();
  target.draw(sprite, states);
  target.draw(text, states);
}

sf::FloatRect gui::Button::getBounds() const {
  return getTransform().transformRect(sprite.getGlobalBounds());
}
