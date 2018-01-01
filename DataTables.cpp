#include <map>
#include "DataTables.hpp"
#include "system/Particle.hpp"

namespace {
  const std::map<char, sf::Color> colors {
      { 'a', sf::Color(255, 100, 100) },
      { 'b', sf::Color(150, 255, 0) },
      { 'c', sf::Color(100, 100, 255) },
      { '-', sf::Color(0, 0, 0) },
      { '+', sf::Color(255, 255, 255) },
      { 'g', sf::Color(150, 150, 150) }
  };

  const std::vector<std::vector<std::string>> levels {
//      {
//          "             ",
//          "             ",
//          "             ",
//          "             ",
//          "             ",
//          "             ",
//          "      a      "
//      },
      {
          "aaaaaaaaaaaaa",
          "bbbbbbbbbbbbb",
          "ccccccccccccc",
          "aaaaaaaaaaaaa",
          "bbbbbbbbbbbbb",
          "ccccccccccccc"
      },
      {
          "a a a a a a a",
          "             ",
          "a a a a a a a",
          "             ",
          "b b b b b b b",
          "             ",
          "c c c c c c c"
      },
      {
          "  g       g  ",
          " ggg     ggg ",
          " ggggggggggg ",
          "ggg ggggg ggg",
          "ggggggggggggg",
          "gggggg gggggg",
          " g gg g gg g ",
          "  g  ggg  g  ",
          "  ggggggggg  ",
          "    ggggg    "
      }
  };
}

std::vector<LevelData> initializeLevelData() {
  std::vector<LevelData> data(levels.size());

  for(int i = 0; i < levels.size(); i++) {
    std::size_t levelSize = levels[i].size();
    for(int y = 0; y < levelSize; y++) {
      std::size_t rowLength = levels[i][y].size();
      for(int x = 0; x < rowLength; x++) {
        char colorKey = levels[i][y].at(static_cast<unsigned long>(x));
        if(colorKey != ' ') {
          const sf::Color& color = colors.at(colorKey);
          data[i].blockColors.emplace_back(std::make_pair(sf::Vector2i(x, y), color));
        }
      }
    }
  }

  data[1].ballSpeedMultiplier = 1.5f;

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
