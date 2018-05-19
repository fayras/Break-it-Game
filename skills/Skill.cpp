#include "Skill.hpp"
#include "../system/Utility.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <cmath>

Skill::Skill(const sf::Texture &texture, const sf::Font &font, sf::Keyboard::Key key, float cooldownInSeconds, float activationTime)
  : SpriteNode(texture), cooldownTime(sf::seconds(cooldownInSeconds)), assignedKey(), activationTime(sf::seconds(activationTime))
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
  assignedKey.setOutlineThickness(1.5f);
  assignedKey.setOutlineColor(sf::Color::Black);

  sf::FloatRect textBounds = assignedKey.getLocalBounds();
  assignedKey.setOrigin(std::floor(textBounds.left + textBounds.width), std::floor(textBounds.top));

  assignedKey.setPosition(60, 5);
}

Skill::Skill(const sf::Texture &texture, const sf::IntRect &textureRect, const sf::Font &font, sf::Keyboard::Key key,
             float cooldownInSeconds, float activationTime)
        : SpriteNode(texture, textureRect), cooldownTime(sf::seconds(cooldownInSeconds)), assignedKey(), activationTime(sf::seconds(activationTime))
{
  auto bounds = sprite.getGlobalBounds();
  // sprite.scale(64.f / bounds.width, 64.f / bounds.height);
  cooldownText.setFont(font);
  cooldownText.setCharacterSize((unsigned int) textureRect.width / 2);
  cooldownText.setPosition(textureRect.width / 2.f, textureRect.height / 2.f);

  assignedKey.setFont(font);
  assignedKey.setCharacterSize(14);
  assignedKey.setStyle(sf::Text::Bold);
  assignedKey.setString(String::from(key));
  assignedKey.setOutlineThickness(1.5f);
  assignedKey.setOutlineColor(sf::Color::Black);

  sf::FloatRect textBounds = assignedKey.getLocalBounds();
  assignedKey.setOrigin(std::floor(textBounds.left + textBounds.width), std::floor(textBounds.top));

  assignedKey.setPosition(textureRect.width - 4, 4);
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
    activation = activationTime;
    cooldown = cooldownTime;
    commands.push(command);
  }

  if(activateSkill && activation > sf::Time::Zero) {
    activation -= dt;
  } else {
    if(activateSkill) {
      activateSkill = false;
      commands.push(deactivateCommand);
    }
    if(cooldown > sf::Time::Zero) {
      setColor({100, 100, 100});
      cooldownText.setString(std::to_string((int) cooldown.asSeconds() + 1));
      centerOrigin(cooldownText);

      cooldown -= dt;
    } else {
      setColor({255, 255, 255});
    }
  }
}
