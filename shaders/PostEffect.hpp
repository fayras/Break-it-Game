#ifndef BREAK_IT_POSTEFFECT_HPP
#define BREAK_IT_POSTEFFECT_HPP


#include <SFML/Graphics/RenderTexture.hpp>

class PostEffect {
    public:
        virtual ~PostEffect();
        virtual void apply(const sf::RenderTexture& input, sf::RenderTarget& output) = 0;

        static bool isSupported();

    protected:
        static void applyShader(const sf::Shader& shader, sf::RenderTarget& output);
};


#endif //BREAK_IT_POSTEFFECT_HPP
