#include "World.hpp"
#include "Paddle.hpp"
#include "Block.hpp"
#include "Ball.hpp"

World::World(sf::RenderTarget &outputTarget, FontHolder &fonts, SoundPlayer &sounds)
  : target(outputTarget),
    sceneTexture(),
    worldView(outputTarget.getDefaultView()),
    textures(),
    fonts(fonts),
    sounds(sounds),
    sceneGraph(),
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
    sceneGraph.onCommand(commandQueue.pop(), dt);
  }

  // Collision detection and response (may destroy entities)
  handleCollisions();

  // Remove all destroyed entities, create new ones
  sceneGraph.removeWrecks();

  sceneGraph.update(dt, commandQueue);
  adaptPlayerPosition();
  updateSounds();
}

void World::draw() {
  target.setView(worldView);
  target.draw(sceneGraph);
}

CommandQueue& World::getCommandQueue() {
  return commandQueue;
}

bool World::hasAlivePlayer() const {
  return true;
}

bool World::hasPlayerReachedEnd() const {
  return false;
}

void World::loadTextures() {
  textures.load(Textures::PADDLE, "assets/textures/paddleBlu.png");
  textures.load(Textures::BLOCK, "assets/textures/element_grey_rectangle.png");
  textures.load(Textures::BALL, "assets/textures/ballGrey.png");
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

bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2) {
  unsigned int category1 = colliders.first->getCategory();
  unsigned int category2 = colliders.second->getCategory();

  // Make sure first pair entry has category type1 and second has type2
  if (type1 & category1 && type2 & category2) {
    return true;
  } else if (type1 & category2 && type2 & category1) {
    std::swap(colliders.first, colliders.second);
    return true;
  } else {
    return false;
  }
}

void World::handleCollisions() {
  std::set<SceneNode::Pair> collisionPairs;
  sceneGraph.checkSceneCollision(sceneGraph, collisionPairs);

  for(SceneNode::Pair pair : collisionPairs) {

  }
}

void World::updateSounds() {
  // Set listener's position to player position
  sounds.setListenerPosition(paddle->getWorldPosition());

  // Remove unused sounds
  sounds.removeStoppedSounds();
}

void World::buildScene() {
  std::unique_ptr<Paddle> pd(new Paddle(textures));
  paddle = pd.get();
  pd->setPosition(spawnPosition);
  sceneGraph.attachChild(std::move(pd));

  std::unique_ptr<Ball> ball(new Ball(textures));
  ball->setPosition(spawnPosition.x, spawnPosition.y - 50);
  sceneGraph.attachChild(std::move(ball));

  SceneNode::Ptr blockContainer(new SceneNode());
  blockContainer->setPosition(80, 70);
  for(int x = 0; x < 13; x++) {
    for(int y = 0; y < 7; y++) {
      sf::Color color;
      if(y < 7) { color.r = 255; color.g = 100; color.b = 100; }
      if(y < 4) { color.r = 150; color.g = 255; color.b = 0; }
      if(y < 2) { color.r = 100; color.g = 100; color.b = 255; }
      std::unique_ptr<Block> block(new Block(textures, color));
      block->move(x * 70, y * 40);
      blockContainer->attachChild(std::move(block));
    }
  }

  sceneGraph.attachChild(std::move(blockContainer));
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
