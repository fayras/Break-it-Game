#include <iostream>
#include "World.hpp"
#include "system/Utility.hpp"
#include "LevelInfo.hpp"
#include "tween/LinearTween.hpp"
#include <cmath>
#include <algorithm>

bool matchesCategories(SceneNode::Pair& colliders, Category::Type typeA, Category::Type typeB) {
  unsigned int categoryA = colliders.first->getCategory();
  unsigned int categoryB = colliders.second->getCategory();

  if(typeA & categoryA && typeB & categoryB) {
    return true;
  } else if(typeA & categoryB && typeB & categoryA) {
    std::swap(colliders.first, colliders.second);
    return true;
  } else {
    return false;
  }
}

World::World(sf::RenderTarget &outputTarget, FontHolder &fonts, SoundPlayer &sounds)
  : target(outputTarget),
    worldView(outputTarget.getDefaultView()),
    fonts(fonts),
    sounds(sounds),
    worldBounds(0.f, 0.f, worldView.getSize().x, worldView.getSize().y),
    spawnPosition(worldView.getSize().x / 2.f, worldBounds.height - 40)
{
  sceneTexture.create(target.getSize().x, target.getSize().y);
  loadTextures();
  buildScene();
  resetPositions();
}

void World::update(sf::Time dt) {
  paddle->setVelocity(paddle->getVelocity() / 2.f);

  while(!commandQueue.empty()) {
    Command command = commandQueue.pop();
    sceneGraph.onCommand(command, dt);
  }

  if(!currentLevel->isLoading()) {
    handleCollisions();
  }
  sceneGraph.update(dt, commandQueue);
  adaptPlayerPosition();
  updateSounds();

  Command bgCommand;
  bgCommand.category = Category::BACKGROUND;
  bgCommand.action = [this](SceneNode& background, sf::Time dt) {
    background.move(0, 100 * dt.asSeconds());
    // keep bg repeating. 1400 = size of texture
    if(background.getPosition().y > 0) {
      background.setPosition(0, -1400 + worldView.getSize().y);
    }
  };
  commandQueue.push(bgCommand);

  sceneGraph.removeWrecks();

  if(!sceneGraph.containsNode(Category::BALL)) {
    auto ball = std::make_unique<Ball>(textures);
    sceneGraph.attachChild(std::move(ball));

    Command command0;
    command0.category = Category::LEVEL_INFO;
    command0.action = derivedAction<LevelInfo>([this](LevelInfo& info, sf::Time) {
      info.show(currentLevel->getID() + 1);
    });
    Command command1;
    command1.category = Category::PADDLE;
    command1.action = derivedAction<Paddle>([this](Paddle& paddle, sf::Time) {
      paddle.damage(1);
      if(!paddle.isDestroyed()) resetPositions();
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
    commandQueue.push(command0);
    commandQueue.push(command1);
    commandQueue.push(command2);
    commandQueue.push(command3);
  }

  if(shakeScreen) {
    shakeTimer += dt;
    if(shakeTimer.asMilliseconds() > 200) {
      shakeScreen = false;
      shakeTimer = sf::Time::Zero;
      worldView.setCenter(worldBounds.width * 0.5f, worldBounds.height * 0.5f);
    } else {
      if(shakeDirection) {
        shakeOffsetX -= 1.f * dt.asMilliseconds();
        if(shakeOffsetX < -10) {
          shakeOffsetX = -10;
          shakeDirection = !shakeDirection;
        }
      } else {
        shakeOffsetX += 1.f * dt.asMilliseconds();
        if(shakeOffsetX > 10) {
          shakeOffsetX = 10;
          shakeDirection = !shakeDirection;
        }
      }
      shakeOffsetY = shakeOffsetX;
      worldView.setCenter(worldBounds.width * 0.5f + shakeOffsetX, worldBounds.height * 0.5f + shakeOffsetY);
    }
  }
  target.setView(worldView);
}

void World::draw() {
  target.setView(worldView);
  target.draw(sceneGraph);
}

CommandQueue& World::getCommandQueue() {
  return commandQueue;
}

bool World::reachedEnd() const {
  return currentLevel->done() && currentLevel->isLast();
}

void World::loadTextures() {
  textures.load(Textures::PADDLE, "assets/textures/paddleBlu.png");
  textures.load(Textures::BLOCK, "assets/textures/block.png");
  textures.load(Textures::BLOCK_SMALL, "assets/textures/block_small.png");
  textures.load(Textures::BALL, "assets/textures/ballGrey.png");
  textures.load(Textures::EXPLOSION, "assets/textures/Explosion.png");
  textures.load(Textures::STARFIELD, "assets/textures/starfield.png");
  textures.load(Textures::SCORE, "assets/textures/glassPanel_cornerBL.png");
  textures.load(Textures::PARTICLE, "assets/textures/particle.png");
  textures.load(Textures::LIFE, "assets/textures/life.png");
  textures.get(Textures::LIFE).setRepeated(true);
}

void World::adaptPlayerPosition() {
  // Keep player's position inside the screen bounds, at least borderDistance units from the border
  const float borderDistance = 52.f;

  sf::Vector2f position = paddle->getPosition();
  position.x = std::max(position.x, worldBounds.left + borderDistance);
  position.x = std::min(position.x, worldBounds.left + worldBounds.width - borderDistance);
  position.y = std::max(position.y, worldBounds.top);
  position.y = std::min(position.y, worldBounds.top + worldBounds.height);
  paddle->setPosition(position);
}

void World::handleCollisions() {
  std::map<SceneNode::Pair, CollisionSide> pairs;
  sceneGraph.checkSceneCollision(sceneGraph, pairs);

  for(auto collision : pairs) {
    SceneNode::Pair collisionPair = collision.first;
    if(matchesCategories(collisionPair, Category::BALL, Category::PADDLE)) {
      auto& ball = dynamic_cast<Ball&>(*collisionPair.first);
      auto& paddle = dynamic_cast<Paddle&>(*collisionPair.second);

      sf::FloatRect ballRect = ball.getBoundingRect();
      sf::FloatRect paddleRect = paddle.getBoundingRect();

      float offset = ballRect.width * 0.5f + ballRect.left - paddleRect.width * 0.5f - paddleRect.left;
      float angle = offset / (paddleRect.width * 0.5f);
      float ballSpeed = Vector::length(ball.getVelocity());
      sf::Vector2f newVel(angle, -0.5f);
      ball.setVelocity(Vector::unit(newVel) * ballSpeed);
      sounds.play(SoundEffect::HIT_GENERAL);
      score->resetMultiplier();

      float oldY = paddle.getPosition().y;
      paddle.tween(std::make_unique<LinearTween>(sf::milliseconds(100), [&paddle, oldY](const float& t) {
        float y = oldY + 20.0f * (0.5f - std::abs(0.5f - t));
        paddle.setPosition(paddle.getPosition().x, y);
      }));
    } else if(matchesCategories(collisionPair, Category::BALL, Category::BLOCK)) {
      auto& ball = dynamic_cast<Ball&>(*collisionPair.first);
      auto& block = dynamic_cast<Block&>(*collisionPair.second);

      if(collision.second == CollisionSide::LEFT || collision.second == CollisionSide::RIGHT) {
        ball.setVelocity(-ball.getVelocity().x, ball.getVelocity().y);
      } else {
        ball.setVelocity(ball.getVelocity().x, -ball.getVelocity().y);
      }

      ball.setVelocity(ball.getVelocity() + Vector::unit(ball.getVelocity()) * 3.0f);
      sounds.play(SoundEffect::HIT_BLOCK);
      block.damage(100);
      shakeScreen = true;
      score->increase(10);
      score->increaseMultiplier();
    } else if(matchesCategories(collisionPair, Category::BALL, Category::WALL)) {
      auto& ball = dynamic_cast<Ball&>(*collisionPair.first);
      auto& wall = dynamic_cast<Wall&>(*collisionPair.second);

      if(collision.second == CollisionSide::LEFT || collision.second == CollisionSide::RIGHT) {
        ball.setVelocity(-ball.getVelocity().x, ball.getVelocity().y);
      } else {
        ball.setVelocity(ball.getVelocity().x, -ball.getVelocity().y);
      }

      if(wall.isDeadly()) {
        ball.destroy();
      }

      sounds.play(SoundEffect::HIT_GENERAL);
    }
  }
}

void World::updateSounds() {
  // Set listener's position to player position
  sounds.setListenerPosition(paddle->getWorldPosition());

  // Remove unused sounds
  sounds.removeStoppedSounds();
}

void World::buildScene() {
  sf::Texture& stars = textures.get(Textures::STARFIELD);
  stars.setRepeated(true);
  auto background = std::make_unique<SpriteNode>(stars);
  background->setCategory(Category::BACKGROUND);
  sceneGraph.attachChild(std::move(background));

  auto ball = std::make_unique<Ball>(textures);
  sceneGraph.attachChild(std::move(ball));

  auto paddle = std::make_unique<Paddle>(textures);
  this->paddle = paddle.get();
  sceneGraph.attachChild(std::move(paddle));

  float wallWidth = 80.f;
  float halfWallWidth = wallWidth / 2.f;

  auto walls = std::make_unique<SceneNode>();

  auto wall1 = std::make_unique<Wall>(halfWallWidth, worldView.getSize().y + wallWidth);
  wall1->setPosition(-halfWallWidth, -halfWallWidth);
  walls->attachChild(std::move(wall1));

  auto wall2 = std::make_unique<Wall>(worldView.getSize().x + wallWidth, halfWallWidth);
  wall2->setPosition(-halfWallWidth, -halfWallWidth);
  walls->attachChild(std::move(wall2));

  auto wall3 = std::make_unique<Wall>(halfWallWidth, worldView.getSize().y + wallWidth);
  wall3->setPosition(worldView.getSize().x, -halfWallWidth);
  walls->attachChild(std::move(wall3));

  auto wall4 = std::make_unique<Wall>(worldView.getSize().x + wallWidth, halfWallWidth);
  wall4->setPosition(-halfWallWidth, worldView.getSize().y + wallWidth);
  wall4->setDeadly(true);
  walls->attachChild(std::move(wall4));

  sceneGraph.attachChild(std::move(walls));

  auto particles = std::make_unique<ParticleNode>(Particle::Propellant, textures);
  sceneGraph.attachChild(std::move(particles));

  auto level = std::make_unique<Level>(textures, fonts);
  level->setBounds(&worldBounds);
  currentLevel = level.get();
  sceneGraph.attachChild(std::move(level));

  auto score = std::make_unique<Score>(fonts.get(Fonts::PIXEL));
  score->setPosition(worldView.getSize().x - 25, -5);
  this->score = score.get();
  sceneGraph.attachChild(std::move(score));

  Command command;
  command.category = Category::LEVEL_INFO;
  command.action = derivedAction<LevelInfo>([this](LevelInfo& info, sf::Time) {
    info.setView(worldView.getSize());
  });
  commandQueue.push(command);
}

sf::FloatRect World::getViewBounds() const {
  return {worldView.getCenter() - worldView.getSize() / 2.f, worldView.getSize()};
}

int World::getScore() const {
  return score->get();
}

void World::resetPositions() {
  Command command1;
  command1.category = Category::PADDLE;
  command1.action = derivedAction<Paddle>([this](Paddle& paddle, sf::Time) {
    paddle.setPosition(spawnPosition);
  });
  Command command2;
  command2.category = Category::BALL;
  command2.action = derivedAction<Ball>([this](Ball& ball, sf::Time) {
    ball.reset(spawnPosition);
    ball.setVelocity(0, -Ball::SPEED * currentLevel->getBallSpeedMultiplier());
  });
  Command command3;
  command3.category = Category::PARTICLE_SYSTEM;
  command3.action = derivedAction<ParticleNode>([](ParticleNode& particles, sf::Time) {
    particles.clearParticles();
  });
  commandQueue.push(command1);
  commandQueue.push(command2);
  commandQueue.push(command3);
}

bool World::destroyed() {
  return paddle->isDestroyed();
}
