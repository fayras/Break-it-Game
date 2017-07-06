#ifndef SFML_TEMPLATE_CONTAINER_HPP
#define SFML_TEMPLATE_CONTAINER_HPP

#include "Component.hpp"
#include <vector>
#include <memory>

namespace gui {
  class Container : public Component {
    public:
      typedef std::shared_ptr<Container> Ptr;

      Container();
      void pack(Component::Ptr component);
      virtual bool selectable() const;
      virtual void handleEvent(const sf::Event& event);
      sf::FloatRect getBounds() const override;

    private:
      virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

      bool hasSelection() const;
      void select(std::size_t index);
      void selectNext();
      void selectPrevious();

      std::vector<Component::Ptr> children;
      int selectedChild;
  };
}

#endif //SFML_TEMPLATE_CONTAINER_HPP
