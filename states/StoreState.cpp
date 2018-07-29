//
// Created by Dimi on 09.05.2018.
//

#include <SFML/Graphics/RenderWindow.hpp>
#include "StoreState.hpp"
#include "../system/ResourceHolder.hpp"
#include "../gui/Button.hpp"
#include "../gui/Label.hpp"
#include "../DataTables.hpp"

namespace {
    const std::vector<SkillData> SkillTable = initializeSkillData();
}

StoreState::StoreState(StateStack &stack, State::Context context)
    : State(stack, context),
      background(sf::Vector2f(context.window->getSize().x, context.window->getSize().y)),
      guiContainer()
{
    background.setFillColor(sf::Color(0, 0, 0, 170));

    auto score = getContext().saveData->getScore();

    auto availablePointsText = std::make_shared<gui::Label>(
            std::wstring(L"Verfügbare Punkte: "),
            context.fonts->get(Fonts::MAIN)
    );
    availablePointsText->setSize(25);
    availablePointsText->setPosition(100, 50);

    auto availablePointsLabel = std::make_shared<gui::Label>(
            std::to_string(getContext().saveData->getScore()),
            context.fonts->get(Fonts::MAIN)
    );
    auto availablePointsPointer = availablePointsLabel.get();
    availablePointsLabel->setSize(25);
    availablePointsLabel->setPosition(330, 50);

    int i = 0;
    for(auto const &skill : SkillTable) {
        sf::Sprite sprite{ context.textures->get(Textures::SKILLS), skill.spriteRect };
        sf::Text text{ skill.description, context.fonts->get(Fonts::MAIN) };

        sprite.setPosition(100, i * 150 + 130);
        text.setPosition(200, i * 150 + 130);
        text.setCharacterSize(15);

        skillSprites.emplace_back(std::move(std::pair<sf::Sprite, sf::Text>(sprite, text)));

        auto button = std::make_shared<gui::Button>(context);
        auto buttonPointer = button.get();
        button->setPosition(600, 130 + i * 150);
        if(getContext().saveData->skillUnlocked(skill.id)) {
            button->setText("Freigeschaltet");
            button->disable(true);
        } else if(score < skill.cost) {
            button->setText("Nicht genug Punkte");
            button->disable(true);
        } else {
            button->setText("Freischalten");
            button->setCallback([this, buttonPointer, availablePointsPointer, skill] () {
                if(getContext().saveData->unlockSkill(skill.id, skill.cost)) {
                    availablePointsPointer->setText(std::to_string(getContext().saveData->getScore()));
                    buttonPointer->setText("Freigeschaltet");
                    buttonPointer->disable(true);
                }
            });
        }

        guiContainer.pack(button);
        i++;
    }

    auto backButton = std::make_shared<gui::Button>(context);
    backButton->setPosition(80.0f, 600.0f);
    backButton->setText(L"Zurück");
    backButton->setShortcut(sf::Keyboard::Escape);
    backButton->setCallback(std::bind(&StoreState::requestStackPop, this));

    guiContainer.pack(backButton);
    guiContainer.pack(availablePointsText);
    guiContainer.pack(availablePointsLabel);
}

StoreState::~StoreState() {
    getContext().saveData->save();
}

void StoreState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());
    window.draw(background);

    for(auto& skill : skillSprites) {
        window.draw(skill.first);
        window.draw(skill.second);
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
