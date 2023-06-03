#include "encabezados/Suelo.hpp"
#include "encabezados/definicions.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glad.h>

Suelo::Suelo(glm::vec3 posicion, float escala, float* limites, unsigned int shaderProgram, int tipoFigura, std::string rutaTextura, std::string rutaTexturaMuro) {
	this->posicion = posicion;
	this->escala = escala;


	this->limites[0] = limites[0];
	this->limites[1] = limites[1];

	this->shaderProgram = shaderProgram;

	this->fg = new Figura(tipoFigura, shaderProgram);
	this->fg->cargarTextura(rutaTextura.c_str(), GL_RGB);

	this->fgMuro = new Figura(tipoFigura, shaderProgram);
	this->fgMuro->cargarTextura(rutaTexturaMuro.c_str(), GL_RGB);
}

void Suelo::renderizarSuelo() {

	float escalado = (float)1.0 / escala;

	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");

	glBindTexture(GL_TEXTURE_2D, fg->textura);

	for (float i = limites[0]; i <= limites[1]; i += (float)(1.0 / escala)) {
		for (float j = limites[0]; j <= limites[1]; j += (float)(1.0 / escala)) {
			matrizModelo = glm::mat4();
			matrizModelo = glm::translate(matrizModelo, glm::vec3(i, 0, j) + posicion);
			matrizModelo = glm::rotate(matrizModelo, (float)(-PI / 2.0), glm::vec3(1.0, 0.0, 0.0));
			matrizModelo = glm::scale(matrizModelo, glm::vec3(escalado));


			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrizModelo));

			fg->renderizar();
		}
	}

	float alturaMuro = -3.0f;

	glBindTexture(GL_TEXTURE_2D, fgMuro->textura);

	// Debuxamos a parte de atras do murito
	for (float i = limites[0]; i <= limites[1]; i += (float)(1.0 / escala)) {

		matrizModelo = glm::mat4();
		matrizModelo = glm::translate(matrizModelo, glm::vec3(i, alturaMuro, limites[0]) + posicion);
		matrizModelo = glm::scale(matrizModelo, glm::vec3(escalado));

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrizModelo));
		fg->renderizar();

	}

	// Debuxamos a parte de diante do murito
	for (float i = limites[0]; i <= limites[1]; i += (float)(1.0 / escala)) {

		matrizModelo = glm::mat4();
		matrizModelo = glm::translate(matrizModelo, glm::vec3(i, alturaMuro, limites[1]) + posicion);
		matrizModelo = glm::rotate(matrizModelo, (float)PI, glm::vec3(0.0, 1.0, 0.0));
		matrizModelo = glm::scale(matrizModelo, glm::vec3(escalado));

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrizModelo));
		fg->renderizar();

	}

	// Debuxamos a parte da esquerda do murito
	for (float i = limites[0]; i <= limites[1]; i += (float)(1.0 / escala)) {

		matrizModelo = glm::mat4();
		matrizModelo = glm::translate(matrizModelo, glm::vec3(limites[0], alturaMuro, i) + posicion);
		matrizModelo = glm::rotate(matrizModelo, (float)(PI / 2.0f), glm::vec3(0.0, 1.0, 0.0));
		matrizModelo = glm::scale(matrizModelo, glm::vec3(escalado));

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrizModelo));
		fg->renderizar();

	}

	// Debuxamos a parte da dereita do murito
	for (float i = limites[0]; i <= limites[1]; i += (float)(1.0 / escala)) {

		matrizModelo = glm::mat4();
		matrizModelo = glm::translate(matrizModelo, glm::vec3(limites[1], alturaMuro, i) + posicion);
		matrizModelo = glm::rotate(matrizModelo, (float)(-PI / 2.0f), glm::vec3(0.0, 1.0, 0.0));
		matrizModelo = glm::scale(matrizModelo, glm::vec3(escalado));

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrizModelo));
		fg->renderizar();

	}
}