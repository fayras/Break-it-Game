#include <map>
#include "DataTables.hpp"
#include "system/Particle.hpp"

std::vector<LevelData> initializeLevelData() {
  std::vector<LevelData> data(4);

  data[0].filename = "assets/level0.json";
  data[1].filename = "assets/level1.json";
  data[2].filename = "assets/level2.json";
  data[3].filename = "assets/level3.json";

  //data[1].ballSpeedMultiplier = 1.5f;
  //data[1].blockType = Blocks::SMALL;

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

std::vector<SkillData> initializeSkillData() {
  std::vector<SkillData> data(2);

  data[0].id = Skills::ID::DUPLICATE_BALL;
  data[0].description = L"Dupliziert den Ball\nKosten: 1000 Punkte";
  data[0].spriteRect = sf::IntRect{120, 120, 40, 40};
  data[0].cost = 1000;

  data[1].id = Skills::ID::SLOWMOTION;
  data[1].description = L"Verlangsamt den Ball für einige Sekunden\nKosten: 3000 Punkte";
  data[1].spriteRect = sf::IntRect{320, 0, 40, 40};
  data[1].cost = 3000;

  return data;
}
