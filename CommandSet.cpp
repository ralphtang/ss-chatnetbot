#include "CommandSet.hpp"
#include "Message.hpp"
#include "Player.hpp"
#include "ThreadPool.hpp"

#include <functional>
#include <utility>

void CommandSet::addCommand(std::unique_ptr<Command> command)
{
  this->_commands.push_back(command);
}

void CommandSet::_onMessage(std::shared_ptr<Player> player, const Message& message)
{
  auto fn = std::bind(&CommandSet::_dispatchMessage, this, player, message);
  ThreadPool::instance().queueSimpleTask(fn);
}

void CommandSet::_dispatchMessage(std::shared_ptr<Player> player, const Message& message)
{
  for (auto cmd : this->_commands)
    cmd.onMessage(player, message);
}

std::vector<std::unique_ptr<Command>>& CommandSet::getCommands()
{
  return this->_commands;
}