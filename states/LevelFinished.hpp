//
// Created by Dimi on 20.05.2018.
//

#ifndef BREAK_IT_LEVELFINISHED_HPP
#define BREAK_IT_LEVELFINISHED_HPP

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "../system/StateStack.hpp"
#include "../gui/Container.hpp"

class LevelFinished : public State {
    public:
        LevelFinished(StateStack& stack, Context context);

        void draw() override;
        bool update(sf::Time dt) override;
        bool handleEvent(const sf::Event& event) override;

    private:
        sf::Text text;
        gui::Container guiContainer;
        sf::RectangleShape backgroundShape;
};


#endif //BREAK_IT_LEVELFINISHED_HPP
