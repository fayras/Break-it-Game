#ifndef SFML_TEMPLATE_COMMANDQUEUE_HPP
#define SFML_TEMPLATE_COMMANDQUEUE_HPP

#include "Command.hpp"
#include <queue>

class CommandQueue {
  public:
    void push(const Command& comand);
    Command pop();
    bool empty() const;

  private:
    std::queue<Command> queue;
};

#endif //SFML_TEMPLATE_COMMANDQUEUE_HPP
