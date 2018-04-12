//
// Created by Dimi on 12.04.2018.
//

#include "Space.hpp"
#include "../system/Utility.hpp"

Space::Space()
        : Space(20, 20, 0.53f)
{}

Space::Space(int iterations, int steps, float formuparam)
        : shaders(), spaceTexture(),
          iterations(iterations), steps(steps), formuparam(formuparam)
{
    shaders.load(Shaders::Space, "assets/shaders/Fullpass.vert", "assets/shaders/Space.frag");
    from = sf::Vector3f(Random::decimal(0, 1), Random::decimal(0, 1), Random::decimal(0, 1));
}

void Space::apply(const sf::RenderTexture &input, sf::RenderTarget &output) {
    prepareTextures(input.getSize());
    space(input, output);
}

void Space::prepareTextures(sf::Vector2u size) {
    if (spaceTexture.getSize() != size) {
        spaceTexture.create(size.x, size.y);
        spaceTexture.setSmooth(true);
    }
}

void Space::space(const sf::RenderTexture &input, sf::RenderTarget &output) {
    sf::Shader& shader = shaders.get(Shaders::Space);

    shader.setUniform("iResolution", sf::Vector3f(input.getSize().x, input.getSize().y, 0));
    shader.setUniform("iTime", time.asSeconds());
    shader.setUniform("iterations", iterations);
    shader.setUniform("volsteps", steps);
    shader.setUniform("formuparam", formuparam);
    shader.setUniform("from", from);
    applyShader(shader, output);
}

void Space::update(sf::Time dt) {
    time += dt;
}
