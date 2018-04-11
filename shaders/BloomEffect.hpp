#ifndef BREAK_IT_BLOOMEFFECT_HPP
#define BREAK_IT_BLOOMEFFECT_HPP

#include "PostEffect.hpp"
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include "../ResourceIdentifiers.hpp"
#include "../system/ResourceHolder.hpp"

#include <array>


class BloomEffect : public PostEffect {
public:
    BloomEffect();
    void apply(const sf::RenderTexture &input, sf::RenderTarget &output) override;

private:
    typedef std::array<sf::RenderTexture, 2> RenderTextureArray;

    void prepareTextures(sf::Vector2u size);

    void filterBright(const sf::RenderTexture& input, sf::RenderTexture& output);
    void blurMultipass(RenderTextureArray& renderTextures);
    void blur(const sf::RenderTexture& input, sf::RenderTexture& output, sf::Vector2f offsetFactor);
    void downsample(const sf::RenderTexture& input, sf::RenderTexture& output);
    void add(const sf::RenderTexture& source, const sf::RenderTexture& bloom, sf::RenderTarget& target);


    ShaderHolder shaders;

    sf::RenderTexture brightnessTexture;
    RenderTextureArray firstPassTextures;
    RenderTextureArray secondPassTextures;
};


#endif //BREAK_IT_BLOOMEFFECT_HPP
