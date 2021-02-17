#include "FuncionsVAOs.h"


unsigned int crearCubo() {
	unsigned int VBO, EBO, VAO;

	float vertices[] = {
		 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f, 0.0,0.0,//diante
		  0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f, 0.0,1.0,
		  0.5f,  0.5f,  0.5f,  0.0f, 0.0f,1.0f, 1.0,1.0,
		  0.5f,  0.5f,  0.5f,  0.0f, 0.0f,1.0f, 1.0,1.0,
		 -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,1.0f, 1.0,0.0,
		 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f, 0.0,0.0,

		  0.5f,  0.5f, -0.5f,  0.0f, 0.0f,-1.0f, 1.0,0.0,//atras
		  0.5f, -0.5f, -0.5f,  0.0f, 0.0f,-1.0f, 0.0,0.0,
		 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,-1.0f, 0.0,1.0,
		  0.5f,  0.5f, -0.5f,  0.0f, 0.0f,-1.0f, 1.0,0.0,
		 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,-1.0f, 0.0,1.0,
		 -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,-1.0f, 1.0,1.0,

		  0.5f,  0.5f, -0.5f,  0.0f, 1.0f,1.0f, 1.0,1.0,//arriba
		 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,1.0f, 1.0,0.0,
		  0.5f,  0.5f,  0.5f,  0.0f, 1.0f,1.0f, 0.0,1.0,
		  0.5f,  0.5f,  0.5f,  0.0f, 1.0f,1.0f, 0.0,1.0,
		 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,1.0f, 1.0,0.0,
		 -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,1.0f, 0.0,0.0,

		  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,0.0f, 1.0,1.0,//abaixo
		 -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,0.0f, 1.0,0.0,
		 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,0.0f, 0.0,0.0,
		  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,0.0f, 1.0,1.0,
		 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,0.0f, 0.0,0.0,
		  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,0.0f, 0.0,1.0,

		 -0.5f,  0.5f,  0.5f,  -1.0f, 0.0f,0.0f, 1.0,1.0,//esquerda
		 -0.5f,  0.5f, -0.5f,  -1.0f, 0.0f,0.0f, 1.0,0.0,
		 -0.5f, -0.5f,  0.5f,  -1.0f, 0.0f,0.0f, 0.0,1.0,
		 -0.5f,  0.5f, -0.5f,  -1.0f, 0.0f,0.0f, 1.0,0.0,
		 -0.5f, -0.5f, -0.5f,  -1.0f, 0.0f,0.0f, 0.0,0.0,
		 -0.5f, -0.5f,  0.5f,  -1.0f, 0.0f,0.0f, 0.0,1.0,

		  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,0.0f, 1.0,0.0,//dereita
		  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,0.0f, 0.0,0.0,
		  0.5f,  0.5f, -0.5f,  1.0f, 0.0f,0.0f, 1.0,1.0,
		  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,0.0f, 0.0,0.0,
		  0.5f, -0.5f, -0.5f,  1.0f, 0.0f,0.0f, 0.0,1.0,
		  0.5f,  0.5f, -0.5f,  1.0f, 0.0f,0.0f, 1.0,1.0
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first.
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	return VAO;
}

unsigned int crearCuboSkyBox() {
	unsigned int VBO, VAO;
	//para o skybox non fan falta nin normales nin coordenadas de texturas
	float skyboxVertices[] = {      
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
	return VAO;
}

unsigned int crearCasita() {
	unsigned int VBO, EBO, VAO;

	float vertices[] = {
		 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f,  0.0,0.0,//diante
		  0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f,  0.0,1.0,
		  0.5f,  0.5f,  0.5f,  0.0f, 0.0f,1.0f,  1.0,1.0,
		  0.5f,  0.5f,  0.5f,  0.0f, 0.0f,1.0f,  1.0,1.0,
		 -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,1.0f,  1.0,0.0,
		 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f,  0.0,0.0,

		  0.5f,  0.5f, -0.5f,  0.0f, 0.0f,-1.0f, 1.0,0.0,//atras
		  0.5f, -0.5f, -0.5f,  0.0f, 0.0f,-1.0f, 0.0,0.0,
		 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,-1.0f, 0.0,1.0,
		  0.5f,  0.5f, -0.5f,  0.0f, 0.0f,-1.0f, 1.0,0.0,
		 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,-1.0f, 0.0,1.0,
		 -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,-1.0f, 1.0,1.0,

		  0.5f,  0.5f, -0.5f,  0.0f, 1.0f,0.0f,  1.0,1.0,//arriba
		 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,0.0f,  1.0,0.0,
		  0.5f,  0.5f,  0.5f,  0.0f, 1.0f,0.0f,  0.0,1.0,
		  0.5f,  0.5f,  0.5f,  0.0f, 1.0f,0.0f,  0.0,1.0,
		 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,0.0f,  1.0,0.0,
		 -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,0.0f,  0.0,0.0,

		  0.5f, -0.5f,  0.5f,  0.0f,-1.0f,0.0f,  1.0,1.0,//abaixo
		 -0.5f, -0.5f,  0.5f,  0.0f,-1.0f,0.0f,  1.0,0.0,
		 -0.5f, -0.5f, -0.5f,  0.0f,-1.0f,0.0f,  0.0,0.0,
		  0.5f, -0.5f,  0.5f,  0.0f,-1.0f,0.0f,  1.0,1.0,
		 -0.5f, -0.5f, -0.5f,  0.0f,-1.0f,0.0f,  0.0,0.0,
		  0.5f, -0.5f, -0.5f,  0.0f,-1.0f,0.0f,  0.0,1.0,

		 -0.5f,  0.5f,  0.5f, -1.0f, 0.0f,0.0f,  1.0,1.0,//esquerda
		 -0.5f,  0.5f, -0.5f, -1.0f, 0.0f,0.0f,  1.0,0.0,
		 -0.5f, -0.5f,  0.5f, -1.0f, 0.0f,0.0f,  0.0,1.0,
		 -0.5f,  0.5f, -0.5f, -1.0f, 0.0f,0.0f,  1.0,0.0,
		 -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,0.0f,  0.0,0.0,
		 -0.5f, -0.5f,  0.5f, -1.0f, 0.0f,0.0f,  0.0,1.0,

		  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,1.0f, 1.0,0.0,//dereita
		  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,1.0f, 0.0,0.0,
		  0.5f,  0.5f, -0.5f,  1.0f, 0.0f,1.0f, 1.0,1.0,
		  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,1.0f, 0.0,0.0,
		  0.5f, -0.5f, -0.5f,  1.0f, 0.0f,1.0f, 0.0,1.0,
		  0.5f,  0.5f, -0.5f,  1.0f, 0.0f,1.0f, 1.0,1.0,

		 -0.5f,  0.5f, -0.5f, -1.0f, 0.0f,0.0f, 0.0,0.0,//triangulo Esquerda
		 -0.5f,  0.5f,  0.5f, -1.0f, 0.0f,0.0f, 0.0,1.0,
		 -0.5f,  0.8f,  0.0f, -1.0f, 0.0f,0.0f, 1.0,0.0,

		  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,1.0f, 0.0,0.0,//triangulo Dereita
		  0.5f,  0.5f, -0.5f,  1.0f, 0.0f,1.0f, 0.0,1.0,
		  0.5f,  0.8f,  0.0f,  1.0f, 0.0f,1.0f, 1.0,0.0,
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	return VAO;
}

unsigned int crearAla() {
	unsigned int VBO, EBO, VAO;

	float vertices[] = {
		 -0.05f,  0.0f,  0.0f,  -1.0f, 0.0f,1.0f,  0.0,1.0, //esquerda
		 -0.05f,  0.0f,  0.5f,  -1.0f, 0.0f,1.0f,  0.0,0.0,
		 -0.05f,  0.5f,  0.0f,  -1.0f, 0.0f,1.0f,  1.0,1.0,
		 -0.05f,  0.5f, -0.2f,  -1.0f, 0.0f,1.0f,  1.0,1.0,
		 -0.05f,  0.0f,  0.0f,  -1.0f, 0.0f,1.0f,  1.0,0.0,
		 -0.05f,  0.5f,  0.0f,  -1.0f, 0.0f,1.0f,  0.0,0.0,
			
		  0.05f,  0.0f,  0.5f,  1.0f, 0.0f,0.0f,  0.0,0.0, //dereita
		  0.05f,  0.0f,  0.0f,  1.0f, 0.0f,0.0f,  0.0,1.0, 
		  0.05f,  0.5f,  0.0f,  1.0f, 0.0f,0.0f,  1.0,1.0,
		  0.05f,  0.0f,  0.0f,  1.0f, 0.0f,0.0f,  1.0,0.0,
		  0.05f,  0.5f, -0.2f,  1.0f, 0.0f,0.0f,  1.0,1.0,
		  0.05f,  0.5f,  0.0f,  1.0f, 0.0f,0.0f,  0.0,0.0,

		  0.05f,  0.0f,  0.5f,  0.0f, 0.0f,1.0f,  0.0,0.0, //diante
		  0.05f,  0.5f,  0.0f,  0.0f, 0.0f,1.0f,  0.0,1.0,
		 -0.05f,  0.0f,  0.5f,  0.0f, 0.0f,1.0f,  1.0,1.0,
		 -0.05f,  0.5f,  0.0f,  0.0f, 0.0f,1.0f,  1.0,0.0,
		 -0.05f,  0.0f,  0.5f,  0.0f, 0.0f,1.0f,  1.0,1.0,
		  0.05f,  0.5f,  0.0f,  0.0f, 0.0f,1.0f,  0.0,0.0,

		  0.05f,  0.5f, -0.2f,  0.0f, 0.0f,-1.0f,  0.0,0.0, //atras
		  0.05f,  0.0f,  0.0f,  0.0f, 0.0f,-1.0f,  0.0,1.0,
		 -0.05f,  0.5f, -0.2f,  0.0f, 0.0f,-1.0f,  1.0,1.0,
		 -0.05f,  0.0f,  0.0f,  0.0f, 0.0f,-1.0f,  1.0,0.0,
		 -0.05f,  0.5f, -0.2f,  0.0f, 0.0f,-1.0f,  1.0,1.0,
		  0.05f,  0.0f,  0.0f,  0.0f, 0.0f,-1.0f,  0.0,0.0,

		  0.05f,  0.5f,  0.0f,  0.0f, 1.0f,0.0f,  0.0,0.0, //arriba
		  0.05f,  0.5f, -0.2f,  0.0f, 1.0f,0.0f,  0.0,1.0,
		 -0.05f,  0.5f,  0.0f,  0.0f, 1.0f,0.0f,  1.0,1.0,
		 -0.05f,  0.5f, -0.2f,  0.0f, 1.0f,0.0f,  1.0,0.0,
		 -0.05f,  0.5f,  0.0f,  0.0f, 1.0f,0.0f,  1.0,1.0,
		  0.05f,  0.5f, -0.2f,  0.0f, 1.0f,0.0f,  0.0,0.0,

		 -0.05f,  0.0f,  0.5f,  0.0f, -1.0f,0.0f,  0.0,0.0, //abaixo
		 -0.05f,  0.0f,  0.0f,  0.0f, -1.0f,0.0f,  0.0,1.0,
		  0.05f,  0.0f,  0.5f,  0.0f, -1.0f,0.0f,  1.0,1.0,
		  0.05f,  0.0f,  0.0f,  0.0f, -1.0f,0.0f,  1.0,0.0,
		  0.05f,  0.0f,  0.5f,  0.0f, -1.0f,0.0f,  1.0,1.0,
		 -0.05f,  0.0f,  0.0f,  0.0f, -1.0f,0.0f,  0.0,0.0,
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	return VAO;
}

unsigned int crearCuadrado() {
	unsigned int VBO, EBO, VAO;

	
	float vertices[] = {
		 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f, 0.0,0.0,
		  0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f, 0.0,1.0,
		  0.5f,  0.5f,  0.5f,  0.0f, 0.0f,1.0f, 1.0,1.0,
		  0.5f,  0.5f,  0.5f,  0.0f, 0.0f,1.0f, 1.0,1.0,
		 -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,1.0f, 1.0,0.0,
		 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f, 0.0,0.0,
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	return VAO;
}


