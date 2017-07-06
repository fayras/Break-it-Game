#include "World.hpp"

World::World(sf::RenderTarget &outputTarget, FontHolder &fonts, SoundPlayer &sounds)
  : target(outputTarget),
    sceneTexture(),
    worldView(outputTarget.getDefaultView()),
    textures(),
    fonts(fonts),
    sounds(sounds),
    sceneGraph(),
    sceneLayers(),
    worldBounds(0.f, 0.f, worldView.getSize().x, 5000.f),
    spawnPosition(worldView.getSize().x / 2.f, worldBounds.height - worldView.getSize().y / 2.f)
{
  sceneTexture.create(target.getSize().x, target.getSize().y);
  loadTextures();
  buildScene();

  // Prepare the view
  worldView.setCenter(spawnPosition);
}

void World::update(sf::Time dt) {
  while(!commandQueue.empty()) {
    sceneGraph.onCommand(commandQueue.pop(), dt);
  }

  // Collision detection and response (may destroy entities)
  handleCollisions();

  // Remove all destroyed entities, create new ones
  sceneGraph.removeWrecks();

  sceneGraph.update(dt, commandQueue);
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

}

void World::adaptPlayerPosition() {
  // Keep player's position inside the screen bounds, at least borderDistance units from the border
  sf::FloatRect viewBounds = getViewBounds();
  const float borderDistance = 40.f;

  sf::Vector2f position = playerAircraft->getPosition();
  position.x = std::max(position.x, viewBounds.left + borderDistance);
  position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
  position.y = std::max(position.y, viewBounds.top + borderDistance);
  position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
  playerAircraft->setPosition(position);
}

void World::adaptPlayerVelocity() {

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
  sounds.setListenerPosition(playerAircraft->getWorldPosition());

  // Remove unused sounds
  sounds.removeStoppedSounds();
}

void World::buildScene() {

}

sf::FloatRect World::getViewBounds() const {
  return sf::FloatRect();
}

sf::FloatRect World::getBattlefieldBounds() const {
  return sf::FloatRect();
}
