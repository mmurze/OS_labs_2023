#pragma once

#include "../client/client.h"
#include "../connections/conn.h"
#include "../utils/message.h"
#include <iostream>
#include <semaphore.h>
// #include <signal.h>
// #include <sys/mman.h>
// #include <unistd.h>

class Host {
public:
  Host();
  ~Host();

  void start();

private:
  void add_client();
  static void *read_message(void *host);
  static void *write_message(void *host);

  sem_t *sem_host_msg, *sem_client_msg;
  std::unique_ptr<Connection> connection;
  pid_t child_pid;
  const size_t sem_timeout_secs = 5;
};