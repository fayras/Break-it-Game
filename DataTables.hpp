#ifndef BREAK_IT_DATATABLES_HPP
#define BREAK_IT_DATATABLES_HPP

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System.hpp>
#include <vector>

struct ParticleData {
  sf::Color color;
  sf::Time lifetime;
};

struct LevelData {
  std::vector<std::pair<sf::Vector2i, sf::Color>> blockColors;
  float ballSpeedMultiplier;
  float scoreMultiplier;
};

std::vector<ParticleData>	initializeParticleData();
std::vector<LevelData> initializeLevelData();

#endif //BREAK_IT_DATATABLES_HPP
