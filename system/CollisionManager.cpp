#include "CollisionManager.hpp"
#include "../entities/Entity.hpp"

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
    std::set<SceneNode::Pair>& pairs
) {

  if(time >= sf::Time::Zero) {
    return;
  }

  auto bpBox = getBroadphasingRect(*entity, time);

  auto found = std::find_if(entities.begin(), entities.end(), [entity, &bpBox](const Entity *e) {
    return entity != e && bpBox.intersects(e->getBoundingRect());
  });

  float nX, nY;
  float collisionTime = sweptAABB(*entity, **found, nX, nY);

  if(collisionTime < 1.0f) {
    pairs.emplace(std::minmax(entity, *found));
    // entity response
    sf::Time remaining = time - time * collisionTime;
    doCollisionChecking(time, entity, entities, pairs);
  }
}

sf::FloatRect CollisionManager::getBroadphasingRect(const Entity& entity, sf::Time dt) {
  sf::FloatRect rect;
  auto b = entity.getBoundingRect();
  auto pos = entity.getPosition();
  auto vel = entity.getVelocity();
  auto offset = vel * dt.asSeconds();
  auto futurePos = pos + offset;

  rect.left = std::min(pos.x, futurePos.x);
  rect.top = std::min(pos.y, futurePos.y);
  rect.width = std::abs(futurePos.x - pos.x) + b.width;
  rect.height = std::abs(futurePos.y - pos.y) + b.height;
  // rect.top = b.vy > 0 ? b.y : b.y + b.vy;
  // rect.width = b.vx > 0 ? b.vx + b.w : b.w - b.vx;
  // rect.height = b.vy > 0 ? b.vy + b.h : b.h - b.vy;

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
