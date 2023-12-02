#include "host.h"
#include <iostream>
#include <stdexcept>

int main() {
  std::cout << "Starting chat" << std::endl << std::endl;
  Host host;

  try {
    host.start();
  } catch (const std::runtime_error &err) {
    std::cout << err.what() << std::endl;
    return 1;
  }

  std::cout << "Exiting chat" << std::endl;
  return 0;
}