#ifndef SFML_TEMPLATE_BUTTON_HPP
#define SFML_TEMPLATE_BUTTON_HPP

#include "Component.hpp"
#include "../ResourceIdentifiers.hpp"
#include "../system/ResourceHolder.hpp"
#include "../system/State.hpp"
#include "../tween/Tweenable.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <SFML/Graphics/RectangleShape.hpp>

namespace gui {
  class Button : public Component, public Tweenable {
    public:
      typedef std::shared_ptr<Button> Ptr;

      explicit Button(State::Context context);
      void setCallback(Callback callback);
      void setText(const std::string& text);
      void setText(const std::wstring& text);
      void setToggle(bool flag);
      void setShortcut(sf::Keyboard::Key key);

      bool selectable() const override;
      void select() override;
      void deselect() override;
      void disable(bool flag = true);

      void activate() override;
      void deactivate() override;
      void handleEvent(const sf::Event& event) override;

      void update(sf::Time dt) override;

      sf::FloatRect getBounds() const override;

    protected:
      void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
      void updatePositions();

      Callback callback;
      sf::RectangleShape deco;
      sf::RectangleShape hover;
      sf::Text text;
      sf::Text shortcutText;
      float maxTextWidth;
      bool isToggle;
      bool disabled;
      sf::Keyboard::Key shortcut;
  };
}

#endif //SFML_TEMPLATE_BUTTON_HPP
