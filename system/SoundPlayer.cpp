#include "SoundPlayer.hpp"
#include <SFML/Audio/Listener.hpp>
#include <cmath>

namespace {
  // Sound coordinate system, point of view of a player in front of the screen:
  // X = left; Y = up; Z = back (out of the screen)
  const float ListenerZ = 300.f;
  const float Attenuation = 8.f;
  const float MinDistance2D = 200.f;
  const float MinDistance3D = std::sqrt(MinDistance2D*MinDistance2D + ListenerZ*ListenerZ);
}

SoundPlayer::SoundPlayer() {
  soundBuffers.load(SoundEffect::HIT_BLOCK,	"assets/sounds/Wav/Explosion_02.wav");
  soundBuffers.load(SoundEffect::HIT_GENERAL,	"assets/sounds/Wav/Hit_01.wav");

  // Listener points towards the screen (default in SFML)
  sf::Listener::setDirection(0.f, 0.f, -1.f);
}

void SoundPlayer::play(SoundEffect::ID effect) {
  play(effect, getListenerPosition());
}

void SoundPlayer::play(SoundEffect::ID effect, sf::Vector2f position) {
  sounds.emplace_back(sf::Sound());
  sf::Sound& sound = sounds.back();

  sound.setBuffer(soundBuffers.get(effect));
  sound.setPosition(position.x, -position.y, 0.f);
  sound.setAttenuation(Attenuation);
  sound.setMinDistance(MinDistance3D);
  sound.setVolume(sound.getVolume() * volume);

  sound.play();
}

void SoundPlayer::removeStoppedSounds() {
  sounds.remove_if([] (const sf::Sound& s) {
    return s.getStatus() == sf::Sound::Stopped;
  });
}

void SoundPlayer::setListenerPosition(sf::Vector2f position) {
  sf::Listener::setPosition(position.x, -position.y, ListenerZ);
}

sf::Vector2f SoundPlayer::getListenerPosition() const {
  sf::Vector3f position = sf::Listener::getPosition();
  return {position.x, -position.y};
}

void SoundPlayer::setMasterVolume(float volume) {
  this->volume = volume / 100;
}

float SoundPlayer::getMasterVolume() const {
  return volume * 100;
}
