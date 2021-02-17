#include "Colisiones.h"

bool circuloTocaRectangulo(float posXcirc, float posZcirc, float radioCirc, float posXrect, float posZrect, float ancho, float alto) {
    float distanciaCirculoX = abs(posXcirc - posXrect);
    float distanciaCirculoZ = abs(posZcirc - posZrect);

    if (distanciaCirculoX > (ancho / 2 + radioCirc)) { return false; }
    if (distanciaCirculoZ > (alto / 2 + radioCirc)) { return false; }

    if (distanciaCirculoX <= (ancho / 2)) { return true; }
    if (distanciaCirculoZ <= (alto / 2)) { return true; }

    float distanciaCuadradoEsquina =
        pow((distanciaCirculoX - ancho / 2), 2) +
        pow((distanciaCirculoZ - alto / 2), 2);

    return (distanciaCuadradoEsquina <= pow(radioCirc, 2));
}

bool circuloTocaRectanguloRotado(float posXcirc, float posZcirc, float radioCirc, float posXrect, float posZrect, float ancho, float alto,float angulo) {

       
    float distanciaCirculoX = abs(posXcirc - posXrect);
    float distanciaCirculoZ = abs(posZcirc - posZrect);

    if (distanciaCirculoX > ((ancho / 2)*cos(angulo*ARADIANES) + radioCirc)) { return false; }
    if (distanciaCirculoZ > ((alto / 2)*sin(angulo * ARADIANES) + radioCirc)) { return false; }

    if (distanciaCirculoX <= ((ancho / 2) * cos(angulo * ARADIANES))) { return true; }
    if (distanciaCirculoZ <= ((alto / 2) * sin(angulo * ARADIANES))) { return true; }

    float distanciaCuadradoEsquina =
        pow((distanciaCirculoX - (ancho / 2) * cos(angulo * ARADIANES)), 2) +
        pow((distanciaCirculoZ - (alto / 2) * sin(angulo * ARADIANES)), 2);

    return (distanciaCuadradoEsquina <= pow(radioCirc, 2));
}

float* obterEsquinasObjeto(float posX, float posZ, float angulo, float dimX, float dimZ) {
	float* esquinas = (float*)malloc(8 * sizeof(float));
	float diagonal = sqrt(pow(dimX, 2) + pow(dimZ, 2));
	esquinas[DIANTEDERX] = posX + diagonal * sin((angulo + 45.0f) * PI / 180.0f);
	esquinas[DIANTEDERZ] = posZ + diagonal * cos((angulo + 45.0f) * PI / 180.0f);
	esquinas[DIANTEESQX] = posX + diagonal * sin((angulo - 45.0f) * PI / 180.0f);
	esquinas[DIANTEESQZ] = posZ + diagonal * cos((angulo - 45.0f) * PI / 180.0f);
	esquinas[ATRASDERX] = posX + diagonal * sin((angulo + 135.0f) * PI / 180.0f);
	esquinas[ATRASDERZ] = posZ + diagonal * cos((angulo + 135.0f) * PI / 180.0f);
	esquinas[ATRASESQX] = posX + diagonal * sin((angulo - 135.0f) * PI / 180.0f);
	esquinas[ATRASESQZ] = posZ + diagonal * cos((angulo - 135.0f) * PI / 180.0f);
	return esquinas;
}

bool puntoSaleDoSuelo(float punto, float dimensionSuelo) {
	if (punto > dimensionSuelo || punto < -dimensionSuelo) return true;
	return false;
}

bool puntoColisionaObjeto(float posX, float posY, float posZ, float* esquinas) {
	float altura = 5;
	if (
		posX <= esquinas[DIANTEDERX] &&
		posX >= esquinas[DIANTEESQX] &&
		posZ >= esquinas[ATRASDERZ] &&
		posZ <= esquinas[DIANTEDERZ] &&
		posY >= 0 && posY <= altura
	) return true;
	return false;
}

bool puntoSaleDoPerimetro(float posX, float posZ, float posXcirc, float posZcirc, float radio) {
    float distanciaCirculoX = abs(posX - posXcirc);
    float distanciaCirculoZ = abs(posZ - posZcirc);

    if (distanciaCirculoX > radio) return true;
    if (distanciaCirculoZ > radio) return true;

	return false;
}