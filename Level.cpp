#include "Level.hpp"
#include "nodes/ParticleNode.hpp"
#include "system/Utility.hpp"
#include "tween/EaseOutElastic.hpp"
#include "entities/Ball.hpp"
#include "entities/Paddle.hpp"
#include "LevelInfo.hpp"
#include "system/ResourceHolder.hpp"
#include "tween/LinearTween.hpp"

namespace {
  const std::vector<LevelData> LevelTable = initializeLevelData();
}

Level::Level(const TextureHolder &textures, const FontHolder& fonts)
  : textures(textures), fonts(fonts), levelData(), bounds(nullptr)
{
  auto blocks = std::make_unique<SceneNode>();
  blocksLayer = blocks.get();
  attachChild(std::move(blocks));

  auto levelInfo = std::make_unique<LevelInfo>(fonts.get(Fonts::PIXEL));
  attachChild(std::move(levelInfo));
}

bool Level::done() const {
  return !blocksLayer->hasChildren();
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
  loading = true;
  levelData = LevelTable[getID()];
  // std::size_t columns = levelData.blockColors.size();

  for(auto const &pair : levelData.blockColors) {
    std::unique_ptr<Block> block(new Block(textures, levelData.blockType));
    block->setColor(pair.second);
    sf::FloatRect blockRect = block->getBoundingRect();
    float margin = 5;
    blockRect.height += margin;
    blockRect.width += margin;

    block->setPosition(pair.first.x * blockRect.width + 80, -40);

    sf::Vector2f from{block->getPosition()};
    sf::Vector2f to{pair.first.x * blockRect.width + 80.0f, pair.first.y * blockRect.height + 110.0f};
    Block* bPointer = block.get();

    auto tween = std::make_unique<EaseOutElastic>(sf::milliseconds(1200), [bPointer, from, to](const float& t) {
      sf::Vector2f diff = to - from;
      bPointer->setPosition(diff * t + from);
    });
    tween->delay(sf::milliseconds(350 + Random::integer(70)));
    tween->attachObserver([this]() {
      loading = false;
    });
    block->tween(std::move(tween));
    blocksLayer->attachChild(std::move(block));
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
      node.recieveEvents = false;
    });
    commands.push(command);

    Command command1;
    command1.category = Category::BALL;
    command1.action = derivedAction<Ball>([this](Ball& ball, sf::Time) {
      sf::Vector2f spawnPosition{levelData.spawnPosition.x * bounds->width, levelData.spawnPosition.y * bounds->height};
      ball.reset(spawnPosition);
      ball.setVelocity(0, -Ball::SPEED * getBallSpeedMultiplier());
      ball.recieveEvents = false;
    });
    commands.push(command1);

    Command command2;
    command2.category = Category::PARTICLE_SYSTEM;
    command2.action = derivedAction<ParticleNode>([](ParticleNode& particles, sf::Time) {
      particles.clearParticles();
    });
    commands.push(command2);

    Command command3;
    command3.category = Category::LEVEL_INFO;
    command3.action = derivedAction<LevelInfo>([this](LevelInfo& info, sf::Time) {
      info.show(getID() + 1, sf::milliseconds(1300));
    });
    commands.push(command3);

    auto t = std::make_unique<LinearTween>(sf::milliseconds(2900), [](const float& t) {});
    t->attachObserver([&commands]() {
      Command command;
      command.category = Category::BALL | Category::PADDLE;
      command.action = derivedAction<Entity>([](Entity& entity, sf::Time) {
        entity.recieveEvents = true;
      });
      commands.push(command);
    });
    tween(std::move(t));
  }
}

void Level::setBounds(sf::FloatRect* bounds) {
  this->bounds = bounds;
}

bool Level::isLoading() const {
  return loading;
}
