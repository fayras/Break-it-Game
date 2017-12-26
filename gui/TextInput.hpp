#ifndef BREAK_IT_TEXTINPUT_HPP
#define BREAK_IT_TEXTINPUT_HPP

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "Component.hpp"
#include "../ResourceIdentifiers.hpp"

namespace gui {
  class TextInput : public Component {
    public:
      typedef std::function<void(std::string)> Callback;

      explicit TextInput(const FontHolder& fonts);

      bool selectable() const override;
      void select() override;
      void deselect() override;
      void activate() override;
      sf::FloatRect getBounds() const override;
      void handleEvent(const sf::Event &event) override;

      void setCallback(TextInput::Callback callback);
      void setSize(float width, float height);

    protected:
      void draw(RenderTarget &target, sf::RenderStates states) const override;

    private:
      bool hasFocus;
      std::string inputString;
      sf::Text text;
      sf::RectangleShape background;
      sf::RectangleShape caret;
      TextInput::Callback callback;
  };
}

#endif //BREAK_IT_TEXTINPUT_HPP
