#include "conn_fifo.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/syslog.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

std::unique_ptr<Connection> Connection::create() {
  return std::make_unique<Fifo>();
}

bool Fifo::open(bool isHost) {
  name = FIFO_ROUTE;
  this->isHost = isHost;
  if (isHost) {
    if (mkfifo(name.c_str(), S_IRWXU | S_IRWXG | S_IRWXO) < 0) {
      syslog(LOG_ERR, "ERROR: failed to create");
      return false;
    }
  }
  descriptor = ::open(name.c_str(), O_RDWR);
  if (descriptor < 0) {
    syslog(LOG_ERR, "ERROR: failed to open");
    return false;
  }

  return true;
}

bool Fifo::read(Message &msg) const {
  if (::read(descriptor, &msg, sizeof(Message)) < 0) {
    syslog(LOG_ERR, "ERROR: failed to read");
    return false;
  }
  return true;
}

bool Fifo::write(const Message &msg) {
  if (::write(descriptor, &msg, sizeof(Message)) < 0) {
    syslog(LOG_ERR, "ERROR: failed to write");
    return false;
  }
  return true;
}

bool Fifo::close() {
  if (::close(descriptor) < 0) {
    syslog(LOG_ERR, "ERROR: failed to close");
    return false;
  }
  if (isHost) {
    if (unlink(name.c_str()) < 0) {
      syslog(LOG_ERR, "ERROR: failed to unlink");
      return false;
    }
  }
  return true;
}