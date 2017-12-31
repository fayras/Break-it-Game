#ifndef BREAK_IT_LABEL_HPP
#define BREAK_IT_LABEL_HPP

#include <SFML/Graphics/Text.hpp>
#include "Component.hpp"
#include "../ResourceIdentifiers.hpp"

namespace gui {
  class Label : public Component {
    public:
      typedef std::shared_ptr<Label> Ptr;

      Label(const std::string& text, const FontHolder& fonts);

      bool selectable() const override;
      void setText(const std::string& text);
      void handleEvent(const sf::Event& event) override;

    protected:
      void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    private:
      sf::Text text;
  };
}


#endif //BREAK_IT_LABEL_HPP
