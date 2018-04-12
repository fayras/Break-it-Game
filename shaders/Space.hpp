#ifndef BREAK_IT_SPACE_HPP
#define BREAK_IT_SPACE_HPP

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include "../ResourceIdentifiers.hpp"
#include "../system/ResourceHolder.hpp"
#include "PostEffect.hpp"

#include <array>
#include <SFML/System/Time.hpp>

class Space : public PostEffect {
public:
    Space();
    Space(int iterations, int steps, float formuparam);
    void apply(const sf::RenderTexture &input, sf::RenderTarget &output) override;
    void update(sf::Time dt);

private:
    void prepareTextures(sf::Vector2u size);

    void space(const sf::RenderTexture &input, sf::RenderTarget &output);

    ShaderHolder shaders;

    sf::RenderTexture spaceTexture;
    sf::Time time;
    int iterations;
    int steps;
    float formuparam;
};


#endif //BREAK_IT_SPACE_HPP
