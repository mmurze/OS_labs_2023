#include "utils.h"
#include <ctime>
#include <iostream>
#include <semaphore.h>
#include <stdexcept>

bool wait_sem_with_timeout(sem_t *sem, size_t timeout) {
  struct timespec ts;
  if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
    throw std::runtime_error("Couldn't get current time");
  ts.tv_sec += timeout;

  int s;
  while ((s = sem_timedwait(sem, &ts)) == -1 && errno == EINTR)
    continue;
  if (s == -1 && errno == ETIMEDOUT)
    return false;

  return true;
}
