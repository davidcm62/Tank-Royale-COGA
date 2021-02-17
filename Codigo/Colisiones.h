#include <glad.h>
#include <glfw3.h>
#include <math.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <iostream>

#define PI 3.14159f
#define ARADIANES PI/180.0f
#define DIANTEESQX 0
#define DIANTEESQZ 1
#define DIANTEDERX 2
#define DIANTEDERZ 3
#define ATRASESQX 4
#define ATRASESQZ 5
#define ATRASDERX 6
#define ATRASDERZ 7

//colisiones circulo-rectangulo
bool circuloTocaRectangulo(float posXcirc, float posZcirc, float radioCirc, float posXrect, float posZrect, float ancho, float alto);

bool circuloTocaRectanguloRotado(float posXcirc, float posZcirc, float radioCirc, float posXrect, float posZrect, float ancho, float alto, float angulo);

//funcion para obter as esquinas dun rectangulo
float* obterEsquinasObjeto(float posX, float posZ, float angulo, float dimX, float dimZ);

//funcions para calcular colisiones punto-rectangulo
bool puntoSaleDoSuelo(float punto, float dimensionSuelo);

bool puntoColisionaObjeto(float posX, float posY, float posZ, float* esquinas);

//colision punto-circulo
bool puntoSaleDoPerimetro(float posX, float posZ, float posXcirc, float posZcirc, float radio);