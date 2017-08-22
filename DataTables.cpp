#include "DataTables.hpp"
#include "system/Particle.hpp"

std::vector<ParticleData> initializeParticleData() {
  std::vector<ParticleData> data(Particle::ParticleCount);

  data[Particle::Propellant].color = sf::Color(255, 255, 50);
  data[Particle::Propellant].lifetime = sf::seconds(0.6f);

  data[Particle::Smoke].color = sf::Color(50, 50, 50);
  data[Particle::Smoke].lifetime = sf::seconds(4.f);

  return data;
}

std::vector<LevelData> initializeLevelData() {
  std::vector<LevelData> data(2);

  data[0].ballSpeedMultiplier = 1.0f;
  for(int x = 6; x < 7; x++) {
    for(int y = 0; y < 1; y++) {
      sf::Color color;
      if(y < 7) { color.r = 255; color.g = 100; color.b = 100; }
      if(y < 4) { color.r = 150; color.g = 255; color.b = 0; }
      if(y < 2) { color.r = 100; color.g = 100; color.b = 255; }
      data[0].blockColors.emplace_back(std::make_pair(sf::Vector2i(x, y), color));
    }
  }

  data[1].ballSpeedMultiplier = 1.5f;
  for(int x = 0; x < 13; x += 2) {
    for(int y = 0; y < 7; y += 2) {
      sf::Color color;
      if(y < 7) { color.r = 255; color.g = 100; color.b = 100; }
      if(y < 4) { color.r = 150; color.g = 255; color.b = 0; }
      if(y < 2) { color.r = 100; color.g = 100; color.b = 255; }
      data[1].blockColors.emplace_back(std::make_pair(sf::Vector2i(x, y), color));
    }
  }

  return data;
}
