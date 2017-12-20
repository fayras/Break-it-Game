#ifndef SFML_TEMPLATE_CATEGORY_HPP
#define SFML_TEMPLATE_CATEGORY_HPP

namespace Category {
  enum Type {
    NONE = 0,
    SCENE = 1 << 0,
    SOUND_EFFECT = 1 << 1,
    PARTICLE_SYSTEM = 1 << 2,
    BALL = 1 << 3,
    PADDLE = 1 << 4,
    BLOCK = 1 << 5,
    WALL = 1 << 6,
    SCORE = 1 << 7,
    LIFE = 1 << 8
  };
}

#endif //SFML_TEMPLATE_CATEGORY_HPP
