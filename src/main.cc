#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <iostream>

#include "Application.hpp"

int main() {
#ifdef _WIN32
  Application p{1280, 720, 4, 6};
#else
  Application p{1280, 720, 4, 1};
#endif

  if (!p.init()) {
    return 0;
  }
  p.run();

  return 0;
}