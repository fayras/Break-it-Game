#include "Level.hpp"

namespace {
  const std::vector<LevelData> LevelTable = initializeLevelData();
}

Level::Level(const TextureHolder &textures)
  : textures(textures), levelData()
{}

bool Level::done() const {
  return children.empty();
}

int Level::getID() const {
  return currentID;
}

void Level::loadNext() {
  if(getID() + 1 >= LevelTable.size()) {
    currentID = -2;
  }
  currentID++;
  load();
}

void Level::load() {
  levelData = LevelTable[getID()];
  for(auto const &pair : levelData.blockColors) {
    std::unique_ptr<Block> block(new Block(textures, pair.second));
    block->move(pair.first.x * 70 + 80, pair.first.y * 40 + 110);
    attachChild(std::move(block));
  }
}

float Level::getBallSpeedMultiplier() const {
  return levelData.ballSpeedMultiplier;
}

bool Level::isLast() const {
  return currentID == -2;
}

void Level::updateCurrent(sf::Time dt, CommandQueue &commands) {
  removeWrecks();
}
