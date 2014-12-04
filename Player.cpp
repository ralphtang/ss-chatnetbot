#include <functional>
#include <utility>

#include "Message.hpp"
#include "Player.hpp"

Player::Player(const std::string &name, const std::string &password) : _name(name), 
  _password(password), _running(false)
{
  this->_connection = std::make_shared<ChatnetConnection>();
}

std::weak_ptr<ChatnetConnection> getConnection()
{
  return this->_connection;
}

bool Player::connect(const Zone& zone)
{
  if (this->_connection)
    return false;

  return this->_connection->connect(zone) == ChatConnection::ReturnTypes::CONN_OK;
}

bool Player::disconnect()
{
  if (!this->_connection)
    return false;
  this->_connection->disconnect();
  this->_connection = nullptr;

  return true;
}

void Player::runListen()
{
  if (this->_running)
    return;

  this->_running = true;
  this->_thd = std::thread(std::bind(&Player::_runListen, this));
}

void Player::_runListen()
{
  std::list<Message> msgs;
  auto conn = this->_connection;
  while (this->_running)
  {
    if (conn->listen(msgs) == ChatnetConnection::ReturnTypes::RECV_OK)
    {
      for (auto m : msgs)
        this->notifyAll(m);
      msgs.clear();
    }
  }
}

void Player::stopListen()
{
  if (!this->_running)
    return;

  this->_running = false;
  this->_thd.join();
}

void Player::sendFreqMessage(const std::string& freq, const std::string& message)
{
  if (!*(this->_connection))
    return;
  this->_connection->send(Message::createTeamMessage(this->_name, freq, message));
}

void Player::sendPrivateMessage(const std::string& playerTo, const std::string& message)
{
  if (!*(this->_connection))
    return;
  this->_connection->send(Message::createPrivateMessage(this->_name, playerTo, message));
}

void Player::goToArena(const std::string& arena)
{
  if (!*(this->_connection))
    return;
  this->_connection->send(Message::createGoMessage(arena));
}