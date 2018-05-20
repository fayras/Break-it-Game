//
// Created by Dimi on 09.05.2018.
//

#ifndef BREAK_IT_STORESTATE_HPP
#define BREAK_IT_STORESTATE_HPP


#include <SFML/Graphics/Sprite.hpp>
#include <unordered_map>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "../system/State.hpp"
#include "../gui/Container.hpp"

class StoreState : public State {
    public:
        StoreState(StateStack& stack, Context context);
        ~StoreState();

        void draw() override;
        bool update(sf::Time dt) override;
        bool handleEvent(const sf::Event& event) override;

    private:
        std::unordered_map<std::string, std::pair<sf::Sprite, sf::Text>> skillSprites;
        std::vector<std::string> skillsUnlocked;
        gui::Container guiContainer;
        sf::RectangleShape background;
};


#endif //BREAK_IT_STORESTATE_HPP
