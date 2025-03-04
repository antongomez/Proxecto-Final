#include "encabezados/Figura.hpp"
#include "encabezados/definicions.h"

#include <glad.h>

#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION
#include "encabezados/tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// Singleton
static std::map<std::string, Figura*> fgCadrados;
static Figura* fgCubo = nullptr;
static std::map<std::string, Figura*> fgCargadas;

Figura::Figura(int tipo, unsigned int shaderProgram, std::string ruta) {
	this->tipo = tipo;
	this->shaderProgram = shaderProgram;

	if (tipo == FIGURA_CARGADA) {	// Neste caso a ruta correspondese cun OBJ
		cargarModelo(ruta);
		debuxaFiguraCargada();
	}
	else {							// Neste caso a ruta correspondese cunha textura
		debuxar();

		if (!ruta.empty()) {
			cargarTextura(ruta.c_str(), GL_RGB);
		}
	}

}

Figura* Figura::GetFigura(int tipo, unsigned int shaderProgram, std::string ruta) {
	Figura* fg = nullptr;

	switch(tipo) {
	case FIGURA_CADRADO:
		fg = fgCadrados[ruta];
		if (fg == nullptr) {
			fg = new Figura(tipo, shaderProgram, ruta);
			fgCadrados[ruta] = fg;
		}
		break;
	case FIGURA_CARGADA:
		fg = fgCargadas[ruta];
		if (fg == nullptr) {
			fg = new Figura(tipo, shaderProgram, ruta);
			fgCargadas[ruta] = fg;
		}
		break;
	case FIGURA_CUBO:

		if (fgCubo == nullptr) {
			fg = new Figura(tipo, shaderProgram, ruta);
			fgCubo = fg;
		}
		break;

	}

	return fg;
}

void Figura::debuxar() {
	// Neste caso so utilizaremos un VAO
	VAO = (unsigned int*)malloc(sizeof(unsigned int));

	switch (tipo) {
	case FIGURA_CADRADO:
		debuxaCadrado();
		break;
	case FIGURA_CUBO:
		debuxaCubo();
		break;
	default:	// Por defecto debuxamos un cubo
		debuxaCubo();
		break;
	}
}

void Figura::renderizar() {
	switch (tipo) {
	case FIGURA_CADRADO:
		renderizarCadrado();
		break;
	case FIGURA_CUBO:
		renderizarCubo();
		break;
	case FIGURA_CARGADA:
		renderizarFiguraCargada();
		break;
	}
}

void Figura::debuxaCadrado() {
	unsigned int VBO;


	float vertices[] = {
		-0.5f, -0.5f,  0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.0f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,

		 -0.5f, -0.5f,  0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.0f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f

	};

	glGenVertexArrays(1, VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(*VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Determinamos a posicion dos vertices no array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Determinamos a posicion dos vectores normais no array
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Determinamos a posicion das texturas no array
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);

}

void Figura::renderizarCadrado() {
	glBindVertexArray(*VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Figura::debuxaCubo() {
	unsigned int VBO, EBO;

	float n = 0.577350f;	// Este valor es 1/sqrt(3)

	float vertices[] = {
		-0.5f, -0.5f,  0.5f, -n, -n, n,
		 0.5f, -0.5f,  0.5f, n, -n, n,
		 0.5f,  0.5f,  0.5f, n, n, n,
		-0.5f,  0.5f,  0.5f, -n, n, n,

		-0.5f, -0.5f,  -0.5f, -n, -n, -n,
		 0.5f, -0.5f,  -0.5f, n, -n, -n,
		 0.5f,  0.5f,  -0.5f, n, n, -n,
		-0.5f,  0.5f,  -0.5f, -n, n, -n
	};

	unsigned int indices[] = {
		// Cara frontal
		0, 1, 2,
		0, 2, 3,

		// Cara de atr�s
		4, 7, 6,
		4, 6, 5,

		// Cara superior
		2, 6, 3,
		3, 6, 7,

		// Cara inferior
		0, 4, 1,
		1, 4, 5,

		// Cara dereita
		1, 5, 2,
		5, 6, 2,

		// Cara esquerda
		0, 3, 4,
		4, 3, 7

	};

	glGenVertexArrays(1, VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(*VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// posicion vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// normais
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Figura::renderizarCubo() {
	glBindVertexArray(*VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Figura::cargarTextura(const char* nombreTextura, int formato) {

	// Cargamos y creamos la textura
	glGenTextures(1, &textura);
	// Ahora todas las operaciones GL_TEXTURE_2D tienen efecto sobre este objeto texture
	glBindTexture(GL_TEXTURE_2D, textura);
	// Establecemos los par�metros de wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Establecemos los parametros de filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Cargamos la imagen, creamos la textura y generamos mipmaps
	int width, height, nrChannels;

	// Antes de cargar a imaxe invertimola para que quede ao dereito
	stbi_set_flip_vertically_on_load(1);

	unsigned char* data = stbi_load(nombreTextura, &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, formato, width, height, 0, formato, GL_UNSIGNED_BYTE, data);
	}
	else {
		std::cout << "Error ao cargar a textura" << std::endl;
	}
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

// Funcion que carga modelos .obj con TinyObjLoader. So necesita como parametro a ruta do .obj, asumese
// que o .mtl estara na mesma carpeta
void Figura::cargarModelo(std::string inputOBJfile) {
	tinyobj::ObjReaderConfig reader_config;

	// Atopa a ultima aparicion do caracter '/'
	size_t posicionUltimaBarra = inputOBJfile.find_last_of('/');

	// Extrae a subcadea dende o inicio ata a posicion da ultima barra
	reader_config.mtl_search_path = inputOBJfile.substr(0, posicionUltimaBarra + 1);

	tinyobj::ObjReader reader;

	// No caso de que haxa erros
	if (!reader.ParseFromFile(inputOBJfile, reader_config)) {
		if (!reader.Error().empty()) {
			std::cerr << "TinyObjReader: " << reader.Error();
		}
		exit(EXIT_FAILURE);
	}

	// No caso de que haxa warnings
	if (!reader.Warning().empty()) {
		std::cout << "TinyObjReader: " << reader.Warning();
	}

	auto& attrib = reader.GetAttrib();
	auto& shapes = reader.GetShapes();
	materiais.assign(reader.GetMaterials().begin(), reader.GetMaterials().end());

	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
			int idMaterial = shapes[s].mesh.material_ids[f];
			// No caso de que o obxecto cargado non tenha materiais o id sera -1.
			if (idMaterial == -1) {
				idMaterial = 0;
			}

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
				tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
				tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];
				vertices[idMaterial].push_back(glm::vec3(vx, vy, vz));

				// Check if `normal_index` is zero or positive. negative = no normal data
				if (idx.normal_index >= 0) {
					tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
					tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
					tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];

					normais[idMaterial].push_back(glm::vec3(nx, ny, nz));
				}

			}
			index_offset += fv;
		}
	}
}

void Figura::debuxaFiguraCargada() {
	// Reservamos memoria para debuxar as caras do obxecto unha a unha
	int numMateriais = vertices.size();

	VAO = (unsigned int*)malloc(numMateriais * sizeof(unsigned int));
	if (VAO == NULL) {
		std::cout << "Erro reservando memoria para os obxectos cargados. \n";
		exit(EXIT_FAILURE);
	}

	std::vector<unsigned int> VBO(numMateriais);
	std::vector<unsigned int> NBO(numMateriais); // numNormais = numVertices

	// Declarar un iterador 
	std::map<int, std::vector<glm::vec3>>::iterator iterador;

	// Percorremos os materiais do obxecto e debuxamos os vertices
	for (iterador = vertices.begin(); iterador != vertices.end(); ++iterador) {
		int idMaterial = iterador->first;

		std::vector<glm::vec3> verticesMaterial = iterador->second;
		std::vector<glm::vec3> normaisMaterial = normais[idMaterial];

		glGenVertexArrays(1, &VAO[idMaterial]);
		glBindVertexArray(VAO[idMaterial]);

		glGenBuffers(1, &VBO[idMaterial]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[idMaterial]);

		glBufferData(GL_ARRAY_BUFFER, verticesMaterial.size() * 3 * sizeof(tinyobj::real_t), &verticesMaterial[0], GL_STATIC_DRAW);

		// Determinamos a posicion dos vertices no array
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(tinyobj::real_t), (void*)0);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &NBO[idMaterial]);
		glBindBuffer(GL_ARRAY_BUFFER, NBO[idMaterial]);
		glBufferData(GL_ARRAY_BUFFER, normaisMaterial.size() * 3 * sizeof(tinyobj::real_t), &normaisMaterial[0], GL_STATIC_DRAW);

		// Determinamos a posicion das normais no array
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(tinyobj::real_t), (void*)0);
		glEnableVertexAttribArray(1);
	}
}

void Figura::renderizarFiguraCargada() {

	glm::vec3 cor(1.0f, 0, 0);

	// Percorremos os materiais do obxecto e renderizamos as caras
	for (std::map<int, std::vector<glm::vec3>>::iterator iter = vertices.begin();
		iter != vertices.end();
		++iter)
	{
		int idMaterial = iter->first;
		std::vector<glm::vec3> verticesMaterial = iter->second;

		tinyobj::material_t material = materiais[idMaterial];

		// Damoslle a cor do material ao obxecto
		unsigned int ambient = glGetUniformLocation(shaderProgram, "material.ambient");
		glUniform3fv(ambient, 1, material.diffuse);
		unsigned int diffuse = glGetUniformLocation(shaderProgram, "material.diffuse");
		glUniform3fv(diffuse, 1, material.diffuse);
		unsigned int specular = glGetUniformLocation(shaderProgram, "material.specular");
		glUniform3fv(specular, 1, material.specular);
		unsigned int shininess = glGetUniformLocation(shaderProgram, "material.shininess");
		glUniform1f(shininess, material.shininess);

		glBindVertexArray(VAO[idMaterial]);
		glDrawArrays(GL_TRIANGLES, 0, verticesMaterial.size());
	}
}