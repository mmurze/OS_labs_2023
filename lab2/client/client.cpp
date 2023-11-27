#include "client.h"
#include "utils.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <unistd.h>

Client::Client(sem_t *sem_host, sem_t *sem_client) {
  log.open("log.file");

  sem_host_msg = sem_host;
  sem_client_msg = sem_client;

  connection = Connection::create();
  connection->open(false);
}

Client::~Client() { log.close(); }

void *Client::read_message(void *client) {
  while (true) {
    sem_wait(((Client *)client)->sem_host_msg);

    Message msg;
    ((Client *)client)->connection->read(msg);

    ((Client *)client)->log << "Message from host: " << msg.text << std::endl
                            << std::flush;
  }

  pthread_exit(NULL);
}

void *Client::write_message(void *client) {

  system("gnome-terminal -e \"bash -c 'cp /dev/stdin  myfile.txt | tail -f "
         "log.file ;'\"");
  Message msg;

  std::streampos last_position = 0;

  while (true) {
    std::ifstream input_file("myfile.txt", std::ios::in | std::ios::ate);

    std::streampos current_position = input_file.tellg();
    input_file.seekg(last_position);

    std::string line;
    std::getline(input_file, line);

    last_position = current_position;

    if (line == "!QUIT") {
      break;
    }

    if (line == "") {
      continue;
    }

    strncpy(msg.text, line.c_str(), sizeof(msg.text) - 1);
    msg.text[sizeof(msg.text) - 1] = '\0';

    ((Client *)client)->connection->write(msg);

    sem_post(((Client *)client)->sem_client_msg);

    // sleep thread for 3 seconds
    usleep(3000000);
  }

  pthread_exit(NULL);
}

void Client::run() {
  pthread_t read_thread;
  pthread_t write_thread;

  pthread_create(&read_thread, NULL, Client::read_message, this);
  pthread_create(&write_thread, NULL, Client::write_message, this);

  pthread_join(read_thread, NULL);
  pthread_join(write_thread, NULL);
}