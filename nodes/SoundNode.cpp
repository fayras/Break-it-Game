#include "SoundNode.hpp"
#include "../system/SoundPlayer.hpp"

SoundNode::SoundNode(SoundPlayer &player)
  : SceneNode(Category::SOUND_EFFECT), sounds(player)
{}

void SoundNode::playSound(SoundEffect::ID sound, sf::Vector2f position) {
  sounds.play(sound, position);
}
