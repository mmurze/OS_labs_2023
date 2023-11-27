#pragma once

#include "../connections/conn.h"
#include <fstream>
#include <semaphore.h>
#include <stdexcept>

class Client {
public:
  Client(sem_t *sem_host, sem_t *sem_client);
  ~Client();

  void run();

private:
  static void *read_message(void *client);
  static void *write_message(void *client);

  sem_t *sem_client_msg, *sem_host_msg;
  std::unique_ptr<Connection> connection;
  const size_t sem_timeout_secs = 5;
  std::ofstream log;
};