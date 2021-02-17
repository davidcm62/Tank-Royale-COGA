#include <glad.h>
#include <glfw3.h>
#include <stdio.h>
#include <math.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <windows.h>
#include <mmsystem.h>

#include "FuncionsVAOs.h"
#include "Sphere.h"
#include "Obxectos.h"
#include "Colisiones.h"
#include "Texturas.h"

//constantes
#define SCR_WIDTH 800
#define SCR_HEIGHT 800
#define CAMARATANQUE 1
#define CAMARAAVION 2
#define CAMARAEXT 3
#define DIMENSIONSUELO 100
#define RANGOVUELOAVION 120
#define TOTALARBOLES 20
#define TOTALTANQUESAI 10
#define MAXBALAS 100
#define MAXBOMBAS 50
int ancho = SCR_WIDTH;
int alto = SCR_HEIGHT;

//funcions OpenGL e externas
extern GLuint setShaders(const char* nVertx, const char* nFrag);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

//shaders
GLuint shaderProgram;
GLuint shaderSkyBox;
unsigned int transformLoc;

//tanques e avion
Tanque tanqueXogador;
Proyectil municion[MAXBALAS];
int balasUsadas = 0;
Tanque tanquesAI[TOTALTANQUESAI];
float girosAI[10] = { 0,0,0,0,0,-2,-2,2,2,2 };
Avion avionXogador;
Proyectil bombas[MAXBOMBAS];
int bombasUsadas = 0;

//variable para saber que vehiculo se esta controlando
// true -> tanque
// false -> avion
bool estaseContralandoTanque = true;

//obxectos
Casa casas[4];
Arbol arboles[TOTALARBOLES];
SkyBox skybox;

//esfera e vaosuelo e cubo
Sphere sphere(1.0f, 144, 72);
unsigned int VAOcuadrado;
unsigned int VAOcubo;

//texturas
unsigned int texturaMenu;
unsigned int texturaSuelo;
unsigned int texturaMetal;
unsigned int texturaMetalAvion;
unsigned int texturaAcero;
unsigned int texturaMadeira;
unsigned int texturaTella;
unsigned int texturaTronco;
unsigned int texturaFollas;

//camara
float posCamara[3] = { 0,0,0 };
int camaraActual = CAMARATANQUE;
bool xogoEmpezado = false;

//inits
void openGlInit() {
	glClearDepth(1.0f); //Valor z-buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // valor limpieza buffer
	glEnable(GL_DEPTH_TEST); // z-buffer
	glEnable(GL_CULL_FACE); //ocultacion caras atras
}

void initTanques() {
	tanqueXogador = inicializarTanque(0, 0, 0);
	//genero os tanques AI en posiciones aleatorias
	for (int i = 0; i < TOTALTANQUESAI; i++) {
		int negativoPostivo = rand() % 2;
		int posX = (negativoPostivo == 0) ? -1 * rand() % 90 : rand() % 90;
		negativoPostivo = rand() % 2;
		int posZ = (negativoPostivo == 0) ? -1 * rand() % 90 : rand() % 90;
		tanquesAI[i] = inicializarTanque(posX, 0, posZ);
	}
}

void initMunicion() {
	for (int i = 0; i < MAXBALAS; i++)
		municion[i] = { 0.0,1.15,0.0,0.16,0.16,0.35,0.0,0.0,0.6,0.6,true };
	
	for (int i = 0; i < MAXBOMBAS; i++)
		bombas[i] = { 0.0,0.0,0.0,0.4,0.4,0.7,90.0,0.0,0.0,-0.3,true };
}

void initCasas() {
	casas[0] = inicializarCasa(50.0, 50.0, 50.0);
	casas[1] = inicializarCasa(-50.0, 50.0, 0.0);
	casas[2] = inicializarCasa(50.0, -50.0, 0.0);
	casas[3] = inicializarCasa(-50.0, -50.0, 50.0);
}

void initAviones() {
	avionXogador = inicializarAvion();
}

void initSkyBox() {
	skybox = inicializarSkyBox("left.jpg", "right.jpg", "top.jpg", "bottom.jpg", "back.jpg", "front.jpg");
}

bool arbolPodeIrEn(float posX, float posZ) {
	bool resultado = true;
	for (int i = 0; i < 4; i++) {
		float* esquinasCasa = obterEsquinasObjeto(
			casas[i].posX,
			casas[i].posZ,
			0,
			0.5f * 8.0f,
			0.5f * 8.0f
		);
		if (puntoColisionaObjeto(posX, 0, posZ, esquinasCasa)) resultado = false;
		free(esquinasCasa);
		if (!resultado) break;
	}
	return resultado;
}

void initArboles() {
	float posX, posZ;
	for (int x = 0; x < TOTALARBOLES; x++) {
		//genero posiciones hasta que non se meta onde hai unha casa
		do {
			int negativoPostivo = rand() % 2;
			posX = (negativoPostivo == 0) ? -1 * rand() % 90 : rand() % 90;
			negativoPostivo = rand() % 2;
			posZ = (negativoPostivo == 0) ? -1 * rand() % 90 : rand() % 90;
		} while (!arbolPodeIrEn(posX, posZ));
		arboles[x] = inicializarArbol(posX, posZ);
	}
}

void initTexturas() {
	texturaSuelo = cargarTextura("floor.jpg");
	texturaMetal = cargarTextura("metal.jpg");
	texturaAcero = cargarTextura("acero.jpg");
	texturaMadeira = cargarTextura("madeira.jpg");
	texturaTella = cargarTextura("tella.jpg");
	texturaMetalAvion = cargarTextura("metalAvion.jpg");
	texturaFollas = cargarTextura("follas.jpg");
	texturaTronco = cargarTextura("tronco.jpg");
}

//funciones para pintar
glm::mat4 pintarObjeto(glm::mat4 matInicial, glm::mat4 matUsada,
	float posX, float posY, float posZ,
	float aEjeX, float aEjeY, float aEjeZ,
	float scaleX, float scaleY, float scaleZ,
	unsigned int VAO, int numVerticesVAO, unsigned int textura) {

	matUsada = glm::translate(matInicial, glm::vec3(posX, posY, posZ));

	matUsada = glm::rotate(matUsada, (aEjeY)*PI / 180.0f, glm::vec3(0, 1, 0));
	matUsada = glm::rotate(matUsada, (aEjeZ)*PI / 180.0f, glm::vec3(0, 0, 1));
	matUsada = glm::rotate(matUsada, (aEjeX)*PI / 180.0f, glm::vec3(1, 0, 0));

	matUsada = glm::scale(matUsada, glm::vec3(scaleX, scaleY, scaleZ));

	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matUsada));
	glBindTexture(GL_TEXTURE_2D, textura);

	if (VAO==NULL) {
		sphere.draw();
	}
	else {
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, numVerticesVAO);
	}
	return matUsada;
}

void mostrarMenu() {
	glm::mat4 transform = glm::mat4();
	transform = glm::translate(transform, glm::vec3(0, 2.5, 0.3));
	transform = glm::rotate(transform, (-20.0f) * PI / 180.0f, glm::vec3(1, 0, 0));
	transform = glm::rotate(transform, (-90.0f) * PI / 180.0f, glm::vec3(0, 0, 1));
	transform = glm::scale(transform, glm::vec3(1.5, 3.0, 1.0));

	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	glBindTexture(GL_TEXTURE_2D, texturaMenu);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glBindVertexArray(VAOcuadrado);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void pintarSuelo(unsigned int textura, unsigned int VAO) {
	glBindTexture(GL_TEXTURE_2D, textura);
	glm::mat4 transform;
	for (int i = -DIMENSIONSUELO-5; i <= DIMENSIONSUELO+5; i += 5) {
		for (int j = -DIMENSIONSUELO-5; j <= DIMENSIONSUELO+5; j += 5) {
			transform = glm::mat4();
			transform = glm::translate(transform, glm::vec3(i, -0.5f, j));
			transform = glm::rotate(transform, (float)(-90 * PI / 180.0f), glm::vec3(1.0, 0.0, 0.0));
			transform = glm::scale(transform, glm::vec3(5.0, 5.0, 1.0));

			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}
}

void pintarValla(unsigned int textura, unsigned int cubo, float posX, float posZ, float angulo) {
	glBindTexture(GL_TEXTURE_2D, textura);
	glm::mat4 transform, aux;
	transform = glm::mat4();
	//posicion inicial
	transform = glm::translate(transform, glm::vec3(posX, 0, posZ));
	transform = glm::rotate(transform, (angulo)*PI / 180.0f, glm::vec3(0, 1, 0));
	//postes
	for (int i = 0; i < 6; i += 5) {
		pintarObjeto(transform, aux, i, 0, 0, 0, 0, 0, 0.5, 8.0, 0.5, cubo, 36, textura);
	}
	//barrotes horizontales
	for (int i = 1; i <= 3; i++) {
		aux = glm::mat4();
		pintarObjeto(transform, aux, 5, i, 0, 0, 0, 0, 10.0, 0.4, 0.4, cubo, 36, textura);
	}
}

void rodearSueloConVallas() {
	for (int z = -DIMENSIONSUELO; z <= DIMENSIONSUELO; z += DIMENSIONSUELO * 2) {
		for (int x = -DIMENSIONSUELO; x <= DIMENSIONSUELO-10; x += 10) {
			pintarValla(texturaMadeira, VAOcubo, x, z, 0);
		}
	}
	for (int x = -DIMENSIONSUELO; x <= DIMENSIONSUELO; x += DIMENSIONSUELO * 2) {
		for (int z = -DIMENSIONSUELO+10; z <= DIMENSIONSUELO; z += 10) {
			pintarValla(texturaMadeira, VAOcubo, x, z, 90);
		}
	}
}

glm::mat4 pintarPeza(Pieza p, unsigned int textura, bool pintarEsfera, glm::mat4 transform) {
	glm::mat4 transCopia;
	transform = glm::translate(transform, glm::vec3(p.posX, p.posY, p.posZ));
	transform = glm::rotate(transform, p.aEjeY * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, p.aEjeX * PI / 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	transCopia = transform;
	transCopia = glm::scale(transform, glm::vec3(p.scaleX, p.scaleY, p.scaleZ));
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transCopia));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textura);
	//escollo entre pintar unha esfera ou un cubo
	if (pintarEsfera) sphere.draw();
	else {
		glBindVertexArray(p.VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	return transform;
}

bool podeSeguirMovendose(float posX, float posZ, float angulo, float dimX, float dimZ, int indexAI) {
	float* esquinas = obterEsquinasObjeto(posX, posZ, angulo, dimX, dimZ);
	bool resultado = true;
	float radio = sqrt(pow(dimX, 2) + pow(dimZ, 2));

	//comprobacion si sale do suelo
	if (
		puntoSaleDoSuelo(esquinas[DIANTEDERX], DIMENSIONSUELO) || puntoSaleDoSuelo(esquinas[DIANTEDERZ], DIMENSIONSUELO) ||
		puntoSaleDoSuelo(esquinas[DIANTEESQX], DIMENSIONSUELO) || puntoSaleDoSuelo(esquinas[DIANTEESQZ], DIMENSIONSUELO) ||
		puntoSaleDoSuelo(esquinas[ATRASDERX], DIMENSIONSUELO) || puntoSaleDoSuelo(esquinas[ATRASDERZ], DIMENSIONSUELO) ||
		puntoSaleDoSuelo(esquinas[ATRASESQX], DIMENSIONSUELO) || puntoSaleDoSuelo(esquinas[ATRASESQZ], DIMENSIONSUELO)
		) resultado = false;
	//colisiones coas casas
	if (resultado) {
		for (int i = 0; i < 4; i++) {
			if (casas[i].aEjeY != 0.0f) {
				if (circuloTocaRectanguloRotado(posX, posZ, radio, casas[i].posX, casas[i].posZ, 8.0, 8.0, casas[i].aEjeY)) {
					resultado = false;
					break;
				}
			}
			else {
				if (circuloTocaRectangulo(posX, posZ, radio, casas[i].posX, casas[i].posZ, 8.0, 8.0)) {
					resultado = false;
					break;
				}
			}
		}
	}
	//colisiones con outros tanques
	if (resultado) {
		for (int i = 0; i < TOTALTANQUESAI; i++) {
			if (!tanquesAI[i].debeSerPintado || i == indexAI) continue;
			if (circuloTocaRectangulo(posX, posZ, radio, tanquesAI[i].base.posX, tanquesAI[i].base.posZ, dimX * 2, dimZ * 2)) {
				resultado = false;
				break;
			}
		}
		if (indexAI != -1) {
			if (circuloTocaRectangulo(posX, posZ, radio, tanqueXogador.base.posX, tanqueXogador.base.posZ, dimX * 2, dimZ * 2)) {
				resultado = false;
			}
		}
	}
	//colisiones cos arboles
	if (resultado) {
		for (int i = 0; i < TOTALARBOLES; i++) {
			if (circuloTocaRectangulo(posX, posZ, radio, arboles[i].posX, arboles[i].posZ, 1.5, 1.5)) {
				resultado = false;
				break;
			}
		}
	}
	free(esquinas);
	return resultado;
}

void pintarTanque(Tanque* t, bool isAI, int indexAI) {
	if (!t->debeSerPintado) return;

	float posX, posZ;
	if (isAI) {
		t->baseTorreta.aEjeY += 3.0f / (indexAI + 3.0f);
		//cada certo tempo cambio o giro
		if (glfwGetTime() - t->tempoIniciMovimiento >= 1.5) {
			t->tempoIniciMovimiento = glfwGetTime();
			t->base.aEjeY += girosAI[rand() % 10];
		}
		t->velocidade = 0.03;
		posX = t->base.posX - (t->velocidade * sin((t->base.aEjeY)));
		posZ = t->base.posZ - (t->velocidade * cos((t->base.aEjeY)));
	}
	else {
		posX = t->base.posX - (t->velocidade * sin((t->base.aEjeY) * PI / 180.0f));
		posZ = t->base.posZ - (t->velocidade * cos((t->base.aEjeY) * PI / 180.0f));
	}

	if (podeSeguirMovendose(posX, posZ, t->base.aEjeY, 0.5f * t->base.scaleX, 0.5f * t->base.scaleZ, indexAI)) {
		t->base.posX = posX;
		t->base.posZ = posZ;
	}
	else {
		t->velocidade = -0.001f;
	}
	glm::mat4 transform = glm::mat4();
	transform = pintarPeza(t->base, texturaMetal, false, transform);
	transform = pintarPeza(t->baseTorreta, texturaMetal, false, transform);
	transform = pintarPeza(t->esferaBase, texturaMetal, true, transform);
	transform = pintarPeza(t->canon, texturaMetal, false, transform);
	pintarPeza(t->esferaCanon, texturaAcero, true, transform);
}

void pintarProyectil(Proyectil* p) {
	glm::mat4 transform;

	p->posX -= (p->velocidadeHorizontal * sin(p->anguloSalidaY * PI / 180.0f));
	p->posY += (p->velocidadeVertical * sin(p->anguloSalidaX * PI / 180.0f));
	p->posZ -= (p->velocidadeHorizontal * cos(p->anguloSalidaY * PI / 180.0f));

	for (int j = 0; j < TOTALTANQUESAI; j++) {
		if (!tanquesAI[j].debeSerPintado) continue;
		float* esquinas = obterEsquinasObjeto(
			tanquesAI[j].base.posX,
			tanquesAI[j].base.posZ,
			tanquesAI[j].base.aEjeY,
			0.5f * tanquesAI[j].base.scaleX,
			0.5f * tanquesAI[j].base.scaleZ
			);
		if (puntoColisionaObjeto(p->posX, p->posY, p->posZ, esquinas)) {
			tanquesAI[j].debeSerPintado = false;
			p->debeSerPintado = false;
		}
		free(esquinas);
	}

	transform = glm::mat4();
	transform = glm::translate(transform, glm::vec3(p->posX, p->posY, p->posZ));
	transform = glm::rotate(transform, p->anguloSalidaY * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, p->anguloSalidaX * PI / 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::scale(transform, glm::vec3(p->scaleX, p->scaleY, p->scaleZ));
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	sphere.draw();
}

void pintarProyectiles() {
	unsigned int colorLoc = glGetUniformLocation(shaderProgram, "objectColor");
	glUniform3f(colorLoc, 128.0 / 255.0, 128.0 / 255.0, 128.0 / 255.0);
	//pinto os proyectiles do tanque
	for (int i = 0; i < balasUsadas; i++) {
		if (!municion[i].debeSerPintado) continue;
		pintarProyectil(&municion[i]);
		if (glfwGetTime() - municion[i].timer > 2) municion[i].debeSerPintado = false;
	}
	//pinto os proyectiles do avion
	for (int i = 0; i < bombasUsadas; i++) {
		if (!bombas[i].debeSerPintado) continue;
		pintarProyectil(&bombas[i]);
		if (bombas[i].posY <= 0.0f) bombas[i].debeSerPintado = false;
	}
}

void pintarCasas() {
	glm::mat4 transform, aux;
	for (int i = 0; i < 4; i++) {
		transform = glm::mat4();
		transform = glm::translate(transform, glm::vec3(casas[i].posX, casas[i].posY, casas[i].posZ));
		transform = glm::rotate(transform, casas[i].aEjeY * PI / 180.0f, glm::vec3(0.0, 1.0, 0.0));

		//Pinto a casa
		pintarObjeto(transform, aux, 0, 0, 0, 0, 0, 0, 8.0, 8.0, 8.0, casas[i].VAOcasa, 42, texturaMadeira);

		//Pinto o tellado
		aux = glm::mat4();
		pintarObjeto(transform, aux, 0, 5.2, -2, -31.0, 0, 0, 11.0, 0.2, 5.0, casas[i].VAOtellado, 36, texturaTella);

		aux = glm::mat4();
		pintarObjeto(transform, aux, 0, 5.2, 2, 31.0, 0, 0, 11.0, 0.2, 5.0, casas[i].VAOtellado, 36, texturaTella);
	}
}

void pintarAvion(Avion* a, bool isAI) {
	float posX, posY, posZ;
	//se o controla AI
	if (isAI) {
		//giro e velocidade cte
		a->aEjeZ = 5;
		a->aEjeX = 0;
		a->velocidade = 0.08;
	}
	//axusto o giro dependendo do aladeo
	a->aEjeY += -a->aEjeZ / 50;

	//calculo as posicions
	if (a->posY >= 20.0f) posY = a->posY + (a->velocidade * sin((a->aEjeX) * PI / 180.0f));
	else {
		//se intenta baixar de 20 entonces estabilizase
		posY = 20.0f;
		if (a->aEjeX < 0) a->aEjeX++;
	}
	posX = a->posX - (a->velocidade * sin((a->aEjeY) * PI / 180.0f));
	posZ = a->posZ - (a->velocidade * cos((a->aEjeY) * PI / 180.0f));

	//se se sale do perimetro establecido entonces axusto o giro hasta que non se salga
	if (puntoSaleDoPerimetro(posX, posZ, 0, 0, RANGOVUELOAVION)) {
		if (a->aEjeZ >= 0 && a->aEjeZ < 50) a->aEjeZ++;
		else
			if (a->aEjeZ > -50) a->aEjeZ--;
	}
	else {
		//actualizo as posicions
		a->posX = posX;
		a->posY = posY;
		a->posZ = posZ;
	}

	glm::mat4 transform, aux;
	//posicion do avion
	transform = glm::mat4();
	transform = glm::translate(transform, glm::vec3(a->posX, a->posY, a->posZ));
	transform = glm::rotate(transform, (180.0f + a->aEjeY) * PI / 180.0f, glm::vec3(0, 1, 0));
	transform = glm::rotate(transform, (a->aEjeZ) * PI / 180.0f, glm::vec3(0, 0, 1));
	transform = glm::rotate(transform, (-a->aEjeX) * PI / 180.0f, glm::vec3(1, 0, 0));

	//fuselaje
	pintarObjeto(transform, aux, 0, 0, 0, 0, 0, 0, 1.0, 1.0, 5.0, NULL, NULL, texturaMetalAvion);

	//aleron
	aux = pintarObjeto(transform, aux, 0, 0.6, -4, 0, 0, 0, 4.0, 4.0, 4.0, a->VAOala, 36, texturaMetalAvion);

	//flaps dereita
	aux = pintarObjeto(aux, aux, 0, 0, 0, 0, 0, -90, 0.8, 1.0, 1.0, a->VAOala, 36, texturaMetalAvion);

	//flaps esquerda
	pintarObjeto(aux, aux, 0, 0, 0, 0, 0, 180, 1.0, 1.0, 1.0, a->VAOala, 36, texturaMetalAvion);

	//ala dereita
	aux = glm::mat4();
	aux = pintarObjeto(transform, aux, 0, -0.1, 0, 0, 0, -90, 2.0, 10.0, 5.0, a->VAOala, 36, texturaMetalAvion);

	//ala esquerda
	pintarObjeto(aux, aux, 0, 0, 0, 0, 0, 180, 1.0, 1.0, 1.0, a->VAOala, 36, texturaMetalAvion);

	//motor dereita
	aux = glm::mat4();
	pintarObjeto(transform, aux, 2, -0.3, 0.8, 0, 0, 0, 0.3, 0.3, 1.0, NULL, NULL, texturaAcero);

	//motor esquerda
	aux = glm::mat4();
	pintarObjeto(transform, aux, -2, -0.3, 0.8, 0, 0, 0, 0.3, 0.3, 1.0, NULL, NULL, texturaAcero);
}

void pintarArboles() {
	glm::mat4 transform, aux, aux2;

	for (int i = 0; i < TOTALARBOLES; i++) {
		transform = glm::mat4();
		transform = glm::translate(transform, glm::vec3(arboles[i].posX, arboles[i].posY, arboles[i].posZ));
		transform = glm::rotate(transform, (arboles[i].aEjeY) * PI / 180.0f, glm::vec3(0, 1, 0));

		//tronco
		pintarObjeto(transform, aux, 0, 0, 0, 0, 0, 0, 0.6, 8.0, 0.6, arboles[i].VAOcubo, 36, texturaTronco);

		//copa
		aux = glm::mat4();
		pintarObjeto(transform, aux, 0, 4, 0, 0, 0, 0, 2.5, 2.5, 2.5, arboles[i].VAOcubo, 36, texturaFollas);

		aux = glm::mat4();
		aux = glm::translate(transform, glm::vec3(0, 4, 0));

		//punta arriba
		aux2 = glm::mat4();
		pintarObjeto(aux, aux2, 0, 1.5, 0, 0, 0, 0, 1.5, 0.5, 1.5, arboles[i].VAOcubo, 36, texturaFollas);

		//salientes laterales
		for (int x = -1; x <= 1; x += 2) {
			for (int j = -1; j <= 1; j += 2) {
				pintarObjeto(aux, aux2, x, 0, j, 0, 0, 0, 1.0, 1.5, 1.0, arboles[i].VAOcubo, 36, texturaFollas);
			}
		}
	}
}

void pintarSkyBox() {
	glDepthFunc(GL_LEQUAL);
	glUseProgram(shaderSkyBox);
	glBindVertexArray(skybox.VAOcubo);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.textura);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}

//camaras
void camaraTerceira(float x, float y, float z, float anguloY, float lonxeXZcamara, float alturaCamara) {
	glViewport(0, 0, ancho, alto); //viewport
	glm::mat4 view = glm::mat4();
	//posicion camara
	posCamara[0] = (x + lonxeXZcamara * sin(anguloY * PI / 180.0f));
	posCamara[1] = y + alturaCamara;
	posCamara[2] = (z + lonxeXZcamara * cos(anguloY * PI / 180.0f));
	//posicion a donde apunta
	float apX = (x - lonxeXZcamara * sin(anguloY * PI / 180.0f));
	float apY = y + 0.5;
	float apZ = (z - lonxeXZcamara * cos(anguloY * PI / 180.0f));
	//actualizo a camara no shader
	glUseProgram(shaderProgram);
	view = glm::lookAt(
		glm::vec3(posCamara[0], posCamara[1], posCamara[2]),
		glm::vec3(apX, apY, apZ),
		glm::vec3(0.0f, 1.0f, 0.0f));
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection;
	projection = glm::mat4();
	projection = glm::ortho(-1000.0, 1000.0, -1000.0, 1000.0, 1.0, 1000.0);
	projection = glm::perspective(glm::radians(60.0f), (float)ancho / (float)alto, 1.0f, 1000.0f);
	unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//actualizo a camara no shader do skybox
	glUseProgram(shaderSkyBox);
	//a matriz de view e igual pero sin as translacions
	view = glm::mat4(glm::mat3(view));
	viewLoc = glGetUniformLocation(shaderSkyBox, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	projectionLoc = glGetUniformLocation(shaderSkyBox, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void camaraExterior() {
	glViewport(0, 0, ancho, alto);
	glm::mat4 view;
	view = glm::mat4();
	posCamara[0] = DIMENSIONSUELO * sin(glfwGetTime() * ARADIANES);
	posCamara[1] = 50.0f;
	posCamara[2] = DIMENSIONSUELO * cos(glfwGetTime() * ARADIANES);
	view = glm::lookAt(
		glm::vec3(posCamara[0], posCamara[1], posCamara[2]),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	glUseProgram(shaderProgram);
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection;
	projection = glm::mat4();
	projection = glm::ortho(-1000.0, 1000.0, -1000.0, 1000.0, 1.0, 1000.0);
	projection = glm::perspective(glm::radians(60.0f), (float)ancho / (float)alto, 1.0f, 1000.0f);
	unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glUseProgram(shaderSkyBox);
	//a matriz de view e igual pero sin as translacions
	view = glm::mat4(glm::mat3(view));
	viewLoc = glGetUniformLocation(shaderSkyBox, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	projectionLoc = glGetUniformLocation(shaderSkyBox, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

//display
void pintarDisplay() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glUseProgram(shaderProgram);
	transformLoc = glGetUniformLocation(shaderProgram, "model");
	//camaras
	switch (camaraActual) {
		case CAMARATANQUE:
			camaraTerceira(
				tanqueXogador.base.posX,
				tanqueXogador.base.posY,
				tanqueXogador.base.posZ,
				tanqueXogador.baseTorreta.aEjeY + tanqueXogador.base.aEjeY,
				3.5,
				3.0
			);
			break;
		case CAMARAAVION:
			camaraTerceira(
				avionXogador.posX,
				avionXogador.posY,
				avionXogador.posZ,
				avionXogador.aEjeY,
				10.0,
				5.0
			);
			break;
		case CAMARAEXT:
			camaraExterior();
			break;
	}
	//color dos obxectos
	glUseProgram(shaderProgram);
	unsigned int colorLoc = glGetUniformLocation(shaderProgram, "objectColor");
	glUniform3f(colorLoc, 0.0f, 0.0f, 0.0f);
	
	//mostro o menu se ainda non se empezou
	if (!xogoEmpezado)
		mostrarMenu();

	//pinto o entorno
	pintarSuelo(texturaSuelo, VAOcuadrado);
	rodearSueloConVallas();
	pintarCasas();
	pintarArboles();

	//pinto os tanques AI
	for (int i = 0; i < TOTALTANQUESAI; i++) {
		pintarTanque(&tanquesAI[i], true, i);
	}
	//pinto o tanque do xogador e o avion
	pintarTanque(&tanqueXogador, !estaseContralandoTanque, -1);
	pintarAvion(&avionXogador, estaseContralandoTanque);
	//pinto os proyectiles
	pintarProyectiles();

	//luces
	unsigned int lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
	glUniform3f(lightColorLoc, 0.6f, 0.6f, 0.6f);
	unsigned int lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
	glUniform3f(lightPosLoc, 100,50,100);
	unsigned int viewPos = glGetUniformLocation(shaderProgram, "viewPos");
	glUniform3f(viewPos, posCamara[0], posCamara[1], posCamara[2]);

	//por ultimo pinto o skybox
	pintarSkyBox();
}

//main
int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(ancho, alto, "Tanques Royale", NULL, NULL);
	if (window == NULL) {
		std::cout << "Fallo Crear Ventana" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glfwSetKeyCallback(window, keyCallback);
	openGlInit();
	//shaders
	shaderProgram = setShaders("shaderLuces.vert", "shaderLuces.frag");
	shaderSkyBox = setShaders("shaderSkyBox.vert", "shaderSkyBox.frag");
	//cargo as variables das texturas
	glUseProgram(shaderProgram);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);

	glUseProgram(shaderSkyBox);
	glUniform1i(glGetUniformLocation(shaderSkyBox, "skybox"), 0);

	//inicializaciones varias
	initTexturas();
	initTanques();
	initMunicion();
	initCasas();
	initAviones();
	initArboles();
	initSkyBox();
	VAOcuadrado = crearCuadrado();
	VAOcubo = crearCubo();
	texturaMenu = cargarTextura("menu.jpg");

	//poño a musica
	PlaySound(TEXT("musica.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);

	//bucle principal
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		pintarDisplay();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glBindVertexArray(0);
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window){
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

//teclas pulsadas
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {	
	//en caso de que non empezara o xogo solo comprobo se se pulsou algunha tecla
	if (!xogoEmpezado) {
		xogoEmpezado = true;
		return;
	}
	
	//controles tanque
	if (estaseContralandoTanque) {
		switch (key) {
			case 'A':
				tanqueXogador.base.aEjeY++;
				break;
			case 'D':
				tanqueXogador.base.aEjeY--;
				break;
			case 'W':
				tanqueXogador.velocidade += 0.005;
				if (tanqueXogador.velocidade == -0.01) tanqueXogador.velocidade = 0.0;
				break;
			case 'S':
				tanqueXogador.velocidade -= 0.005;
				if (tanqueXogador.velocidade == 0.01) tanqueXogador.velocidade = 0.0;
				break;
			case 'I':
				if (tanqueXogador.esferaBase.aEjeX > 0) tanqueXogador.esferaBase.aEjeX -= 0.5;
				break;
			case 'K':
				if (tanqueXogador.esferaBase.aEjeX < 20) tanqueXogador.esferaBase.aEjeX += 0.5;
				break;
			case 'J':
				tanqueXogador.baseTorreta.aEjeY++;
				break;
			case 'L':
				tanqueXogador.baseTorreta.aEjeY--;
				break;
			case 32:
				if (balasUsadas < MAXBALAS) {
					municion[balasUsadas].posX = tanqueXogador.base.posX;
					municion[balasUsadas].posZ = tanqueXogador.base.posZ;
					municion[balasUsadas].anguloSalidaX = tanqueXogador.esferaBase.aEjeX;
					municion[balasUsadas].anguloSalidaY = tanqueXogador.base.aEjeY + tanqueXogador.baseTorreta.aEjeY;
					municion[balasUsadas].timer = glfwGetTime();
					balasUsadas++;
				}
				break;
			case '1':
				camaraActual = CAMARATANQUE;
				estaseContralandoTanque = true;
				break;
			case '2':
				camaraActual = CAMARAAVION;
				estaseContralandoTanque = false;
				break;
			case '3':
				camaraActual = CAMARAEXT;
				break;
		}
	}
	else { //controles avion
		switch (key) {
			case 'A':
				if (avionXogador.aEjeZ > -50) avionXogador.aEjeZ--;
				break;
			case 'D':
				if (avionXogador.aEjeZ < 50) avionXogador.aEjeZ++;
				break;
			case 'W':
				if (avionXogador.aEjeX > -50) avionXogador.aEjeX--;
				break;
			case 'S':
				if (avionXogador.aEjeX < 50) avionXogador.aEjeX++;
				break;
			case 'Z':
				if (avionXogador.velocidade <= 0.48) avionXogador.velocidade += 0.08;
				break;
			case 'X':
				if (avionXogador.velocidade >= 0.16) avionXogador.velocidade -= 0.08;
				break;
			case 32:
				if (bombasUsadas < MAXBOMBAS) {
					bombas[bombasUsadas].posX = avionXogador.posX;
					bombas[bombasUsadas].posY = avionXogador.posY;
					bombas[bombasUsadas].posZ = avionXogador.posZ;
					bombas[bombasUsadas].anguloSalidaY = avionXogador.aEjeY;
					bombasUsadas++;
				}
				break;
			case '1':
				camaraActual = CAMARATANQUE;
				estaseContralandoTanque = true;
				break;
			case '2':
				camaraActual = CAMARAAVION;
				estaseContralandoTanque = false;
				break;
			case '3':
				camaraActual = CAMARAEXT;
				break;
		}
	}
}

//se cambia o tamano da ventana, adaptase o ancho e alto para o viewport
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	alto = height;
	ancho = width;
}