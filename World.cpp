#include <iostream>
#include "World.hpp"
#include "system/Utility.hpp"
#include "DataTables.hpp"
#include <cmath>

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

  // Prepare the view
  // worldView.setCenter(spawnPosition);
}

void World::update(sf::Time dt) {
  background->move(0, 100 * dt.asSeconds());
  // keep bg repeating. 1400 = size of texture
  if(background->getPosition().y > 0) {
    background->setPosition(0, -1400 + worldView.getSize().y);
  }

  paddle->setVelocity(paddle->getVelocity() / 2.f);
  while(!commandQueue.empty()) {
    Command command = commandQueue.pop();
    paddle->onCommand(command, dt);
    ball->onCommand(command, dt);
    particles->onCommand(command, dt);
    for(auto& wall : walls) wall->onCommand(command, dt);
    // for(auto& block : blocks) block->onCommand(command, dt);
    currentLevel->onCommand(command, dt);
  }

  // Collision detection and response (may destroy entities)
  handleCollisions();

  if(currentLevel->done()) {
    loadNextLevel();
    // currentLevel->loadNext();
  }

  paddle->update(dt, commandQueue);
  ball->update(dt, commandQueue);
  particles->update(dt, commandQueue);
  currentLevel->update(dt, commandQueue);
  for(auto& wall : walls) wall->update(dt, commandQueue);
  // for(auto& block : blocks) block->update(dt, commandQueue);

  adaptPlayerPosition();
  updateSounds();

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
  target.draw(*background);
  for(const auto& wall : walls) target.draw(*wall);
  // for(const auto& block : blocks) target.draw(*block);
  target.draw(*currentLevel);
  target.draw(*particles);
  target.draw(*ball);
  target.draw(*paddle);
  target.draw(*score);
}

CommandQueue& World::getCommandQueue() {
  return commandQueue;
}

bool World::hasAlivePlayer() const {
  return ball->getPosition().y < worldBounds.height;
}

bool World::hasPlayerReachedEnd() const {
  return currentLevel->done() && currentLevel->isLast();
}

void World::loadTextures() {
  textures.load(Textures::PADDLE, "assets/textures/paddleBlu.png");
  textures.load(Textures::BLOCK, "assets/textures/element_grey_rectangle.png");
  textures.load(Textures::BALL, "assets/textures/ballGrey.png");
  textures.load(Textures::EXPLOSION, "assets/textures/Explosion.png");
  textures.load(Textures::STARFIELD, "assets/textures/starfield.png");
  textures.load(Textures::SCORE, "assets/textures/glassPanel_cornerBL.png");
  textures.load(Textures::PARTICLE, "assets/textures/particle.png");
}

void World::adaptPlayerPosition() {
  // Keep player's position inside the screen bounds, at least borderDistance units from the border
  sf::FloatRect viewBounds = getViewBounds();
  const float borderDistance = 52.f;

  sf::Vector2f position = paddle->getPosition();
  position.x = std::max(position.x, viewBounds.left + borderDistance);
  position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
  position.y = std::max(position.y, viewBounds.top + borderDistance);
  position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
  paddle->setPosition(position);
}

void World::handleCollisions() {
  sf::FloatRect ballRect = ball->getBoundingRect();
  sf::FloatRect paddleRect = paddle->getBoundingRect();
  sf::Vector2f ballVel = ball->getVelocity();

  if(collision(ballRect, paddleRect, ballVel)) {
    float offset = ballRect.width * 0.5f + ballRect.left - paddleRect.width * 0.5f - paddleRect.left;
    float angle = offset / (paddleRect.width * 0.5f);
    float ballSpeed = Vector::length(ball->getVelocity());
    sf::Vector2f newVel(angle, -0.5f);
    ball->setVelocity(Vector::unit(newVel) * ballSpeed);
    sounds.play(SoundEffect::HIT_GENERAL);
    score->resetMultiplier();
  }

  for(auto block : currentLevel->getBlocks()) {
    sf::FloatRect blockRect = block->getBoundingRect();
    if(collision(ballRect, blockRect, ballVel)) {
      ball->setVelocity(ball->getVelocity() + Vector::unit(ball->getVelocity()) * 3.0f);
      sounds.play(SoundEffect::HIT_BLOCK);
      block->damage(100);
      shakeScreen = true;
      score->increase(10);
      score->increaseMultiplier();
    }
  }

  for(const auto& wall : walls) {
    sf::FloatRect blockRect = wall->getBoundingRect();
    if(collision(ballRect, blockRect, ballVel)) {
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
  score = std::move(std::make_unique<Score>(textures.get(Textures::SCORE), fonts.get(Fonts::ARCADE)));
  score->setPosition(worldView.getSize().x - 295, 10);

  sf::Texture& stars = textures.get(Textures::STARFIELD);
  stars.setRepeated(true);
  background = std::move(std::make_unique<SpriteNode>(stars));

  ball = std::move(std::make_unique<Ball>(textures));

  paddle = std::move(std::make_unique<Paddle>(textures));

  float wallWidth = 80.f;
  float halfWallWidth = wallWidth / 2.f;

  walls.push_back(std::move(std::make_unique<Wall>(halfWallWidth, worldView.getSize().y + wallWidth)));
  walls.back()->setPosition(-halfWallWidth, -halfWallWidth);
  walls.push_back(std::move(std::make_unique<Wall>(worldView.getSize().x + wallWidth, halfWallWidth)));
  walls.back()->setPosition(-halfWallWidth, -halfWallWidth);
  walls.push_back(std::move(std::make_unique<Wall>(halfWallWidth, worldView.getSize().y + wallWidth)));
  walls.back()->setPosition(worldView.getSize().x, -halfWallWidth);

  particles = std::move(std::make_unique<ParticleNode>(Particle::Propellant, textures));
  currentLevel = std::move(std::make_unique<Level>(textures));

  loadNextLevel();
}

void World::loadNextLevel() {
  currentLevel->loadNext();

  paddle->setPosition(spawnPosition);
  ball->setPosition(spawnPosition.x, spawnPosition.y - 50);
  ball->setVelocity(0, -300 * currentLevel->getBallSpeedMultiplier());
  particles->clearParticles();
  showNewLevelMessage = true;
}

sf::FloatRect World::getViewBounds() const {
  return {worldView.getCenter() - worldView.getSize() / 2.f, worldView.getSize()};
}

bool World::collision(sf::FloatRect ballRect, sf::FloatRect blockRect, sf::Vector2f& ballVelocity) const {
  if(!ballRect.intersects(blockRect)) {
    return false;
  }

  float overlapLeft = ballRect.left + ballRect.width - blockRect.left;
  float overlapRight = blockRect.left + blockRect.width - ballRect.left;
  float overlapTop = ballRect.top + ballRect.height - blockRect.top;
  float overlapBottom = blockRect.top + blockRect.height - ballRect.top;

  bool ballFromLeft = std::abs(overlapLeft) < std::abs(overlapRight);
  bool ballFromTop = std::abs(overlapTop) < std::abs(overlapBottom);

  float minOverlapX = ballFromLeft ? overlapLeft : overlapRight;
  float minOverlapY = ballFromTop ? overlapTop : overlapBottom;

  if(std::abs(minOverlapX) < std::abs(minOverlapY)) {
    ball->setVelocity(ballVelocity.x * -1.f, ballVelocity.y);
  } else {
    ball->setVelocity(ballVelocity.x, ballVelocity.y * -1.f);
  }

  return true;
}

int World::getScore() const {
  return score->get();
}

int World::getLevel() const {
  return currentLevel->getID();
}
