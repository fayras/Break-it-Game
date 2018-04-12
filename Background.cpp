#include "Background.hpp"

Background::Background(const sf::Texture &texture, sf::View &worldView)
        : SpriteNode(texture), spaceEffect(15, 17, 0.53f)
{
    setCategory(Category::BACKGROUND);
    this->texture.setView(worldView);
    this->texture.create((unsigned int) worldView.getSize().x + 100, (unsigned int) worldView.getSize().y + 100);
}

void Background::updateCurrent(sf::Time dt, CommandQueue &commands) {
    if(PostEffect::isSupported()) {
        spaceEffect.update(dt);
        texture.clear();
        spaceEffect.apply(texture, texture);
        texture.display();
        sprite.setTexture(texture.getTexture());
    } else {
        move(0, 100 * dt.asSeconds());
        // keep bg repeating. 1400 = size of texture
        if(getPosition().y > 0) {
            setPosition(0, -1400);
        }
    }
}
