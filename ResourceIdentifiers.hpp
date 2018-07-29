#ifndef SFML_TEMPLATE_RESOURCEIDENTIFIERS_HPP
#define SFML_TEMPLATE_RESOURCEIDENTIFIERS_HPP

#include <string>

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
    BLOCK,
    BLOCK_SMALL,
    BALL,
    EXPLOSION,
    STARFIELD,
    TITLE,
    SCORE,
    GAME_OVER_SCREEN,
    GAME_WIN_SCREEN,
    LIFE,
    SKILLS
  };
}

namespace Fonts {
  enum ID {
    MAIN,
    PIXEL,
    NARROW
  };
}

namespace SoundEffect {
  enum ID {
    HIT_BLOCK,
    HIT_GENERAL
  };
}

namespace Music {
  enum ID {
    GAME,
    TITLE
  };
}

namespace Shaders {
    enum ID {
        BrightnessPass,
        DownSamplePass,
        GaussianBlurPass,
        AddPass,
        Space
    };
}

namespace Skills {
    namespace ID {
        extern const std::string DUPLICATE_BALL;
        extern const std::string SLOWMOTION;
    }
}

// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;
typedef ResourceHolder<sf::SoundBuffer, SoundEffect::ID> SoundBufferHolder;
typedef ResourceHolder<sf::Shader, Shaders::ID>	ShaderHolder;

#endif //SFML_TEMPLATE_RESOURCEIDENTIFIERS_HPP
