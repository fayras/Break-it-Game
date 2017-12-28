#include "Level.hpp"
#include "nodes/ParticleNode.hpp"
#include "system/Utility.hpp"
#include "tween/EaseOutElastic.hpp"
#include "Ball.hpp"
#include "Paddle.hpp"

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
    sf::Vector2f from{block->getPosition()};
    sf::Vector2f to{pair.first.x * 70.0f + 80.0f, pair.first.y * 40.0f + 110.0f};
    Block* bPointer = block.get();
    auto tween = std::make_unique<EaseOutElastic>(sf::milliseconds(1200), [bPointer, from, to](const float& t) {
      sf::Vector2f diff = to - from;
      bPointer->setPosition(diff * t + from);
    });
    tween->delay(sf::milliseconds(Random::integer(70)));
    block->tween(std::move(tween));
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
    command.category = Category::PADDLE;
    command.action = derivedAction<Paddle>([this](Paddle& node, sf::Time) {
      sf::Vector2f spawnPosition{levelData.spawnPosition.x * bounds->width, levelData.spawnPosition.y * bounds->height};
      node.setPosition(spawnPosition);
    });
    commands.push(command);

    Command command1;
    command1.category = Category::BALL;
    command1.action = derivedAction<Ball>([this](Ball& ball, sf::Time) {
      sf::Vector2f spawnPosition{levelData.spawnPosition.x * bounds->width, levelData.spawnPosition.y * bounds->height};
      ball.reset(spawnPosition);
      ball.setVelocity(0, -Ball::SPEED * getBallSpeedMultiplier());
    });
    commands.push(command1);

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
