//
// Created by Dimi on 20.05.2018.
//

#include <SFML/Graphics/RenderWindow.hpp>
#include <cmath>
#include "../system/ResourceHolder.hpp"
#include "LevelFinished.hpp"
#include "../gui/Button.hpp"

LevelFinished::LevelFinished(StateStack &stack, State::Context context) : State(stack, context) {
    sf::Font& font = context.fonts->get(Fonts::MAIN);
    sf::Vector2f windowSize(context.window->getSize());

    text.setFont(font);
    text.setString("Level beendet");
    text.setCharacterSize(70);
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
    text.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);

    backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
    backgroundShape.setSize(windowSize);

    auto btContinue = std::make_shared<gui::Button>(context);
    btContinue->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 75);
    btContinue->setText("Weiter");
    btContinue->setCallback([this] () {
        requestStackPop();
    });

    auto btStore = std::make_shared<gui::Button>(context);
    btStore->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 125);
    btStore->setText("Shop");
    btStore->setCallback([this] () {
        requestStackPush(States::STORE);
    });

    guiContainer.pack(btContinue);
    guiContainer.pack(btStore);
}

void LevelFinished::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());

    window.draw(backgroundShape);
    window.draw(guiContainer);
    window.draw(text);
}

bool LevelFinished::update(sf::Time dt) {
    guiContainer.update(dt);
    return false;
}

bool LevelFinished::handleEvent(const sf::Event &event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        requestStackPop();
        return false;
    }
    guiContainer.handleEvent(event);
    return false;
}
