#include "host.h"
#include "utils.h"
#include <cstring>
#include <signal.h>
#include <sys/mman.h>
#include <unistd.h>

Host::Host() {
  sem_host_msg = (sem_t *)mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
                               MAP_ANON | MAP_SHARED, -1, 0);
  sem_client_msg = (sem_t *)mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
                                 MAP_ANON | MAP_SHARED, -1, 0);
  if (sem_host_msg == MAP_FAILED || sem_client_msg == MAP_FAILED)
    throw std::runtime_error(
        "Couldn't allocate shared memory for semaphores with mmap");

  if (sem_init(sem_host_msg, 1, 0) == -1 ||
      sem_init(sem_client_msg, 1, 0) == -1)
    throw std::runtime_error("Couldn't initialize semaphores with sem_init");

  connection = Connection::create();
  connection->open(true);
}

Host::~Host() {
  if (sem_destroy(sem_host_msg) == -1 || sem_destroy(sem_client_msg) == -1) {
    std::cout << "Couldn't destroy semaphores" << std::endl;
    std::exit(1);
  }

  if (munmap(sem_host_msg, sizeof(sem_t)) == -1 ||
      munmap(sem_client_msg, sizeof(sem_t)) == -1) {
    std::cout << "Couldn't free shared memory from semaphores" << std::endl;
    std::exit(1);
  }
}

void *Host::read_message(void *host) {
  while (true) {
    if (!wait_sem_with_timeout(((Host *)host)->sem_client_msg, 60)) {
      kill(((Host *)host)->child_pid, SIGKILL);
      break;
    }

    Message msg;
    ((Host *)host)->connection->read(msg);

    std::cout << "Message from client: " << msg.text << std::endl;
  }

  pthread_exit(NULL);
}
void *Host::write_message(void *host) {
  Message msg;

  while (true) {
    std::string line;
    std::getline(std::cin, line);

    if (line == "!QUIT") {
      break;
    }

    strncpy(msg.text, line.c_str(), sizeof(msg.text) - 1);
    msg.text[sizeof(msg.text) - 1] = '\0';

    ((Host *)host)->connection->write(msg);
    sem_post(((Host *)host)->sem_host_msg);
  }

  pthread_exit(NULL);
}

void Host::start() {
  add_client();

  pthread_t read_thread;
  pthread_t write_thread;

  pthread_create(&read_thread, NULL, Host::read_message, this);
  pthread_create(&write_thread, NULL, Host::write_message, this);

  pthread_join(read_thread, NULL);
  pthread_join(write_thread, NULL);
}

void Host::add_client() {
  child_pid = fork();
  if (child_pid == 0) {
    Client cl(sem_host_msg, sem_client_msg);
    cl.run();
  } else if (child_pid == -1)
    throw std::runtime_error("Couldn't create child proccess");
}