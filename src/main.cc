#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <iostream>

#include "Program.hpp"

int main() {
#ifdef _WIN32
  Program p{1280, 720, 4, 6};
#else
  Program p{1280, 720, 4, 1};
#endif

  if (!p.init()) {
    return 0;
  }
  p.run();

  return 0;
}