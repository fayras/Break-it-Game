#ifndef SFML_TEMPLATE_SCENENODE_HPP
#define SFML_TEMPLATE_SCENENODE_HPP

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include <vector>
#include <memory>

#include "Category.hpp"
#include "../system/CommandQueue.hpp"

struct Command;

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
    virtual unsigned int getCategory() const;

    void update(sf::Time dt, CommandQueue& commands);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

  protected:
    virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
    void updateChildren(sf::Time dt, CommandQueue& commands);

    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

    std::vector<Ptr> children;
    SceneNode* parent;
    Category::Type defaultCategory;
};


#endif //SFML_TEMPLATE_SCENENODE_HPP
