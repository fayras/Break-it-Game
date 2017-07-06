#ifndef SFML_TEMPLATE_SOUNDNODE_HPP
#define SFML_TEMPLATE_SOUNDNODE_HPP

#include "SceneNode.hpp"
#include "../ResourceIdentifiers.hpp"

class SoundPlayer;

class SoundNode : public SceneNode {
  public:
    explicit SoundNode(SoundPlayer& player);
    void playSound(SoundEffect::ID sound, sf::Vector2f position);

  private:
    SoundPlayer& sounds;
};


#endif //SFML_TEMPLATE_SOUNDNODE_HPP
