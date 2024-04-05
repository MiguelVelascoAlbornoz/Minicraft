#include "Face.h"
#include <GL/glew.h>


Face::Face(GLfloat* vertices/*SIEMPRE EN LAS AGUJAS DEL RELOJ*/) {
	GLuint indexData[]{
		0,1,3,
		1,2,3
	};
	shouldDestroy = false;
	//creacion del VBO
	GLuint VBO;
	glGenBuffers(1/*queremos crear un solo buffer*/, &VBO/*esera el identificador del buffer*/);// le decimos a opengl que vamos a configurar VBO
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {

		std::cerr << "Error VBO: " << error << std::endl;
		this->shouldDestroy = true;
	}
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//le decimos al VBO que significa lo que le mandamos con el target (el target dice como deven ser usados estos datos)
	// GL_ARRAY_BUFFER (para vertices y atributos de vertices), le pasamos el array de los datos de los vertices y
	//le decimos cuanto pesa este array
	glBufferData(GL_ARRAY_BUFFER/*el target*/, sizeof(GLfloat) * 24 /*el tamaño del array*/, vertices/* el array*/, GL_STATIC_DRAW);

	//Creacion del VAO
	glGenVertexArrays(1, &VAO);
	 error = glGetError();
	if (error != GL_NO_ERROR) {
		
		std::cerr << "Error VAO: " << error << std::endl;
		this->shouldDestroy = true;
	}
	glBindVertexArray(VAO);
	
	//Le configuramos como tiene que ser leido
	glVertexAttribPointer(0/*a que layout se envia*/, 3/*cuanto mide cada grupo*/, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT)/*cada cuanto se repite*/, (GLvoid*)0/*desde donde comenzamos*/);

	//creamos el index Buffer || es usado para decirle a opengl que cree otro vertex buffer
	//con las coordenadas que tomo del vertex buffer solo que reorganizadas y copiadas
	//segun las instrucciones que le demos con el indexbuffer (en pocas palabras, el index buffer es el indicie
	//de como usar el VertexBuffer)
	GLuint indexBuffer;
	glGenBuffers(1/*queremos crear un solo buffer*/, &indexBuffer/*ese sera el identificador del buffer*/);
	 error = glGetError();
	if (error != GL_NO_ERROR) {

		std::cerr << "IndexBuffer: " << error << std::endl;
		this->shouldDestroy = true;
	}
	// le decimos a opengl que vamos a configurar el buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	//le decimos a opengl que queremos que haga con este buffer con el target
	//GL_ELEMENT_ARRAY_BUFFER (le dice que esto es un index buffer),
	//y le pasamos los datos del buffer, junto con cuanto mide en bytes este array
	glBufferData(GL_ELEMENT_ARRAY_BUFFER/*el target*/, sizeof(indexData)/*el tamaño del array*/, indexData/* el array*/, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &indexBuffer);

}
Face::~Face() {
	
	glDeleteBuffers(1, &VAO);
}
void Face::draw() {
	
	glBindVertexArray(VAO);
	(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));
}
