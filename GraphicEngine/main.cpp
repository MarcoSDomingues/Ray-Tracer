///////////////////////////////////////////////////////////////////////
//
// P3D Course
// (c) 2016 by group 9
//
///////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <limits>
#include <cmath> 

#include "Dependencies/glew/glew.h"
#include "Dependencies/freeglut/freeglut.h"

#include "Scene.h"
#include "Vectors.h"
#include "Utils.h"

#define CAPTION "Ray Tracer"

#define VERTEX_COORD_ATTRIB 0
#define COLOR_ATTRIB 1

#define DELTA 1.0001
#define THRESH 0.3
#define MAX_DEPTH 6
#define SHADOW_MAX_RAYS 25

Color shadowColor;

// Points defined by 2 attributes: positions which are stored in vertices array and colors which are stored in colors array
float *colors;
float *vertices;

int size_vertices;
int size_colors;

GLfloat m[16];  //projection matrix initialized by ortho function

GLuint VaoId;
GLuint VboId[2];

GLuint VertexShaderId, FragmentShaderId, ProgramId;
GLint UniformId;

Scene* scene = NULL;
int RES_X, RES_Y;

/* Draw Mode: 0 - point by point; 1 - line by line; 2 - full frame */
int draw_mode=1;

int WindowHandle = 0;

Ray firstIt[3];
Ray secondIt[4];

///////////////////////////////////////////////////////////////////////  RAY-TRACE SCENE

float randomNumber() {
	float r = rand() % 10;
	float random = r / (r + 2);
	return roundf(random * 10) / 10;
}

Color rayTracing(Ray ray, int depth, float RefrIndex)
{
	Color color;

	bool reflectiveObject = true;

	int objID = 0;
	Vector3 hitPoint, hit;
	bool has_collision = false;

	float distance = 0.0f;
	Vector3 normal;
	Vector3 normalHitPoint;

	//std::cout << ray.direction.x << " " << ray.direction.y << "  " << ray.direction.z << std::endl;
    //intersect ray with all objects
	float maxDistance = -1;
	for (int i = 0; i < scene->objects.size(); i++) {
		if (scene->objects[i]->checkIntersection(ray, hit, distance, normal)) {
			if (distance <= maxDistance || maxDistance == -1) {
				maxDistance = distance;
				hitPoint = hit;
				has_collision = true;
				objID = i;
				normalHitPoint = normal;
			}
		}
	}

	if (!has_collision) {
		color.r = scene->background[0];
		color.g = scene->background[1];
		color.b = scene->background[2];
		return color;
	}
	else {

		Material mat = scene->objects[objID]->material;

		color.r = 0.0f;
		color.g = 0.0f;
		color.b = 0.0f;

		shadowColor.r = 0.0f;
		shadowColor.g = 0.0f;
		shadowColor.b = 0.0f;

		//compute normal at hitpoint
		Vector3 n = normalHitPoint;
		Vector3 V = (scene->camera.eye - hitPoint).normalize();

		for (int i = 0; i < scene->lights.size(); i++) {
			Light light = scene->lights[i];
			Vector3 lightPos = Vector3(light.x, light.y, light.z);

			Vector3 a = Vector3(1.0f, 0.0f, 0.0f);
			Vector3 b = Vector3(0.0f, 1.0f, 0.0f);

			for (int k = 0; k < SHADOW_MAX_RAYS; k++) {
				float j = 0.1;
				Vector3 LP = lightPos + (randomNumber() * a) + (randomNumber() * b);

				Vector3 L = (LP - hitPoint).normalize();
				Vector3 Ln = (L.dot(n) * n);

				Vector3 h = Ln - L;
				Vector3 R = Ln + h;

				bool in_dark = false;

				float fs = 1.0f;

				if (L.dot(n) > 0.0) {
					Ray shadowFeeler;
					shadowFeeler.origin = hitPoint * DELTA;
					shadowFeeler.direction = L;


					Vector3 hitShadow;
					float dist;

					for (int k = 0; k < scene->objects.size(); k++) {
						if (k != objID) {
							if (scene->objects[k]->checkIntersection(shadowFeeler, hitShadow, dist, normal)) {
								in_dark = true;
							}
						}
					}

					if (in_dark) fs = 0.0f;

					shadowColor.r += fs * ((light.r * (mat.r * mat.kd) * n.dot(L)) + (light.r * (mat.r * mat.ks) * h.dot(n)));
					shadowColor.g += fs * ((light.g * (mat.g * mat.kd) * n.dot(L)) + (light.g * (mat.g * mat.ks) * h.dot(n)));
					shadowColor.b += fs * ((light.b * (mat.b * mat.kd) * n.dot(L)) + (light.b * (mat.b * mat.ks) * h.dot(n)));

					in_dark = false;
				}
			}

			color.r = shadowColor.r / SHADOW_MAX_RAYS;
			color.g = shadowColor.g / SHADOW_MAX_RAYS;
			color.b = shadowColor.b / SHADOW_MAX_RAYS;
		}

		if (depth >= MAX_DEPTH)	return color;

		if (mat.ks > 0.0) {
			//compute input cosine and sine vectors
			Vector3 Rr = 2 * (V.dot(n)) * n - V;

			Ray reflectedRay;
			reflectedRay.origin = hitPoint * DELTA;
			reflectedRay.direction = Rr;

			Color rColor = rayTracing(reflectedRay, depth + 1, 1.0);
			color.r += rColor.r * mat.ks;
			color.g += rColor.g * mat.ks;
			color.b += rColor.b * mat.ks;
		}

		if (mat.t > 0.0) {
			Vector3 Vt = V.dot(n)*n - V;
			float sinI = Vt.length();
			float sinT = (RefrIndex / mat.iof) * sinI;

			float cosT = sqrt(1 - pow(sinT, 2));

			Vector3 t = (Vt / sinI).normalize();

			Vector3 Rt = sinT * t + cosT * -n;

			Ray refractedRay;
			refractedRay.origin = hitPoint * DELTA;
			refractedRay.direction = Rt;

			Color tColor = rayTracing(refractedRay, depth + 1, 1.0);
			color.r += tColor.r * mat.t;
			color.g += tColor.g * mat.t;
			color.b += tColor.b * mat.t;
		}

		return color;
	}
}

/////////////////////////////////////////////////////////////////////// ERRORS

bool isOpenGLError() {
    bool isError = false;
    GLenum errCode;
    const GLubyte *errString;
    while ((errCode = glGetError()) != GL_NO_ERROR) {
        isError = true;
        errString = gluErrorString(errCode);
        std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
    }
    return isError;
}

void checkOpenGLError(std::string error)
{
    if(isOpenGLError()) {
        std::cerr << error << std::endl;
        exit(EXIT_FAILURE);
    }
}
/////////////////////////////////////////////////////////////////////// SHADERs
const GLchar* VertexShader =
{
    "#version 330 core\n"
    
    "in vec2 in_Position;\n"
    "in vec3 in_Color;\n"
    "uniform mat4 Matrix;\n"
    "out vec4 color;\n"
    
    "void main(void)\n"
    "{\n"
    "	vec4 position = vec4(in_Position, 0.0, 1.0);\n"
    "	color = vec4(in_Color, 1.0);\n"
    "	gl_Position = Matrix * position;\n"
    
    "}\n"
};

const GLchar* FragmentShader =
{
    "#version 330 core\n"
    
    "in vec4 color;\n"
    "out vec4 out_Color;\n"
    
    "void main(void)\n"
    "{\n"
    "	out_Color = color;\n"
    "}\n"
};

void createShaderProgram()
{
    VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShaderId, 1, &VertexShader, 0);
    glCompileShader(VertexShaderId);
    
    FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShaderId, 1, &FragmentShader, 0);
    glCompileShader(FragmentShaderId);
    
    ProgramId = glCreateProgram();
    glAttachShader(ProgramId, VertexShaderId);
    glAttachShader(ProgramId, FragmentShaderId);
    
    glBindAttribLocation(ProgramId, VERTEX_COORD_ATTRIB, "in_Position");
    glBindAttribLocation(ProgramId, COLOR_ATTRIB, "in_Color");
    
    glLinkProgram(ProgramId);
    UniformId = glGetUniformLocation(ProgramId, "Matrix");
    
    checkOpenGLError("ERROR: Could not create shaders.");
}

void destroyShaderProgram()
{
    glUseProgram(0);
    glDetachShader(ProgramId, VertexShaderId);
    glDetachShader(ProgramId, FragmentShaderId);
    
    glDeleteShader(FragmentShaderId);
    glDeleteShader(VertexShaderId);
    glDeleteProgram(ProgramId);
    
    checkOpenGLError("ERROR: Could not destroy shaders.");
}
/////////////////////////////////////////////////////////////////////// VAOs & VBOs
void createBufferObjects()
{
    glGenVertexArrays(1, &VaoId);
    glBindVertexArray(VaoId);
    glGenBuffers(2, VboId);
    glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
    
    /* N„o È necess·rio fazer glBufferData, ou seja o envio dos pontos para a placa gr·fica pois isso
     È feito na drawPoints em tempo de execuÁ„o com GL_DYNAMIC_DRAW */
    
    glEnableVertexAttribArray(VERTEX_COORD_ATTRIB);
    glVertexAttribPointer(VERTEX_COORD_ATTRIB, 2, GL_FLOAT, 0, 0, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, VboId[1]);
    glEnableVertexAttribArray(COLOR_ATTRIB);
    glVertexAttribPointer(COLOR_ATTRIB, 3, GL_FLOAT, 0, 0, 0);
    
    // unbind the VAO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(VERTEX_COORD_ATTRIB);
    glDisableVertexAttribArray(COLOR_ATTRIB);
    checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
}

void destroyBufferObjects()
{
    glDisableVertexAttribArray(VERTEX_COORD_ATTRIB);
    glDisableVertexAttribArray(COLOR_ATTRIB);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    glDeleteBuffers(1, VboId);
    glDeleteVertexArrays(1, &VaoId);
    checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
}

void drawPoints()
{
    glBindVertexArray(VaoId);
    glUseProgram(ProgramId);
    
    glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
    glBufferData(GL_ARRAY_BUFFER, size_vertices, vertices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, VboId[1]);
    glBufferData(GL_ARRAY_BUFFER, size_colors, colors, GL_DYNAMIC_DRAW);
    
    glUniformMatrix4fv(UniformId, 1, GL_FALSE, m);
    glDrawArrays(GL_POINTS, 0, RES_X*RES_Y);
    glFinish();
    
    glUseProgram(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    checkOpenGLError("ERROR: Could not draw scene.");
}

/////////////////////////////////////////////////////////////////////// CALLBACKS

// Render function by primary ray casting from the eye towards the scene's objects

Color averageColor(Color c1, Color c2, Color c3, Color c4) {
	Color color;
	color.r = (c1.r + c2.r + c3.r + c4.r) / 4.0f;
	color.g = (c1.g + c2.g + c3.g + c4.g) / 4.0f;
	color.b = (c1.b + c2.b + c3.b + c4.b) / 4.0f;
	return color;
}


bool compareColors(Color c1, Color c2) {
	float diff = std::abs(c1.r - c2.r) + std::abs(c1.g - c2.g) + std::abs(c1.b - c2.b);
	if (diff < 3.0f) {
		return true;
	}
	return false;
}

int auxN = 0; 

Color adaptativeSuperSampling(int x, int y, int n) {

	Ray ray;
	Color color[4];

	float delta = 1.0f / (n + 1.0f);

	//monteCarlo
	ray = scene->camGetPrimaryRay(x, y);
	color[0] = rayTracing(ray, 1, 1.0);

	ray = scene->camGetPrimaryRay(x + delta, y);
	color[1] = rayTracing(ray, 1, 1.0);

	ray = scene->camGetPrimaryRay(x + delta, y + delta);
	color[2] = rayTracing(ray, 1, 1.0);

	ray = scene->camGetPrimaryRay(x, y + delta);
	color[3] = rayTracing(ray, 1, 1.0);

	if (compareColors(color[0], color[1])) {
		if (compareColors(color[2], color[3])) {
			if (compareColors(color[0], color[2]))
				return averageColor(color[0], color[1], color[2], color[3]);
		}
	}

	n++;

	if (auxN != n) { //para debug
		auxN = n;
		std::cout << n << std::endl;
	}

	if (n == 3) {
		std::cout << "fim\n";
		return averageColor(color[0], color[1], color[2], color[3]);
	}

	float nextDelta = 1 / (n + 1);
	color[0] = adaptativeSuperSampling(x, y, n);
	color[1] = adaptativeSuperSampling(x + nextDelta, y, n);
	color[2] = adaptativeSuperSampling(x, y+ nextDelta, n);
	color[3] = adaptativeSuperSampling(x + nextDelta, y + nextDelta, n);
	return averageColor(color[0], color[1], color[2], color[3]);

}

void renderScene()
{
    int index_pos=0;
    int index_col=0;
    
	Ray ray;
	Color color;

    for (int y = 0; y < RES_Y; y++)
    {
        for (int x = 0; x < RES_X; x++)
        {

            //YOUR 2 FUNTIONS:
            
			color = adaptativeSuperSampling(x, y, 0);
			//ray = scene->camGetPrimaryRay(x, y);
			//color = rayTracing(ray, 1, 1.0);

            
            vertices[index_pos++]= (float)x;
            vertices[index_pos++]= (float)y;
            colors[index_col++]= (float)color.r;
            colors[index_col++]= (float)color.g;
            colors[index_col++]= (float)color.b;
            
            if(draw_mode == 0) {  // desenhar o conte˙do da janela ponto a ponto
                drawPoints();
                index_pos=0;
                index_col=0;
            }
        }
        //printf("line %d", y);
        if(draw_mode == 1) {  // desenhar o conte˙do da janela linha a linha
            drawPoints();
            index_pos=0;
            index_col=0;
        }
    }
    
    if(draw_mode == 2) //preenchar o conte˙do da janela com uma imagem completa
        drawPoints();
    
    printf("Terminou!\n");
}

void cleanup()
{
    destroyShaderProgram();
    destroyBufferObjects();
}

void ortho(float left, float right, float bottom, float top,
           float nearp, float farp)
{
    m[0 * 4 + 0] = 2 / (right - left);
    m[0 * 4 + 1] = 0.0;
    m[0 * 4 + 2] = 0.0;
    m[0 * 4 + 3] = 0.0;
    m[1 * 4 + 0] = 0.0;
    m[1 * 4 + 1] = 2 / (top - bottom);
    m[1 * 4 + 2] = 0.0;
    m[1 * 4 + 3] = 0.0;
    m[2 * 4 + 0] = 0.0;
    m[2 * 4 + 1] = 0.0;
    m[2 * 4 + 2] = -2 / (farp - nearp);
    m[2 * 4 + 3] = 0.0;
    m[3 * 4 + 0] = -(right + left) / (right - left);
    m[3 * 4 + 1] = -(top + bottom) / (top - bottom);
    m[3 * 4 + 2] = -(farp + nearp) / (farp - nearp);
    m[3 * 4 + 3] = 1.0;
}

void reshape(int w, int h)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, w, h);
    ortho(0, (float)RES_X, 0, (float)RES_Y, -1.0, 1.0);
}

/////////////////////////////////////////////////////////////////////// SETUP
void setupCallbacks()
{
    glutCloseFunc(cleanup);
    glutDisplayFunc(renderScene);
    glutReshapeFunc(reshape);
}

void setupGLEW() {
    glewExperimental = GL_TRUE;
    GLenum result = glewInit() ;
    if (result != GLEW_OK) {
        std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
        exit(EXIT_FAILURE);
    }
    GLenum err_code = glGetError();
    printf ("Vendor: %s\n", glGetString (GL_VENDOR));
    printf ("Renderer: %s\n", glGetString (GL_RENDERER));
    printf ("Version: %s\n", glGetString (GL_VERSION));
    printf ("GLSL: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));
}

void setupGLUT(int argc, char* argv[])
{
    glutInit(&argc, argv);
    
    glutInitContextVersion(3, 3);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    
    glutInitWindowPosition(640,100);
    glutInitWindowSize(RES_X, RES_Y);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glDisable(GL_DEPTH_TEST);
    WindowHandle = glutCreateWindow(CAPTION);
    if(WindowHandle < 1) {
        std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
        exit(EXIT_FAILURE);
    }
}

void init(int argc, char* argv[])
{
    setupGLUT(argc, argv);
    setupGLEW();
    std::cerr << "CONTEXT: OpenGL v" << glGetString(GL_VERSION) << std::endl;
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    createShaderProgram();
    createBufferObjects();
    setupCallbacks();
}

int main(int argc, char* argv[])
{
	scene = new Scene(std::string("NFF/balls_low.nff"));

	RES_X = scene->camera.resolution.WinX;
    RES_Y = scene->camera.resolution.WinY;
    
    if (draw_mode == 0) { // desenhar o conteudo da janela ponto a ponto
        size_vertices = 2 * sizeof(float);
        size_colors = 3 * sizeof(float);
        printf("DRAWING MODE: POINT BY POINT\n");
    }
    else if (draw_mode == 1) { // desenhar o conteudo da janela linha a linha
        size_vertices = 2 * RES_X * sizeof(float);
        size_colors = 3 * RES_X * sizeof(float);
        printf("DRAWING MODE: LINE BY LINE\n");
    }
    else if (draw_mode == 2) { // preencher o conteudo da janela com uma imagem completa
        size_vertices = 2 * RES_X * RES_Y * sizeof(float);
        size_colors = 3 * RES_X * RES_Y * sizeof(float);
        printf("DRAWING MODE: FULL IMAGE\n");
    }
    else {
        printf("Draw mode not valid \n");
        exit(0);
    }
    printf("resx = %d  resy= %d.\n", RES_X, RES_Y);
    
    vertices = (float*) malloc(size_vertices);
    if (vertices==NULL) exit (1);
    
    colors = (float*) malloc(size_colors);
    if (colors==NULL) exit (1);
    
    init(argc, argv);
    glutMainLoop();	
    exit(EXIT_SUCCESS);
}
///////////////////////////////////////////////////////////////////////