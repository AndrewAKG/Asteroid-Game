#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>
#include <cmath>


int WIDTH = 1000;
int HEIGHT = 700;

#define DEG2RAD(a) (a * 0.0174532925)

//Camera controller variables
float cameraRadius = 15;
float cameraAngle = 90.0;
bool camera360 = false;
float camy = 0.03;

//Plane Controller variables
float planeX = 0.0;
float planeY = 0.0;
float planeAngX = 0.0;
float planeAngY = 0.0;
float planeAngZ = 0.0;

GLuint tex;
char title[] = "3D Model Loader Sample";

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 100;

int cameraZoom = 0;

//// Model Variables
Model_3DS model_house;
Model_3DS model_tree;
Model_3DS model_plane;
Model_3DS model_plane2;
Model_3DS shield;
Model_3DS model_asteroid;
// Textures
GLTexture tex_ground;

class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f &v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f &v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};

class Camera {
public:
	Vector3f eye, center, up;

	Camera(float eyeX = 0.0f, float eyeY = 0.0f, float eyeZ = 15.0f, float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
	}

	void sideView() {
		eye.x = 1.0;
		eye.y = 0.0;
		eye.z = 0.0;
	}

	void frontView() {
		eye.x = 0.0;
		eye.y = 0.0;
		eye.z = 1.0;
	}

	void topView() {
		eye.x = 0.0;
		eye.y = 0.6;
		eye.z = 0.8;
	}

	void look() {
		if (camera360) {
			eye.x = cameraRadius* cos(DEG2RAD(cameraAngle));
			eye.y = camy;
			eye.z = cameraRadius* sin(DEG2RAD(cameraAngle));
		}
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
	}
};

Camera camera;

float* bezier(float t, float* p0, float* p1, float* p2, float* p3)
{
	float res[2];
	res[0] = pow((1 - t), 3)*p0[0] + 3 * t*pow((1 - t), 2)*p1[0] + 3 * pow(t, 2)*(1 - t)*p2[0] + pow(t, 3)*p3[0];
	res[1] = pow((1 - t), 3)*p0[1] + 3 * t*pow((1 - t), 2)*p1[1] + 3 * pow(t, 2)*(1 - t)*p2[1] + pow(t, 3)*p3[1];
	return res;
}

//=======================================================================
// Keyboard Function
//=======================================================================
void Keyboard(unsigned char key, int x, int y) {

	switch (key) {
	case 'f':
		camera.frontView();
		break;
	case 's':
		camera.sideView();
		break;
	case 't':
		camera.topView();
		break;
	case 'r':
		camera360 = !camera360;
		break;
	}
	glutPostRedisplay();
}

void Special(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_LEFT:
		planeX -= 0.1;
		planeAngY += 0.1;
		planeAngZ += 0.5;
		break;
	case GLUT_KEY_RIGHT:
		planeX += 0.1;
		planeAngY -= 0.1;
		planeAngZ -= 0.5;
		break;
	case GLUT_KEY_UP:
		planeY += 0.1;
		planeAngX += 0.5;
		break;
	case GLUT_KEY_DOWN:
		planeY -= 0.1;
		planeAngX -= 0.5;
		break;
	}

	glutPostRedisplay();
}

void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(100, 1000 / 700, 0.001, 100);
	//	glOrtho(-500, 500, -350, 350, -800, 800);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
}

//=======================================================================
// Lighting Configuration Function
//=======================================================================
void InitLightSource()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	// OpengL has 8 light sources
	glEnable(GL_LIGHT0);

	// Define Light source 0 ambient light
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	// Define Light source 0 diffuse light
	GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	// Define Light source 0 Specular light
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// Finally, define light source 0 position in World Space
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

//=======================================================================
// Material Configuration Function
//======================================================================
void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

void Timer(int value) {
	/************************CAMERA ANIMATION***************************/
	if (camera360) {
		cameraAngle++;
	}

	glutPostRedisplay();
	glutTimerFunc(10, Timer, 0);
}

//=======================================================================
// Display Function
//=======================================================================
void myDisplay(void)
{
	setupCamera();
	InitLightSource();
	//InitMaterial();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
	GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);*/

	glPushMatrix();
	glTranslated(planeX, planeY, 0);
	glRotated(planeAngX, 1, 0, 1);
	glRotated(planeAngY, 0, 1, 0);
	glRotated(planeAngZ, 0, 0, 1);
	glTranslatef(0, -3, 10);
	glScalef(0.016, 0.016, 0.016);
	model_plane2.Draw();
	glPopMatrix();

	glPushMatrix();
	glScalef(4, 4, 4);
	glRotatef(90, 1,0, 0);
	shield.Draw();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslatef(-10, 10, 0);
	glScalef(0.1, 0.1, 0.1);
	model_asteroid.Draw();
	glPopMatrix();

	//sky box
	glPushMatrix();

	GLUquadricObj * qobj;
	qobj = gluNewQuadric();
	//glTranslated(50, 0, 0);
	glRotated(90, 1, 0, 1);
	glBindTexture(GL_TEXTURE_2D, tex);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 80, 100, 100);
	gluDeleteQuadric(qobj);

	glPopMatrix();

	glColor3f(1, 1, 1);
	glutSwapBuffers();
}

//=======================================================================
// Assets Loading Function
//=======================================================================
void LoadAssets()
{

	//Loading Model files
	model_plane2.Load("Models/plane2/plane2.3ds");
	shield.Load("Models/shield/shield2/CaptainAmericasShield.3ds");
	model_asteroid.Load("Models/asteroid-3DS.3DS");
	//Loading texture files
	loadBMP(&tex, "textures/space.bmp", true);
}

//=======================================================================
// Main Function
//=======================================================================
void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(200, 100);

	glutCreateWindow(title);

	glutDisplayFunc(myDisplay);

	glutKeyboardFunc(Keyboard);

	glutSpecialFunc(Special);

	glutTimerFunc(0, Timer, 0);

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	LoadAssets();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}