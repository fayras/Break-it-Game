#ifndef SFML_TEMPLATE_BUTTON_HPP
#define SFML_TEMPLATE_BUTTON_HPP

#include "Component.hpp"
#include "../ResourceIdentifiers.hpp"
#include "../system/ResourceHolder.hpp"
#include "../system/State.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <vector>
#include <string>
#include <memory>
#include <functional>

namespace gui {
  class Button : public Component {
    public:
      typedef std::shared_ptr<Button> Ptr;
      typedef std::function<void()> Callback;

      explicit Button(State::Context context);
      void setCallback(Callback callback);
      void setText(const std::string& text);
      void setToggle(bool flag);

      bool selectable() const override;
      void select() override;
      void deselect() override;

      void activate() override;
      void deactivate() override;
      void handleEvent(const sf::Event& event) override;
      sf::FloatRect getBounds() const override;

    protected:
      void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

      Callback callback;
      sf::Texture& normalTexture;
      sf::Texture& selectedTexture;
      sf::Texture& pressedTexture;
      sf::Sprite sprite;
      sf::Text text;
      bool isToggle;
  };
}

#endif //SFML_TEMPLATE_BUTTON_HPP
