#include "MusicPlayer.hpp"

MusicPlayer::MusicPlayer()
  : volume(100.f)
{
  filenames[Music::GAME] = "./assets/music/Retro1 140.wav";
}

void MusicPlayer::play(Music::ID theme) {
  std::string filename = filenames[theme];

  if (!music.openFromFile(filename))
    throw std::runtime_error("Music " + filename + " could not be loaded.");

  music.setVolume(volume);
  music.setLoop(true);
  music.play();
}

void MusicPlayer::stop() {
  music.stop();
}

void MusicPlayer::setVolume(float volume) {
  this->volume = volume;
}

void MusicPlayer::setPaused(bool paused)
{
  if (paused) music.pause();
  else music.play();
}

float MusicPlayer::getVolume() const {
  return volume;
}
