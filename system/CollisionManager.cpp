#include "CollisionManager.hpp"
#include "../entities/Entity.hpp"
#include "Utility.hpp"
#include <cmath>

CollisionManager::CollisionManager(SceneNode &rootNode)
  : nodes(rootNode)
{}

std::set<SceneNode::Pair>& CollisionManager::check(sf::Time dt) {
  pairs.clear();

  std::list<Entity*> entities;
  nodes.getAllEntities(entities);
  // std::list<Entity*>::iterator found;

  for(Entity* entity : entities) {
    if(entity->getVelocity().x == 0.f && entity->getVelocity().y == 0.f) {
      continue;
    }

    doCollisionChecking(dt, entity, entities, pairs);
  }

  return pairs;
}

void CollisionManager::doCollisionChecking(
    sf::Time time, Entity *entity, const std::list<Entity*>& entities,
    std::set<SceneNode::Pair>& pairs, int depth
) {

  if(time <= sf::Time::Zero) {
    return;
  }

  auto bpBox = getBroadphasingRect(*entity, time * 2.f);

  std::list<Entity*> found;
  std::copy_if(entities.begin(), entities.end(), std::back_inserter(found), [entity, &bpBox](const Entity *e) {
    return entity != e && bpBox.intersects(e->getBoundingRect());
  });

  if (found.empty()) {
    if(depth > 0) {
      Entity::Direction dir;
      dir.distance = 1.0;
      dir.deltaTime = time.asSeconds();
      dir.dir = sf::Vector2f(1, 1);
      entity->pushDirection(dir);
    }
    return;
  };

  auto pos = entity->getPosition();
  found.sort([&pos](const Entity* e1, const Entity* e2) {
    auto dist1 = Vector::length(e1->getPosition() - pos);
    auto dist2 = Vector::length(e2->getPosition() - pos);
    return dist1 > dist2;
  });

  for(auto const& otherEntity : found) {
    float nX, nY;
    float collisionTime = sweptAABB(*entity, *otherEntity, nX, nY);

    if (collisionTime < 1.0f) {
      pairs.emplace(std::minmax(entity, otherEntity));

      Entity::Direction dir;
      dir.distance = collisionTime;
      dir.deltaTime = time.asSeconds();

      if (std::abs(nX) > 0.0001f) dir.dir = sf::Vector2f(-1, 1);
      if (std::abs(nY) > 0.0001f) dir.dir = sf::Vector2f(1, -1);

      entity->pushDirection(dir);

      sf::Time remaining = time - time * collisionTime;
      doCollisionChecking(remaining, entity, entities, pairs, ++depth);
      return;
    }
  }
}

sf::FloatRect CollisionManager::getBroadphasingRect(const Entity& entity, sf::Time dt) {
  sf::FloatRect rect;
  auto b = entity.getBoundingRect();
  auto pos = entity.getPosition();
  auto vel = entity.getVelocity();
  auto offset = vel * dt.asSeconds();
  auto futurePos = pos + offset;

  rect.left = std::min(pos.x, futurePos.x) - b.width / 2.f;
  rect.top = std::min(pos.y, futurePos.y) - b.height / 2.f;
  rect.width = std::abs(futurePos.x - pos.x) + b.width;
  rect.height = std::abs(futurePos.y - pos.y) + b.height;

  return rect;
}

float CollisionManager::sweptAABB(const Entity& e1, const Entity& e2, float &normalX, float &normalY) {
  auto b1 = e1.getBoundingRect();
  auto b2 = e2.getBoundingRect();
  auto vel = e1.getVelocity();

  float xInvEntry, yInvEntry;
  float xInvExit, yInvExit;

  // find time of collision and time of leaving for each axis (if statement is to prevent divide by zero)
  float xEntry, yEntry;
  float xExit, yExit;

  // find the distance between the objects on the near and far sides for both x and y
  if (vel.x > 0.0f) {
    xInvEntry = b2.left - (b1.left + b1.width);
    xInvExit = (b2.left + b2.width) - b1.left;
  } else {
    xInvEntry = (b2.left + b2.width) - b1.left;
    xInvExit = b2.left - (b1.left + b1.width);
  }

  if (vel.y > 0.0f) {
    yInvEntry = b2.top - (b1.top + b1.height);
    yInvExit = (b2.top + b2.height) - b1.top;
  } else {
    yInvEntry = (b2.top + b2.height) - b1.top;
    yInvExit = b2.top - (b1.top + b1.height);
  }

  if (vel.x == 0.0f) {
    xEntry = -std::numeric_limits<float>::infinity();
    xExit = std::numeric_limits<float>::infinity();
  } else {
    xEntry = xInvEntry / vel.x;
    xExit = xInvExit / vel.x;
  }

  if (vel.y == 0.0f) {
    yEntry = -std::numeric_limits<float>::infinity();
    yExit = std::numeric_limits<float>::infinity();
  } else {
    yEntry = yInvEntry / vel.y;
    yExit = yInvExit / vel.y;
  }

  // find the earliest/latest times of collision
  float entryTime = std::max(xEntry, yEntry);
  float exitTime = std::min(xExit, yExit);

  // if there was no collision
  if (entryTime > exitTime || xEntry < 0.0f && yEntry < 0.0f || xEntry > 1.0f || yEntry > 1.0f) {
    normalX = 0.0f;
    normalY = 0.0f;
    return 1.0f;
  } else { // if there was a collision
    // calculate normal of collided surface
    if (xEntry > yEntry) {
      if (xInvEntry < 0.0f) {
        normalX = 1.0f;
        normalY = 0.0f;
      } else {
        normalX = -1.0f;
        normalY = 0.0f;
      }
    } else {
      if (yInvEntry < 0.0f) {
        normalX = 0.0f;
        normalY = 1.0f;
      } else {
        normalX = 0.0f;
        normalY = -1.0f;
      }
    }

    // return the time of collision
    return entryTime;
  }
}
