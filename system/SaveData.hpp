//
// Created by Dimi on 20.05.2018.
//

#ifndef BREAK_IT_SAVEDATA_HPP
#define BREAK_IT_SAVEDATA_HPP


#include <SFML/System/NonCopyable.hpp>
#include "SettingsParser.hpp"

class SaveData : private sf::NonCopyable {
    public:
        SaveData();

        void loadFromFile(std::string file);
        bool exists() const;

        int getLevel() const;
        void setLevel(int level);

        int getScore() const;
        void setScore(int score);

        bool skillUnlocked(const std::string& string) const;
        bool unlockSkill(const std::string& string, int cost);

        void save();
    private:
        SettingsParser data;
        std::vector<std::string> skillsUnlocked;

};


#endif //BREAK_IT_SAVEDATA_HPP
