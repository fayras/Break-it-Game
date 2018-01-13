#ifndef SFML_TEMPLATE_SCENENODE_HPP
#define SFML_TEMPLATE_SCENENODE_HPP

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include <list>
#include <memory>
#include <map>

#include "Category.hpp"
#include "../system/CommandQueue.hpp"
#include "../tween/Tweenable.hpp"

struct Command;
class CommandQueue;

enum CollisionSide {
  NONE = 0, LEFT = 1, RIGHT = 1<<1, TOP = 1<<2, BOTTOM = 1<<3
};

class SceneNode : public virtual sf::Transformable, public sf::Drawable, private sf::NonCopyable, public virtual Tweenable {
  public:
    typedef std::unique_ptr<SceneNode> Ptr;
    typedef std::pair<SceneNode*, SceneNode*> Pair;

    explicit SceneNode(Category::Type category = Category::NONE);

    void attachChild(Ptr child);
    void attachChildFront(Ptr child);
    Ptr detachChild(const SceneNode& node);
    bool containsNode(Category::Type type) const;

    sf::Vector2f getWorldPosition() const;
    sf::Transform getWorldTransform() const;
    bool hasChildren() const;

    void onCommand(const Command& command, sf::Time dt);
    void setCategory(Category::Type category);
    virtual unsigned int getCategory() const;

    void update(sf::Time dt, CommandQueue& commands);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

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

    std::list<Ptr> children;
    SceneNode* parent;
    Category::Type defaultCategory;
};

CollisionSide collision(const SceneNode &lhs, const SceneNode &rhs);
float	distance(const SceneNode& lhs, const SceneNode& rhs);

#endif //SFML_TEMPLATE_SCENENODE_HPP
