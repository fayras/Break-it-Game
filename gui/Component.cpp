#include "Component.hpp"

gui::Component::Component()
  : isSelected(false), isActive(false)
{}

gui::Component::~Component() {}

bool gui::Component::selected() const {
  return isSelected;
}

void gui::Component::select() {
  isSelected = true;
}

void gui::Component::deselect() {
  isSelected = false;
}

bool gui::Component::active() const {
  return isActive;
}

void gui::Component::activate() {
  isActive = true;
}

void gui::Component::deactivate() {
  isActive = false;
}

sf::FloatRect gui::Component::getBounds() const {
  return sf::FloatRect();
}
