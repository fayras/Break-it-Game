#include <iostream>
#include "World.hpp"
#include "system/Utility.hpp"

World::World(sf::RenderTarget &outputTarget, FontHolder &fonts, SoundPlayer &sounds)
  : target(outputTarget),
    sceneTexture(),
    worldView(outputTarget.getDefaultView()),
    textures(),
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
  paddle->setVelocity(paddle->getVelocity() / 2.f);
  while(!commandQueue.empty()) {
    Command command = commandQueue.pop();
    paddle->onCommand(command, dt);
    ball->onCommand(command, dt);
    for(auto& wall : walls) wall->onCommand(command, dt);
    for(auto& block : blocks) block->onCommand(command, dt);
  }

  // Collision detection and response (may destroy entities)
  handleCollisions();

  // Remove all destroyed entities, create new ones
  removeWrecks();

  paddle->update(dt, commandQueue);
  ball->update(dt, commandQueue);
  for(auto& wall : walls) wall->update(dt, commandQueue);
  for(auto& block : blocks) block->update(dt, commandQueue);

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
  for(const auto& wall : walls) target.draw(*wall);
  for(const auto& block : blocks) target.draw(*block);
  target.draw(*ball);
  target.draw(*paddle);
}

CommandQueue& World::getCommandQueue() {
  return commandQueue;
}

bool World::hasAlivePlayer() const {
  return ball->getPosition().y < worldBounds.height;
}

bool World::hasPlayerReachedEnd() const {
  return blocks.empty();
}

void World::loadTextures() {
  textures.load(Textures::PADDLE, "assets/textures/paddleBlu.png");
  textures.load(Textures::BLOCK, "assets/textures/element_grey_rectangle.png");
  textures.load(Textures::BALL, "assets/textures/ballGrey.png");
  textures.load(Textures::EXPLOSION, "assets/textures/Explosion.png");
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
  sf::Vector2f ballVel = ball->getVelocity();

  collision(ballRect, paddle->getBoundingRect(), ballVel);
  for(const auto& block : blocks) {
    sf::FloatRect blockRect = block->getBoundingRect();
    if(collision(ballRect, blockRect, ballVel)) {
      sounds.play(SoundEffect::HIT_BLOCK);
      block->destroy();
      shakeScreen = true;
    }
  }
  for(const auto& wall : walls) {
    sf::FloatRect blockRect = wall->getBoundingRect();
    if(collision(ballRect, blockRect, ballVel)) {
      // shakeScreen = true;
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
  ball = std::move(std::unique_ptr<Ball>(new Ball(textures)));
  ball->move(spawnPosition.x, spawnPosition.y - 50);
  ball->setVelocity(Random::integer(-500, 500), Random::integer(-400, -200));

  paddle = std::move(std::unique_ptr<Paddle>(new Paddle(textures)));
  paddle->move(spawnPosition);

  walls.push_back(std::move(std::unique_ptr<Wall>(new Wall{20.f, worldView.getSize().y + 40.f})));
  walls.back()->setPosition(-20.f, -20.f);
  walls.push_back(std::move(std::unique_ptr<Wall>(new Wall{worldView.getSize().x, 20.f})));
  walls.back()->setPosition(0.f, -20.f);
  walls.push_back(std::move(std::unique_ptr<Wall>(new Wall{20.f, worldView.getSize().y + 40.f})));
  walls.back()->setPosition(worldView.getSize().x, -20.f);

  for(int x = 0; x < 13; x++) {
    for(int y = 0; y < 7; y++) {
      sf::Color color;
      if(y < 7) { color.r = 255; color.g = 100; color.b = 100; }
      if(y < 4) { color.r = 150; color.g = 255; color.b = 0; }
      if(y < 2) { color.r = 100; color.g = 100; color.b = 255; }
      blocks.push_back(std::move(std::unique_ptr<Block>(new Block{textures, color})));
      blocks.back()->move(x * 70 + 80, y * 40 + 70);
    }
  }
}

sf::FloatRect World::getViewBounds() const {
  return sf::FloatRect(worldView.getCenter() - worldView.getSize() / 2.f, worldView.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const {
  // Return view bounds + some area at top, where enemies spawn
  sf::FloatRect bounds = getViewBounds();
  bounds.top -= 100.f;
  bounds.height += 100.f;

  return bounds;
}

void World::removeWrecks() {
  auto wreckfieldBegin = std::remove_if (blocks.begin(), blocks.end(), std::mem_fn(&Block::isMarkedForRemoval));
  blocks.erase(wreckfieldBegin, blocks.end());

  // Call function recursively for all remaining children
  std::for_each(blocks.begin(), blocks.end(), std::mem_fn(&SceneNode::removeWrecks));
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
