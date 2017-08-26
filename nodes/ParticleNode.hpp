#ifndef SFML_TEMPLATE_PARTICLENODE_HPP
#define SFML_TEMPLATE_PARTICLENODE_HPP

#include "SceneNode.hpp"
#include "../system/Particle.hpp"
#include "../ResourceIdentifiers.hpp"
#include <SFML/Graphics/VertexArray.hpp>
#include <deque>

class ParticleNode : public SceneNode {
  public:
    ParticleNode(Particle::Type type, const TextureHolder& textures);
    void addParticle(sf::Vector2f position);
    void clearParticles();
    Particle::Type getParticleType() const;
    unsigned int getCategory() const override;

  private:
    void updateCurrent(sf::Time dt, CommandQueue& commands) override;
    void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

    void addVertex(float worldX, float worldY, float texCoordX, float texCoordY, const sf::Color& color) const;
    void computeVertices() const;

    std::deque<Particle> particles;
    const sf::Texture& texture;
    Particle::Type type;
    mutable sf::VertexArray vertexArray;
    mutable bool needsVertexUpdate;
};

#endif //SFML_TEMPLATE_PARTICLENODE_HPP
