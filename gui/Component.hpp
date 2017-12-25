#ifndef SFML_TEMPLATE_COMPONENT_HPP
#define SFML_TEMPLATE_COMPONENT_HPP

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <memory>

namespace sf {
  class Event;
}

namespace gui {
  class Component : public sf::Drawable, public sf::Transformable, private sf::NonCopyable {
    public:
      typedef std::shared_ptr<Component> Ptr;

      Component();
      ~Component() override;

      bool selected() const;
      virtual bool selectable() const = 0;
      virtual void select();
      virtual void deselect();

      bool active() const;
      virtual void activate();
      virtual void deactivate();
      virtual sf::FloatRect getBounds() const;

      virtual void handleEvent(const sf::Event& event) = 0;

    private:
      bool isSelected;
      bool isActive;

  };
}



#endif //SFML_TEMPLATE_COMPONENT_HPP
