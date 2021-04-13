#ifndef GL_MY_UTILITY_HPP
#define GL_MY_UTILITY_HPP
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <reactphysics3d/reactphysics3d.h>

#include <iostream>
#include <string>

#include "Camera.hpp"

struct PhysicsProperties {
  reactphysics3d::PhysicsWorld* physicsWorld = nullptr;
  reactphysics3d::PhysicsCommon* physicsCommon = nullptr;
};

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                GLsizei length, const GLchar* message,
                                const void* userParam);

void onWindowResize(GLFWwindow* window, int width, int height);

auto shaderTypeToString(GLenum shaderType) -> std::string;

template <typename T>
GLenum typeToGL();

#endif