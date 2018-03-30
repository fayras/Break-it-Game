#include <map>
#include "DataTables.hpp"
#include "system/Particle.hpp"

namespace {
  const std::map<char, sf::Color> getColors() {
    return {
        { 'a', sf::Color(255, 100, 100) },
        { 'b', sf::Color(150, 255, 0) },
        { 'c', sf::Color(100, 100, 255) },
        { '-', sf::Color(0, 0, 0) },
        { '+', sf::Color(255, 255, 255) },
        { 'g', sf::Color(150, 150, 150) },

        { 'q', sf::Color(102, 78, 50) },
        { 'w', sf::Color(208, 148, 76) },
        { 'r', sf::Color(168, 118, 36) },
        { 'e', sf::Color(255, 220, 205) },
        { 'p', sf::Color(255, 153, 153) },
    };
  };

  std::vector<std::vector<std::string>> getLevels() {
    return {
        {
            "aaaaaaaaaaaaa",
            "bbbbbbbbbbbbb",
            "ccccccccccccc",
            "aaaaaaaaaaaaa",
            "bbbbbbbbbbbbb",
            "ccccccccccccc"
        },
        {
            "    q        q",
            "    qq      qq",
            "    qeq    qeq",
            "    qwwqqqqwwq",
            "   qwwwwrwrwwq",
            "   qwwqwrwwqwwq",
            "  qqwpwwwwwwpwqq",
            "   qrrwwwewwwrq",
            "  qqwwweeweewwqq",
            "    qwwrwwwrwq",
            "     qqqqqqqq"
        },
        {
            "a a a a a a a",
            "             ",
            "a a a a a a a",
            "             ",
            "b b b b b b b",
            "             ",
            "c c c c c c c"
        }
    };
  }
}

std::vector<LevelData> initializeLevelData() {
  auto levels = getLevels();
  auto colors = getColors();
  std::vector<LevelData> data(levels.size());

  for(std::size_t i = 0; i < levels.size(); i++) {
    std::size_t levelSize = levels[i].size();
    for(std::size_t y = 0; y < levelSize; y++) {
      std::size_t rowLength = levels[i][y].size();
      for(std::size_t x = 0; x < rowLength; x++) {
        char colorKey = levels[i][y].at(static_cast<unsigned long>(x));
        if(colorKey != ' ') {
          const sf::Color& color = colors.at(colorKey);
          data[i].blockColors.emplace_back(std::make_pair(sf::Vector2i(x, y), color));
        }
      }
    }
  }

  data[1].ballSpeedMultiplier = 1.5f;
  data[1].blockType = Blocks::SMALL;

  return data;
}

std::vector<ParticleData> initializeParticleData() {
  std::vector<ParticleData> data(Particle::ParticleCount);

  data[Particle::Propellant].color = sf::Color(255, 255, 170);
  data[Particle::Propellant].lifetime = sf::seconds(0.3f);

  data[Particle::Smoke].color = sf::Color(50, 50, 50);
  data[Particle::Smoke].lifetime = sf::seconds(4.f);

  return data;
}
