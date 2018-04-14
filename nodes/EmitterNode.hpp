#ifndef SFML_TEMPLATE_EMITTERNODE_HPP
#define SFML_TEMPLATE_EMITTERNODE_HPP

#include "SceneNode.hpp"
#include "../system/Particle.hpp"

class ParticleNode;

class EmitterNode : public SceneNode {
  public:
    explicit EmitterNode(Particle::Type type, float emissionRate = 100.f);
    void setEmissionRate(float rate);

  private:
    void updateCurrent(sf::Time dt, CommandQueue& commands) override;
    void emitParticles(sf::Time dt);

    sf::Time accumulatedTime;
    Particle::Type type;
    ParticleNode* particleSystem;
    float emissionRate;
};

#endif //SFML_TEMPLATE_EMITTERNODE_HPP
