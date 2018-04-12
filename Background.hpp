//
// Created by Dimi on 12.04.2018.
//

#ifndef BREAK_IT_BACKGROUND_HPP
#define BREAK_IT_BACKGROUND_HPP


#include <SFML/Graphics/RenderTexture.hpp>
#include "nodes/SpriteNode.hpp"
#include "shaders/Space.hpp"

class Background : public SpriteNode {
public:
    explicit Background(const sf::Texture &texture, sf::View &worldView);

protected:
    void updateCurrent(sf::Time dt, CommandQueue &commands) override;

private:
    sf::RenderTexture texture;
    Space spaceEffect;
};


#endif //BREAK_IT_BACKGROUND_HPP
