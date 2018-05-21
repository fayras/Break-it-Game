#include <fstream>
#include "Level.hpp"
#include "nodes/ParticleNode.hpp"
#include "system/Utility.hpp"
#include "tween/EaseOutElastic.hpp"
#include "entities/Ball.hpp"
#include "entities/Paddle.hpp"
#include "LevelInfo.hpp"
#include "system/ResourceHolder.hpp"
#include "tween/LinearTween.hpp"
#include "Score.hpp"
#include "Life.hpp"
#include "system/json.hpp"

using json = nlohmann::json;

namespace {
  const std::vector<LevelData> LevelTable = initializeLevelData();
}

Level::Level(const TextureHolder &textures, const FontHolder& fonts, const sf::Vector2f view)
  : textures(textures), fonts(fonts), levelData(), bounds(nullptr)
{
  auto blocks = std::make_unique<SceneNode>();
  blocksLayer = blocks.get();
  attachChild(std::move(blocks));

  auto balls = std::make_unique<SceneNode>();
  ballsLayer = balls.get();
  attachChild(std::move(balls));

  auto levelInfo = std::make_unique<LevelInfo>(fonts.get(Fonts::PIXEL), view);
  attachChild(std::move(levelInfo));
}

bool Level::done() const {
  return !blocksLayer->hasChildren();
}

bool Level::ballLost() const {
  return !ballsLayer->hasChildren();
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
  objectsNeedUpdate = true;
  blocksLayer->clearChildren();
  std::string file = LevelTable[getID()].filename;
  levelData = LevelTable[getID()];
  // std::size_t columns = levelData.blockColors.size();

  std::ifstream inputData(file);
  json levelJson;
  inputData >> levelJson;

  auto map = levelJson["map"];
  levelData.spawnPosition = sf::Vector2f(
          map["spawnX"].get<float>() / map["worldWidth"].get<float>(),
          map["spawnY"].get<float>() / map["worldHeight"].get<float>()
  );

  auto objects = levelJson["objects"];
  auto blocks = objects["contents"];
  for(auto const &blockJson : blocks) {
    auto blockType = static_cast<Blocks::Type>(blockJson["blockType"].get<int>());

    std::unique_ptr<Block> block(new Block(textures, blockType));
    std::string colorHexString = blockJson["color"];
    unsigned int colorHex = std::stoul("0x" + colorHexString + "ff", nullptr, 16);
    block->setColor(sf::Color(colorHex));
    sf::FloatRect blockRect = block->getBoundingRect();
    float margin = 5;
    blockRect.height += margin;
    blockRect.width += margin;

    block->setPosition(blockJson["x"], -40);

    sf::Vector2f from{block->getPosition()};
    sf::Vector2f to{blockJson["x"], blockJson["y"]};
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
    blocksLayer->attachChildNow(std::move(block));
  }

  ballsLayer->clearChildren();
  auto ball = std::make_unique<Ball>(textures.get(Textures::BALL));
  ballsLayer->attachChildNow(std::move(ball));
}

float Level::getBallSpeedMultiplier() const {
  return 1.0f; //levelData.ballSpeedMultiplier;
}

bool Level::isLast() const {
  return currentID == -2;
}

void Level::updateCurrent(sf::Time dt, CommandQueue &commands) {
  if(ballLost()) {
    objectsNeedUpdate = true;
    auto ball = std::make_unique<Ball>(textures.get(Textures::BALL));
    ballsLayer->attachChildNow(std::move(ball));
    Command command1;
    command1.category = Category::PADDLE;
    command1.action = derivedAction<Paddle>([this](Paddle& paddle, sf::Time) {
      paddle.damage(1);
    });
    Command command2;
    command2.category = Category::LIFE;
    command2.action = derivedAction<Life>([](Life& life, sf::Time) {
      life.decrease();
    });
    Command command3;
    command3.category = Category::SCORE;
    command3.action = derivedAction<Score>([](Score& score, sf::Time) {
      score.resetMultiplier();
      score.increase(-score.get() / 2);
    });
    commands.push(command1);
    commands.push(command2);
    commands.push(command3);
  }

  if(objectsNeedUpdate) {
    resetObjects(commands);
    objectsNeedUpdate = false;
  }
}

void Level::setBounds(sf::FloatRect* bounds) {
  this->bounds = bounds;
}

bool Level::isLoading() const {
  return loading;
}

void Level::resetObjects(CommandQueue &commands) {
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
    command.category = Category::BALL;
    command.action = derivedAction<Ball>([](Ball& entity, sf::Time) {
      entity.recieveEvents = true;
    });

    Command commandPaddle;
      commandPaddle.category = Category::PADDLE;
      commandPaddle.action = derivedAction<Paddle>([](Paddle& entity, sf::Time) {
      entity.recieveEvents = true;
    });

    commands.push(command);
    commands.push(commandPaddle);
  });
  tween(std::move(t));
}

void Level::load(int level) {
  currentID = level;
  load();
}
