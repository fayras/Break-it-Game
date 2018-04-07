#include "Skill.hpp"
#include "../system/Utility.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

Skill::Skill(const sf::Texture &texture, const sf::Font &font, float cooldownInSeconds, sf::Keyboard::Key key)
  : SpriteNode(texture), cooldownTime(sf::seconds(cooldownInSeconds)), assignedKey()
{
  auto bounds = sprite.getGlobalBounds();
  sprite.scale(64.f / bounds.width, 64.f / bounds.height);
  cooldownText.setFont(font);
  cooldownText.setCharacterSize(30);
  cooldownText.setPosition(32, 32);

  assignedKey.setFont(font);
  assignedKey.setCharacterSize(14);
  assignedKey.setStyle(sf::Text::Bold);
  assignedKey.setString(String::from(key));

  sf::FloatRect textBounds = assignedKey.getLocalBounds();
  assignedKey.setOrigin(std::floor(textBounds.left + textBounds.width), std::floor(textBounds.top));

  assignedKey.setPosition(60, 5);
}

void Skill::activate() {
  activateSkill = true;
}

void Skill::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
  SpriteNode::drawCurrent(target, states);
  target.draw(assignedKey, states);
  if(cooldown > sf::Time::Zero) {
    target.draw(cooldownText, states);
  }
}

void Skill::updateCurrent(sf::Time dt, CommandQueue &commands) {
  SceneNode::updateCurrent(dt, commands);

  if(activateSkill && cooldown <= sf::Time::Zero) {
    cooldown = cooldownTime;
    commands.push(command);
  }

  if(cooldown > sf::Time::Zero) {
    setColor({100, 100, 100});
    cooldownText.setString(std::to_string((int) cooldown.asSeconds() + 1));
    centerOrigin(cooldownText);
  } else {
    setColor({255, 255, 255});
  }

  cooldown -= dt;
  activateSkill = false;
}
