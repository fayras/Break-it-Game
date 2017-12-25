#ifndef SFML_TEMPLATE_SCENENODE_HPP
#define SFML_TEMPLATE_SCENENODE_HPP

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include <vector>
#include <memory>
#include <map>

#include "Category.hpp"
#include "../system/CommandQueue.hpp"

struct Command;
class CommandQueue;

enum CollisionSide {
  NONE = 0, LEFT = 1, RIGHT = 1<<1, TOP = 1<<2, BOTTOM = 1<<3
};

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable {
  public:
    typedef std::unique_ptr<SceneNode> Ptr;
    typedef std::pair<SceneNode*, SceneNode*> Pair;

    SceneNode(Category::Type category = Category::NONE);

    void attachChild(Ptr child);
    Ptr detachChild(const SceneNode& node);

    sf::Vector2f getWorldPosition() const;
    sf::Transform getWorldTransform() const;

    void onCommand(const Command& command, sf::Time dt);
    void setCategory(Category::Type category);
    virtual unsigned int getCategory() const;

    void update(sf::Time dt, CommandQueue& commands);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    virtual sf::FloatRect	getBoundingRect() const;

    void checkSceneCollision(SceneNode& sceneGraph, std::map<Pair, CollisionSide>& collisionPairs);
    void checkNodeCollision(SceneNode& node, std::map<Pair, CollisionSide>& collisionPairs);
    void removeWrecks();
    virtual bool isMarkedForRemoval() const;
    virtual bool isDestroyed() const;

  protected:
    virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
    void updateChildren(sf::Time dt, CommandQueue& commands);

    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

    std::vector<Ptr> children;
    SceneNode* parent;
    Category::Type defaultCategory;
};

CollisionSide collision(const SceneNode &lhs, const SceneNode &rhs);
float	distance(const SceneNode& lhs, const SceneNode& rhs);

#endif //SFML_TEMPLATE_SCENENODE_HPP
