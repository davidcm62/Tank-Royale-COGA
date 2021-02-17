#include <glad.h>
#include <glfw3.h>
#include <stdio.h>
#include <math.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

//textura normal
GLuint cargarTextura(const char* pathTextura);

//textura dun cubo
unsigned int cargarTexturasSkyBox(const char* pathTextura[6]);