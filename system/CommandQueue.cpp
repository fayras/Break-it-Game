#include "CommandQueue.hpp"

void CommandQueue::push(const Command &comand) {
  queue.push(comand);
}

Command CommandQueue::pop() {
  Command command = queue.front();
  queue.pop();
  return command;
}

bool CommandQueue::empty() const {
  return queue.empty();
}
