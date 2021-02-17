#include <stdio.h>
#include <iostream>

/*
	Estructuras para os obxectos
*/
typedef struct {
	float posX, posY, posZ;
	float aEjeX;
	float aEjeY;
	float scaleX, scaleY, scaleZ;
	unsigned int VAO;
}Pieza;

typedef struct {
	Pieza base;
	Pieza baseTorreta;
	Pieza esferaBase;
	Pieza canon;
	Pieza esferaCanon;
	float velocidade;
	bool debeSerPintado;
	float tempoIniciMovimiento;
}Tanque;

typedef struct {
	float posX, posY, posZ;
	float scaleX, scaleY, scaleZ;
	float anguloSalidaX;
	float anguloSalidaY;
	float velocidadeHorizontal;
	float velocidadeVertical;
	bool debeSerPintado;
	float timer;
}Proyectil;

typedef struct {
	float posX, posY, posZ;
	float aEjeY;
	unsigned int VAOcasa;
	unsigned int VAOtellado;
}Casa;

typedef struct {
	float posX, posY, posZ;
	float aEjeX, aEjeY, aEjeZ;
	float velocidade;
	unsigned int VAOala;
}Avion;

typedef struct {
	float posX, posY, posZ;
	float aEjeY;
	unsigned int VAOcubo;
}Arbol;

typedef struct {
	const char* texturasPaths[6];
	unsigned int VAOcubo;
	unsigned int textura;
} SkyBox;

//funcions para inicializar
Tanque inicializarTanque(float posX, float posY, float posZ);

Casa inicializarCasa(float posX, float posZ, float aEjeY);

Avion inicializarAvion();

Arbol inicializarArbol(float posX, float posZ);

SkyBox inicializarSkyBox(const char* left, const char* right, const char* top, const char* bottom, const char* back, const char* front);