//
// Created by Dimi on 20.05.2018.
//

#include "SaveData.hpp"
#include <algorithm>

SaveData::SaveData()
    : skillsUnlocked()
{}

void SaveData::loadFromFile(std::string file) {
    if(data.loadFromFile(file)) {
        data.get("skillsUnlocked", skillsUnlocked);
    }
}

bool SaveData::exists() const {
    return data.exists();
}

int SaveData::getLevel() const {
    return data.get("player_level", 0);
}

void SaveData::setLevel(int level) {
    data.set("player_level", level);
}

int SaveData::getScore() const {
    return data.get("player_score", 0);
}

void SaveData::setScore(int score) {
    data.set("player_score", score);
}

void SaveData::save() {
    if(!skillsUnlocked.empty()) {
        data.set("skillsUnlocked", skillsUnlocked);
    }
    data.saveToFile();
}

bool SaveData::skillUnlocked(const std::string &string) const {
    return std::find(skillsUnlocked.begin(), skillsUnlocked.end(), string) != skillsUnlocked.end();
}

bool SaveData::unlockSkill(const std::string &string, int cost) {
    auto score = getScore();
    if(!skillUnlocked(string) && score >= cost) {
        skillsUnlocked.push_back(string);
        setScore(score - cost);
        return true;
    }

    return false;
}
