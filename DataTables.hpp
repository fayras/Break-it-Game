#ifndef BREAK_IT_DATATABLES_HPP
#define BREAK_IT_DATATABLES_HPP

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System.hpp>
#include <vector>
#include "ResourceIdentifiers.hpp"
#include "entities/BlockTypes.hpp"

struct ParticleData {
  sf::Color color;
  sf::Time lifetime;
};

struct LevelData {
  std::vector<std::pair<sf::Vector2i, sf::Color>> blockColors;
  Blocks::Type blockType{Blocks::NORMAL};
  float ballSpeedMultiplier{1.0f};
  float scoreMultiplier{1.0f};
  sf::Vector2f spawnPosition{0.5f, 0.94f};
};

std::vector<ParticleData>	initializeParticleData();
std::vector<LevelData> initializeLevelData();

#endif //BREAK_IT_DATATABLES_HPP
