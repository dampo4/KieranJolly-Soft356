//////////////////////////////////////////////////////////////////////////////
//
//  Triangles.cpp
//
//////////////////////////////////////////////////////////////////////////////

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"
#include "LoadShaders.h"
#include <glm/glm.hpp> //includes GLM
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include <glm/ext/matrix_transform.hpp> // GLM: translate, rotate
#include <glm/ext/matrix_clip_space.hpp> // GLM: perspective and ortho 
#include <glm/gtc/type_ptr.hpp> // GLM: access to the value_ptr
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <sstream>
#include <list>
#include <iostream>
#include <fstream>
#include<vector>

#include "TexturedCube.h"
using namespace std;

// to use this example you will need to download the header files for GLM put them into a folder which you will reference in
// properties -> VC++ Directories -> libraries

enum VAO_IDs { Triangles, Indices, Colours, Tex, NumVAOs = 1 };
enum Buffer_IDs { ArrayBuffer, NumBuffers = 4 };
enum Attrib_IDs { vPosition = 0, cPosition = 1, tPosition = 2 };

GLuint  VAOs[NumVAOs];
GLuint  Buffers[NumBuffers];
GLuint texture1;
GLfloat vertices[8][3];
GLuint indices[12][3];
GLfloat  texture_coords[72];
GLfloat  temp[36];
vector<float> lvertices;
vector<float> ltexture;
vector<float> lfaces;
vector<float> ltexture_coords;
const GLuint  NumVertices = 36;

//----------------------------------------------------------------------------
//
// init
//
#define BUFFER_OFFSET(a) ((void*)(a))


void
init(void)
{
	glGenVertexArrays(NumVAOs, VAOs);
	glBindVertexArray(VAOs[Triangles]);

	ShaderInfo  shaders[] =
	{
		{ GL_VERTEX_SHADER, "media/triangles.vert" },
		{ GL_FRAGMENT_SHADER, "media/triangles.frag" },
		{ GL_NONE, NULL }
	};

	GLuint program = LoadShaders(shaders);
	glUseProgram(program);

	/*GLfloat vertices[][3] = {
		{0.5f,  0.5f, -0.5f},  //0 top right
		{0.5f, -0.5f, -0.5f},  //1 bottom right
		{-0.5f, -0.5f, -0.5f}, //2 bottom left
		{-0.5f,  0.5f, -0.5f},  //3 top left

		{0.5f,  0.5f, 0.5f},  //4 top right
		{0.5f, -0.5f, 0.5f},  //5 bottom right
		{-0.5f, -0.5f, 0.5f}, //6 bottom left
		{-0.5f,  0.5f, 0.5f}  //7 top left 
	};*/
	/*GLuint indices[][3] = {  // note that we start from 0!
		{0, 3, 1},  // first Triangle front
		{3, 2, 1},   // second Triangle
		
		{4, 7, 0 },
		{7, 3, 0 },
		
		{1, 2, 5 },
		{2, 6, 5 },
		
		{5, 4, 0 },
		{0, 1, 5 },
		
		{2, 3, 7 },
		{7, 6, 2 },
		
		{4, 5, 7 },  // first Triangle back
		{7, 5, 6 }   // second Triangle
	};*/

	GLfloat  colours[][4] = {
		{ 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f },  
		{ 1.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, 
		{ 0.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, 
	};
	/*GLfloat  texture_coords[] = {
		 0.340409, - 0.000303,
 0.593540, - 0.000303,
 0.593540,0.249793,
 0.340409, 0.249793,
 0.085962, 0.504830,
 0.341672, 0.504830,
 0.341672, 0.752417,
 0.085962, 0.752417,
 0.341137, 0.249562,
 0.593337, 0.249562,
 0.593337, 0.499562,
 0.341137, 0.499562,
 0.848302, 0.753039,
 0.595141, 0.753039,
 0.595141, 0.509132,
 0.848302, 0.509132,
 0.340409, 0.502321,
 0.594750, 0.502321,
 0.594750, 0.752417,
 0.340409, 0.752417,
 0.341044, 0.754030,
 0.592683, 0.754030,
 0.592683, 1.004126,
 0.341044, 1.004126,

		
	
	};*/



	glGenBuffers(NumBuffers, Buffers);
	
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Triangles]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[Indices]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	

	glVertexAttribPointer(vPosition, 3, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));
	
	//Colour Binding
	//glBindBuffer(GL_ARRAY_BUFFER, Buffers[Colours]);
	//glBufferStorage(GL_ARRAY_BUFFER, sizeof(colours), colours, 0);


	glVertexAttribPointer(cPosition, 4, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));

	//Texture Binding
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Tex]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texture_coords), texture_coords, GL_STATIC_DRAW);
	glVertexAttribPointer(tPosition, 2, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));

	loadTexture(texture1, "Texture.png");
	glUniform1i(glGetUniformLocation(program, "texture1"), 0);

	// creating the model matrix
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	model = glm::rotate(model, glm::radians(-40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));

	// creating the view matrix
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));

	// creating the projection matrix
	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3, 0.1f, 20.0f);

	// Adding all matrices up to create combined matrix
	glm::mat4 mvp = projection * view * model;


	//adding the Uniform to the shader
	int mvpLoc = glGetUniformLocation(program, "mvp");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	glEnableVertexAttribArray(vPosition);
	glEnableVertexAttribArray(cPosition); 
	glEnableVertexAttribArray(tPosition);
}

void loadTexture(GLuint &texture, std::string texturepath)
{
	// load and create a texture 
// -------------------------

// texture 1
// ---------
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	GLint width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load(texturepath.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}


//----------------------------------------------------------------------------
//
// display
//

void
display(void)
{
	static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	glClearBufferfv(GL_COLOR, 0, black);
	glClear(GL_COLOR_BUFFER_BIT);
	// bind textures on corresponding texture units
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glBindVertexArray(VAOs[Triangles]);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glDrawElements(GL_TRIANGLES, NumVertices, GL_UNSIGNED_INT, 0);

	
}
void
loadObjModel(void)
{
	string line;
	ifstream myFile("Creeper.obj", ios::binary);
	/*if (myFile.is_open())
	{
		GLubyte byte; // is 8bit and maps to char
		while (myFile >> byte)
		{
			cout << byte;

		}
	}*/
	while (getline(myFile, line))
	{

		//check v for vertices
		if (line.substr(0, 2) == "v ") {
			istringstream v(line.substr(2));
			cout << "here";
			float x, y, z;
			v >> x; v >> y; v >> z;
			lvertices.push_back(x);
			lvertices.push_back(y);
			lvertices.push_back(z);
		}
		//check for texture co-ordinate
		else if (line.substr(0, 2) == "vt") {

			std::istringstream v(line.substr(3));
			glm::vec2 tex;
			float U, V;
			v >> U; v >> V;
			ltexture.push_back(U);
			ltexture.push_back(V);
		}
		//check for faces
		else if (line.substr(0, 2) == "f ") {
			int a, b, c, d; //to store mesh index
			int A, B, C, D; //to store texture index
			int w, x, y, z;
			//std::istringstream v;
		  //v.str(line.substr(2));
			const char* chh = line.c_str();
			sscanf_s(chh, "f %i/%i/%i %i/%i/%i %i/%i/%i %i/%i/%i", &a, &A, &w, &b, &B, &x, &c, &C, &y, &d, &D, &z); //here it read the line start with f and store the corresponding values in the variables

			//v>>a;v>>b;v>>c;
			a--; b--; c--; d--;
			A--; B--; C--; D--;
			//std::cout<<a<<b<<c<<A<<B<<C;
			lfaces.push_back(a); ltexture_coords.push_back(A);
			lfaces.push_back(b); ltexture_coords.push_back(B);
			lfaces.push_back(c); ltexture_coords.push_back(C);
			lfaces.push_back(d); ltexture_coords.push_back(D);
		}

	}
	//the mesh data is finally calculated here
	//cout << lvertices.size();
	for (int i = 0; i < lvertices.size()/3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			vertices[i][j] = lvertices[(i * 3) + j];
			cout << vertices[i][j];
		}
	}
	for (int i = 0; i < lfaces.size()/4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (j == 0)
			{
				indices[i*2][j] = lfaces[(i * 4) + j];
				temp[i*2+j] = ltexture_coords[i * 4 + j];
				indices[i*2 + 1][j] = lfaces[(i * 4) + j];
				temp[i * 2 + 3 + j] = ltexture_coords[i * 4 + j];
			}
			if (j == 1)
			{
				indices[i*2][j + 1] = lfaces[(i * 4) + j];
				temp[i * 2 + 1 + j] = ltexture_coords[i * 4 + j];
			}
			if (j == 2)
			{
				indices[i*2][j - 1] = lfaces[(i * 4) + j];
				temp[i * 2 + j-1] = ltexture_coords[i * 4 + j];
				indices[i*2 + 1][j] = lfaces[(i * 4) + j];
				temp[i * 2 + 3 + j] = ltexture_coords[i * 4 + j];
			}
			if (j == 3)
			{
				indices[i*2 + 1][j - 2] = lfaces[(i * 4) + j];
				temp[i * 2 + 3 + j-2] = ltexture_coords[i * 4 + j];
				
			}
		}
	}
	for (size_t i = 0; i < 36; i++)
	{
		texture_coords[i*2] = ltexture[temp[i]*2];
		texture_coords[i*2+1] = ltexture[temp[i]*2+1];
	}

}
//----------------------------------------------------------------------------
//
// main
//


int
main(int argc, char** argv)
{
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(800, 600, "Textured Cube", NULL, NULL);

	glfwMakeContextCurrent(window);
	glewInit();
	loadObjModel();
	init();

	while (!glfwWindowShouldClose(window))
	{
		// uncomment to draw only wireframe 
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();

}