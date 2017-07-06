#ifndef SFML_TEMPLATE_CATEGORY_HPP
#define SFML_TEMPLATE_CATEGORY_HPP

namespace Category {
  enum Type {
    NONE = 0,
    SCENE = 1 << 0,
    SOUND_EFFECT = 1 << 1,
    PARTICLE_SYSTEM = 1 << 2,
    PLAYER = 1 << 3
  };
}

#endif //SFML_TEMPLATE_CATEGORY_HPP
