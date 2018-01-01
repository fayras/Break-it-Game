#ifndef SFML_TEMPLATE_MUSICPLAYER_HPP
#define SFML_TEMPLATE_MUSICPLAYER_HPP

#include "ResourceHolder.hpp"
#include "../ResourceIdentifiers.hpp"
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Audio/Music.hpp>

#include <map>
#include <string>

class MusicPlayer : private sf::NonCopyable {
  public:
    MusicPlayer();
    void play(Music::ID theme);
    void stop();
    void setPaused(bool paused);
    float getVolume() const;
    void setVolume(float volume);

  private:
    sf::Music music;
    std::map<Music::ID, std::string> filenames;
    float volume;
};


#endif //SFML_TEMPLATE_MUSICPLAYER_HPP
