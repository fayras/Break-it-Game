#ifndef SFML_TEMPLATE_RESOURCEIDENTIFIERS_HPP
#define SFML_TEMPLATE_RESOURCEIDENTIFIERS_HPP

// Forward declaration of SFML classes
namespace sf {
  class Texture;
  class Font;
  class Shader;
  class SoundBuffer;
}

namespace Textures {
  enum ID {
    BUTTON_NORMAL,
    BUTTON_SELECTED,
    BUTTON_PRESSED,
    PARTICLE,
    PADDLE,
    BLOCK
  };
}

namespace Fonts {
  enum ID {
    MAIN,
  };
}

namespace SoundEffect {
  enum ID {

  };
}

namespace Music {
  enum ID {

  };
}


// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID>	TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;
typedef ResourceHolder<sf::SoundBuffer, SoundEffect::ID> SoundBufferHolder;

#endif //SFML_TEMPLATE_RESOURCEIDENTIFIERS_HPP
