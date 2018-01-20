#include "Skill.hpp"

Skill::Skill(const sf::Texture &texture, float cooldownInSeconds)
  : SpriteNode(texture), cooldownTime(sf::seconds(cooldownInSeconds))
{}

void Skill::activate() {
  activateSkill = true;
}

void Skill::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
  if(cooldown <= sf::Time::Zero) {
    SpriteNode::drawCurrent(target, states);
  }
}

void Skill::updateCurrent(sf::Time dt, CommandQueue &commands) {
  SceneNode::updateCurrent(dt, commands);

  if(activateSkill && cooldown <= sf::Time::Zero) {
    cooldown = cooldownTime;
    commands.push(command);
  }

  cooldown -= dt;
  activateSkill = false;
}
