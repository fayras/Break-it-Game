//
// Created by Dimi on 09.05.2018.
//

#include <SFML/Graphics/RenderWindow.hpp>
#include "StoreState.hpp"
#include "../system/ResourceHolder.hpp"
#include "../gui/Button.hpp"
#include "../gui/Label.hpp"

StoreState::StoreState(StateStack &stack, State::Context context)
    : State(stack, context),
      background(sf::Vector2f(context.window->getSize().x, context.window->getSize().y)),
      guiContainer()
{
    background.setFillColor(sf::Color(0, 0, 0, 170));
    context.saveData->get("skillsUnlocked", skillsUnlocked);

    sf::Sprite sprite1{ context.textures->get(Textures::SKILLS), sf::IntRect{120, 120, 40, 40} };
    sf::Sprite sprite2{ context.textures->get(Textures::SKILLS), sf::IntRect{320, 0, 40, 40} };
    sf::Text text1{ "Dupliziert den Ball\nKosten: 3000 Punkte", context.fonts->get(Fonts::MAIN) };
    sf::Text text2{ L"Verlangsamt den Ball für einige Sekunden\nKosten: 8000 Punkte", context.fonts->get(Fonts::MAIN) };

    skillSprites.emplace(Skills::ID::SLOWMOTION, std::move(std::pair<sf::Sprite, sf::Text>(sprite2, text2)));
    skillSprites.emplace(Skills::ID::DUPLICATE_BALL, std::move(std::pair<sf::Sprite, sf::Text>(sprite1, text1)));
    // skillSprites.emplace("AAA", sf::Sprite{ context.textures->get(Textures::SKILLS), sf::IntRect{320, 0, 40, 40} });

    auto score = getContext().saveData->get("player_score", 0);

    auto availablePointsText = std::make_shared<gui::Label>(
            std::wstring(L"Verfügbare Punkte: "),
            context.fonts->get(Fonts::MAIN)
    );
    availablePointsText->setSize(25);
    availablePointsText->setPosition(100, 50);

    auto availablePointsLabel = std::make_shared<gui::Label>(
            std::to_string(getContext().saveData->get("player_score", 0)),
            context.fonts->get(Fonts::MAIN)
    );
    auto availablePointsPointer = availablePointsLabel.get();
    availablePointsLabel->setSize(25);
    availablePointsLabel->setPosition(330, 50);

    auto btDup = std::make_shared<gui::Button>(context);
    auto btDupPointer = btDup.get();
    btDup->setPosition(600, 130);
    if(skillUnlocked(Skills::ID::DUPLICATE_BALL)) {
        btDup->setText("Freigeschaltet");
        btDup->disable(true);
    } else if(score < 3000) {
        btDup->setText("Nicht genug Punkte");
        btDup->disable(true);
    } else {
        btDup->setText("Freischalten");
        btDup->setCallback([this, btDupPointer, availablePointsPointer] () {
            if(unlockSkill(Skills::ID::DUPLICATE_BALL, 3000)) {
                availablePointsPointer->setText(std::to_string(getContext().saveData->get("player_score", 0)));
                btDupPointer->setText("Freigeschaltet");
                btDupPointer->disable(true);
            }
        });
    }

    auto btSlow = std::make_shared<gui::Button>(context);
    auto btSlowPointer = btSlow.get();
    btSlow->setPosition(600, 280);
    if(skillUnlocked(Skills::ID::SLOWMOTION)) {
        btSlow->setText("Freigeschaltet");
        btSlow->disable(true);
    } else if(score < 8000) {
        btSlow->setText("Nicht genug Punkte");
        btSlow->disable(true);
    } else {
        btSlow->setText("Freischalten");
        btSlow->setCallback([this, btSlowPointer, availablePointsPointer] () {
            if(unlockSkill(Skills::ID::SLOWMOTION, 8000)) {
                availablePointsPointer->setText(std::to_string(getContext().saveData->get("player_score", 0)));
                btSlowPointer->setText("Freigeschaltet");
                btSlowPointer->disable(true);
            }
        });
    }

    auto backButton = std::make_shared<gui::Button>(context);
    backButton->setPosition(80.0f, 600.0f);
    backButton->setText(L"Zurück");
    backButton->setShortcut(sf::Keyboard::Escape);
    backButton->setCallback(std::bind(&StoreState::requestStackPop, this));

    guiContainer.pack(btDup);
    guiContainer.pack(btSlow);
    guiContainer.pack(backButton);
    guiContainer.pack(availablePointsText);
    guiContainer.pack(availablePointsLabel);
}

void StoreState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());
    window.draw(background);
    int i = 0;
    for(auto& skill : skillSprites) {
        skill.second.first.setPosition(100, i * 150 + 130);
        skill.second.second.setPosition(200, i * 150 + 130);
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

bool StoreState::unlockSkill(const std::string &string, int cost) {
    auto score = getContext().saveData->get("player_score", 0);
    if(!skillUnlocked(string) && score >= cost) {
        skillsUnlocked.push_back(string);
        getContext().saveData->set("skillsUnlocked", skillsUnlocked);
        getContext().saveData->set("player_score", score - cost);
        getContext().saveData->saveToFile();
        return true;
    }

    return false;
}
