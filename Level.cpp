#include "Level.hpp"
#include "nodes/ParticleNode.hpp"
#include "system/Utility.hpp"

namespace {
  const std::vector<LevelData> LevelTable = initializeLevelData();
}

Level::Level(const TextureHolder &textures)
  : textures(textures), levelData(), bounds(nullptr)
{
  loadNext();
}

bool Level::done() const {
  return children.empty();
}

int Level::getID() const {
  return currentID;
}

void Level::loadNext() {
  if(getID() + 1 >= LevelTable.size()) {
    currentID = -2;
    return;
  }

  currentID++;
  load();
}

void Level::load() {
  levelData = LevelTable[getID()];
  for(auto const &pair : levelData.blockColors) {
    std::unique_ptr<Block> block(new Block(textures, pair.second));
    block->setPosition(pair.first.x * 70 + 80, -40);
    block->tweenPositionTo(sf::Vector2f(pair.first.x * 70 + 80, pair.first.y * 40 + 110));
    block->tweenDelay(sf::milliseconds(Random::integer(100)));
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

  if(done()) {
    loadNext();
    Command command;
    command.category = Category::PADDLE | Category::BALL;
    command.action = derivedAction<Entity>([this](Entity& node, sf::Time) {
      sf::Vector2f spawnPosition{levelData.spawnPosition.x * bounds->width, levelData.spawnPosition.y * bounds->height};
      if(node.getCategory() == Category::PADDLE) {
        node.setPosition(spawnPosition);
      }

      if(node.getCategory() == Category::BALL) {
        node.setPosition(spawnPosition.x, spawnPosition.y - 50);
        node.setVelocity(0, -300 * getBallSpeedMultiplier());
      }
    });
    commands.push(command);

    Command command2;
    command2.category = Category::PARTICLE_SYSTEM;
    command2.action = derivedAction<ParticleNode>([](ParticleNode& particles, sf::Time) {
      particles.clearParticles();
    });
    commands.push(command2);
  }
}

std::vector<Block*> Level::getBlocks() const {
  std::vector<Block*> blocks;

  for (auto& item : children) {
    auto t = dynamic_cast<Block*>(item.get());
    blocks.push_back(t);
  }

  return blocks;
}

void Level::setBounds(sf::FloatRect* bounds) {
  this->bounds = bounds;
}
