#include "Container.hpp"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

gui::Container::Container()
  : children(), selectedChild(-1)
{}

void gui::Container::pack(gui::Component::Ptr component) {
  component->setParent(this);
  children.push_back(component);

  if (!hasSelection() && component->selectable())
    select(children.size() - 1);
}

bool gui::Container::selectable() const {
  return false;
}

void gui::Container::handleEvent(const sf::Event &event) {
  if (event.type == sf::Event::KeyReleased) {
    if (event.key.code == sf::Keyboard::Up) {
      selectPrevious();
    } else if (event.key.code == sf::Keyboard::Down) {
      selectNext();
    }
  }
  for(auto& child : children) {
    child->handleEvent(event);
  }
}

void gui::Container::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  states.transform *= getTransform();
  for(const Component::Ptr& child : children) {
    target.draw(*child, states);
  }
}

bool gui::Container::hasSelection() const {
  return selectedChild >= 0;
}

void gui::Container::select(std::size_t index) {
  if (children[index]->selectable())
  {
    if (hasSelection())
      children[selectedChild]->deselect();

    children[index]->select();
    selectedChild = index;
  }
}

void gui::Container::selectNext() {
  if (!hasSelection())
    return;

  // Search next component that is selectable, wrap around if necessary
  int next = selectedChild;
  do
    next = (next + 1) % children.size();
  while (!children[next]->selectable());

  // Select that component
  select(next);
}

void gui::Container::selectPrevious() {
  if (!hasSelection())
    return;

  // Search previous component that is selectable, wrap around if necessary
  int prev = selectedChild;
  do
    prev = (prev + children.size() - 1) % children.size();
  while (!children[prev]->selectable());

  // Select that component
  select(prev);
}

sf::FloatRect gui::Container::getBounds() const {
  // TODO: berechne Bounds anhand der Rects der Kinder.
  return sf::FloatRect();
}
