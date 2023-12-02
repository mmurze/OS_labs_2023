#pragma once

#include "conn.h"

#include <string>
#include <sys/socket.h>

class Socket : public Connection {
private:
  const std::string SOCKET_ROUTE = "/tmp/socket_conn_";
  const size_t MAX_CLIENT_NUM = 1;

  socklen_t hostSocket;
  socklen_t clientSocket;

  bool isHost = false;
  std::string name;

public:
  Socket() = default;
  ~Socket() = default;

  bool open(bool isHost) final;
  bool read(Message &msg) const final;
  bool write(const Message &msg) final;
  bool close() final;
};