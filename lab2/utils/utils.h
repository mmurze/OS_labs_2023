#pragma once

#include <semaphore.h>

bool wait_sem_with_timeout(sem_t *sem, size_t timeout);