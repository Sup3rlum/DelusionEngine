#pragma once



#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>

#include <vector>
#include <random>
#include <cmath>
#include <ctime>
#include <chrono>
#include <time.h>
#include <map>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <stack>

#include "framework.h"
#include "logger.h"
#include "../util/macros.h"
#include "../util/result.h"



#define FL_INT32 int
#define FL_INT64 long long

#define _UNS_ unsigned

#define FL_HANDLE _UNS_ FL_INT32




typedef glm::mat3x3 Matrix3;
typedef glm::mat4x4 Matrix4;
typedef glm::vec2 Vector2;
typedef glm::vec3 Vector3;
typedef glm::vec4 Vector4;
typedef glm::quat Quaternion;
