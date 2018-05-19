//
// Created by Dimi on 09.05.2018.
//

#include <SFML/Graphics/RenderWindow.hpp>
#include "StoreState.hpp"
#include "../system/ResourceHolder.hpp"
#include "../gui/Button.hpp"

StoreState::StoreState(StateStack &stack, State::Context context)
    : State(stack, context),
      guiContainer()
{
    context.saveData->get("skillsUnlocked", skillsUnlocked);

    sf::Sprite sprite1{ context.textures->get(Textures::SKILLS), sf::IntRect{120, 120, 40, 40} };
    sf::Text text1{ "Dupliziert den Ball\nKosten: 3000 Punkte", context.fonts->get(Fonts::MAIN) };
    skillSprites.emplace(Skills::ID::DUPLICATE_BALL, std::move(std::pair<sf::Sprite, sf::Text>(sprite1, text1)));
    skillSprites.emplace(Skills::ID::SLOWMOTION, std::move(std::pair<sf::Sprite, sf::Text>(sprite1, text1)));
    // skillSprites.emplace("AAA", sf::Sprite{ context.textures->get(Textures::SKILLS), sf::IntRect{320, 0, 40, 40} });

    auto btDup = std::make_shared<gui::Button>(context);
    auto btDupPointer = btDup.get();
    btDup->setPosition(600, 100);
    if(skillUnlocked(Skills::ID::DUPLICATE_BALL)) {
        btDup->setText("Bereits Freigeschaltet");
        btDup->disable(true);
    } else {
        btDup->setText("Freischalten");
        btDup->setCallback([this, btDupPointer] () {
            if(std::find(skillsUnlocked.begin(), skillsUnlocked.end(), Skills::ID::DUPLICATE_BALL) == skillsUnlocked.end()) {
                skillsUnlocked.push_back(Skills::ID::DUPLICATE_BALL);
                this->context.saveData->set("skillsUnlocked", skillsUnlocked);
                this->context.saveData->saveToFile();
                btDupPointer->setText("Bereits Freigeschaltet");
                btDupPointer->disable(true);
            }
        });
    }

    auto backButton = std::make_shared<gui::Button>(context);
    backButton->setPosition(80.0f, 600.0f);
    backButton->setText(L"Zurück");
    backButton->setShortcut(sf::Keyboard::Escape);
    backButton->setCallback(std::bind(&StoreState::requestStackPop, this));

    guiContainer.pack(btDup);
    guiContainer.pack(backButton);
}

void StoreState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());
    int i = 0;
    for(auto& skill : skillSprites) {
        skill.second.first.setPosition(100, i * 150 + 100);
        skill.second.second.setPosition(200, i * 150 + 100);
        skill.second.second.setCharacterSize(15);
        window.draw(skill.second.first);
        window.draw(skill.second.second);

        i++;
    }
    window.draw(guiContainer);
}

bool StoreState::update(sf::Time dt) {
    guiContainer.update(dt);
    return false;
}

bool StoreState::handleEvent(const sf::Event &event) {
    guiContainer.handleEvent(event);
    return false;
}

bool StoreState::skillUnlocked(const std::string &string) const {
    return std::find(skillsUnlocked.begin(), skillsUnlocked.end(), string) != skillsUnlocked.end();
}
