#ifndef BREAK_IT_DATATABLES_HPP
#define BREAK_IT_DATATABLES_HPP

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
#include <vector>

struct ParticleData {
  sf::Color color;
  sf::Time lifetime;
};

std::vector<ParticleData>	initializeParticleData();


#endif //BREAK_IT_DATATABLES_HPP
