#include <iostream>
#include "World.hpp"
#include "system/Utility.hpp"
#include "tween/LinearTween.hpp"
#include "skills/DuplicateBallSkill.hpp"
#include "shaders/PostEffect.hpp"
#include "Background.hpp"
#include "skills/SlowmotionSkill.hpp"
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

World::World(sf::RenderTarget &outputTarget, FontHolder &fonts, SoundPlayer &sounds, SettingsParser& settings)
  : target(outputTarget),
    worldView(outputTarget.getDefaultView()),
    fonts(fonts),
    sounds(sounds),
    settings(settings),
    worldBounds(0.f, 0.f, worldView.getSize().x, worldView.getSize().y),
    spawnPosition(worldView.getSize().x / 2.f, worldBounds.height - 40),
    collisions(sceneGraph)
{
  sceneTexture.create(target.getSize().x, target.getSize().y);
  loadTextures();
  buildScene();
}

void World::update(sf::Time dt) {
  paddle->setVelocity(paddle->getVelocity() / 2.f);

  while(!commandQueue.empty()) {
    Command command = commandQueue.pop();
    sceneGraph.onCommand(command, dt);
  }

  if(!currentLevel->isLoading()) {
    handleCollisions(dt);
  }

  if(currentLevel->done()) {
    currentLevel->loadNext();
  }

  sceneGraph.update(dt, commandQueue);
  adaptPlayerPosition();
  updateSounds();

  sceneGraph.removeWrecks();

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
  if(PostEffect::isSupported()) {
    sceneTexture.clear();
    sceneTexture.setView(worldView);
    sceneTexture.draw(sceneGraph);
    sceneTexture.display();
    bloomEffect.apply(sceneTexture, target);
  } else {
    target.setView(worldView);
    target.draw(sceneGraph);
  }
}

CommandQueue& World::getCommandQueue() {
  return commandQueue;
}

bool World::reachedEnd() const {
  return currentLevel->done() && currentLevel->isLast();
}

void World::loadTextures() {
  textures.load(Textures::PADDLE, "assets/textures/paddle.png");
  textures.load(Textures::BLOCK, "assets/textures/enemy_base_ship.png");
  textures.load(Textures::BLOCK_SMALL, "assets/textures/enemy_art.png");
  textures.load(Textures::BALL, "assets/textures/meteor_small.png");
  textures.load(Textures::EXPLOSION, "assets/textures/explosion_2.png");
  textures.load(Textures::STARFIELD, "assets/textures/starfield.png");
  textures.load(Textures::PARTICLE, "assets/textures/particle.png");
  textures.load(Textures::LIFE, "assets/textures/life.png");
  textures.load(Textures::SKILLS, "assets/textures/skills.png");
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

void World::handleCollisions(sf::Time dt) {
  std::set<SceneNode::Pair> pairs = collisions.check(dt);

  // sceneGraph.checkSceneCollision(sceneGraph, pairs);

  for(auto collisionPair : pairs) {
    if(matchesCategories(collisionPair, Category::BALL, Category::PADDLE)) {
      auto& ball = dynamic_cast<Ball&>(*collisionPair.first);
      auto& paddle = dynamic_cast<Paddle&>(*collisionPair.second);

      sf::Vector2f ballPos = ball.getPosition();
      sf::Vector2f paddlePos = paddle.getPosition();
      sf::Vector2f delta = ballPos - paddlePos;
      //float offset = ballRect.width * 0.5f + ballRect.left - paddleRect.width * 0.5f - paddleRect.left;
      //float angle = offset / (paddleRect.width * 0.5f);
      float ballSpeed = Vector::length(ball.getVelocity());

      ball.setVelocity(Vector::unit(delta) * ballSpeed);
      sounds.play(SoundEffect::HIT_GENERAL);
      score->resetMultiplier();

      paddle.bounce(-20.f);
    } else if(matchesCategories(collisionPair, Category::BALL, Category::BLOCK)) {
      auto& ball = dynamic_cast<Ball&>(*collisionPair.first);
      auto& block = dynamic_cast<Block&>(*collisionPair.second);

      auto currentVelocity = ball.getVelocity();
      sf::Vector2f newVelocity = currentVelocity + Vector::unit(currentVelocity) * 3.0f;
      Ball::Direction dir;
      dir.deltaTime = dir.distance = 0;
      dir.dir = sf::Vector2f(
              currentVelocity.x != 0 ? newVelocity.x / currentVelocity.x : 0,
              currentVelocity.y != 0 ? newVelocity.y / currentVelocity.y : 0
      );
      ball.pushDirection(dir);

      sounds.play(SoundEffect::HIT_BLOCK);
      block.damage(100);
      shakeScreen = true;
      score->increase(10);
      score->increaseMultiplier();
    } else if(matchesCategories(collisionPair, Category::BALL, Category::WALL)) {
      auto& ball = dynamic_cast<Ball&>(*collisionPair.first);
      auto& wall = dynamic_cast<Wall&>(*collisionPair.second);

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
  auto background = std::make_unique<Background>(stars, worldView);
  sceneGraph.attachChild(std::move(background));

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

  auto level = std::make_unique<Level>(textures, fonts, worldView.getSize());
  level->setBounds(&worldBounds);
  currentLevel = level.get();
  sceneGraph.attachChild(std::move(level));

  auto score = std::make_unique<Score>(fonts.get(Fonts::PIXEL));
  score->setPosition(worldView.getSize().x - 25, -5);
  this->score = score.get();
  sceneGraph.attachChild(std::move(score));
}

sf::FloatRect World::getViewBounds() const {
  return {worldView.getCenter() - worldView.getSize() / 2.f, worldView.getSize()};
}

int World::getScore() const {
  return score->get();
}

bool World::destroyed() {
  return paddle->isDestroyed();
}

void World::setLevel(int level) {
  currentLevel->load(level);
}

void World::setScore(int score) {
  this->score->set(score);
}

int World::getCurrentLevel() const {
  return currentLevel->getID();
}

bool World::finishedLevel() const {
    return currentLevel->done();
}

void World::unlockSkill(const std::string &string) {
  if (string == Skills::ID::DUPLICATE_BALL && !sceneGraph.containsNode(Category::DUPLICATE_SKILL)) {
    auto dupSkill = std::make_unique<DuplicateBallSkill>(
            textures.get(Textures::SKILLS), fonts.get(Fonts::MAIN),
            (sf::Keyboard::Key) settings.get(Skills::ID::DUPLICATE_BALL,
            (int) sf::Keyboard::Q)
    );
    dupSkill->move(200, 10);
    sceneGraph.attachChild(std::move(dupSkill));
  } else if(string == Skills::ID::SLOWMOTION && !sceneGraph.containsNode(Category::SLOWMOTION_SKILL)) {
    auto slowSkill = std::make_unique<SlowmotionSkill>(
            textures.get(Textures::SKILLS), fonts.get(Fonts::MAIN),
            (sf::Keyboard::Key) settings.get(Skills::ID::SLOWMOTION,
            (int) sf::Keyboard::W)
    );
    slowSkill->move(260, 10);
    sceneGraph.attachChild(std::move(slowSkill));
  }
}
