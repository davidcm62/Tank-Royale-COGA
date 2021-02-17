#include "Obxectos.h"
#include "FuncionsVAOs.h"
#include "Texturas.h"

Pieza crearPeza(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ) {
	Pieza p;
	p.posX = posX;
	p.posY = posY;
	p.posZ = posZ;
	p.scaleX = scaleX;
	p.scaleY = scaleY;
	p.scaleZ = scaleZ;
	p.aEjeX = 0.0f;
	p.aEjeY = 0.0f;
	return p;
}

Tanque inicializarTanque(float posX, float posY, float posZ) {
	Tanque t;
	t.base = crearPeza(posX, posY, posZ, 2.5, 1, 2.5);
	t.base.VAO = crearCubo();

	t.baseTorreta = crearPeza(0, 0.8, 0, 1.5, 0.8, 2.0);
	t.baseTorreta.VAO = crearCubo();

	t.esferaBase = crearPeza(0, 0.45, 0, 0.5, 0.5, 0.5);

	t.canon = crearPeza(0, 0, -0.75, 0.3, 0.3, 2.4);
	t.canon.VAO = crearCubo();

	t.esferaCanon = crearPeza(0, 0, -1.2, 0.2, 0.2, 0.2);

	t.velocidade = 0.0f;
	t.debeSerPintado = true;
	t.tempoIniciMovimiento = glfwGetTime();
	return t;
}

Casa inicializarCasa(float posX, float posZ, float aEjeY) {
	Casa c;
	c.posX = posX;
	c.posY = 0.0f;
	c.posZ = posZ;
	c.aEjeY = aEjeY;
	c.VAOcasa = crearCasita();
	c.VAOtellado = crearCubo();

	return c;
}

Avion inicializarAvion() {
	Avion a;
	a.posX = -80;
	a.posY = 40;
	a.posZ = 0;
	a.aEjeX = 0;
	a.aEjeY = 0;
	a.aEjeZ = 5;
	a.velocidade = 0.08;
	a.VAOala = crearAla();
	return a;
}

Arbol inicializarArbol(float posX, float posZ) {
	Arbol a;
	a.posX = posX;
	a.posY = 0.0;
	a.posZ = posZ;
	a.aEjeY = rand() % 360;
	a.VAOcubo = crearCubo();
	return a;
}

SkyBox inicializarSkyBox(const char *left, const char* right, const char* top, const char* bottom, const char* back, const char* front) {
	SkyBox skybox;
	skybox.texturasPaths[0] = "left.jpg";
	skybox.texturasPaths[1] = "right.jpg";
	skybox.texturasPaths[2] = "top.jpg";
	skybox.texturasPaths[3] = "bottom.jpg";
	skybox.texturasPaths[4] = "back.jpg";
	skybox.texturasPaths[5] = "front.jpg";

	skybox.VAOcubo = crearCuboSkyBox();
	skybox.textura = cargarTexturasSkyBox(skybox.texturasPaths);

	return skybox;
}