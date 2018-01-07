#ifndef BREAK_IT_BLOCKTYPES_HPP
#define BREAK_IT_BLOCKTYPES_HPP

namespace Blocks {
  enum Type {
    NORMAL = 0,
    DOUBLE_HP = 1 << 0,
    INVISIBLE = 1 << 1,
    SELF_HEALING = 1 << 2,
    SMALL = 1 << 3,
  };

  inline Type operator|(Type a, Type b) {
    return static_cast<Type>(static_cast<int>(a) | static_cast<int>(b));
  }

  inline bool operator==(Type a, Type b) {
    return static_cast<bool>(static_cast<int>(a) & static_cast<int>(b));
  }
}

#endif //BREAK_IT_BLOCKTYPES_HPP
