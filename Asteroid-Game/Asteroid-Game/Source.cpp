#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>
#include <cmath>

int WIDTH = 1000;
int HEIGHT = 700;

#define DEG2RAD(a) (a * 0.0174532925)
#define PI 3.14159265358979323846

// Plane Controller variables
float planeX = 0.0;
float planeY = 0.0;

// All asteroids controller variables
float asteroidScale = 0.03;
int seconds = 0;

// First asteroid controller variables
bool asteroid1 = true;
float a1t = 0.0;
float a1x = 0.0;
float a1y = 0.0;
float a1z = -30.0;
float a10[2];
float a11[2];
float a12[2];
float a13[2];

// Second asteroid controller variables
bool asteroid2 = true;
float a2t = 0.0;
float a2x = 0.0;
float a2y = 0.0;
float a2z = -30.0;

// Third asteroid controller variables
bool asteroid3 = true;
float a3t = 0.0;
float a3x = 0.0;
float a3y = (rand() % 7) - 3;
float a3z = -30.0;
float a30[2];
float a31[2];
float a32[2];
float a33[2];

// Fourth asteroid controller variables
bool asteroid4 = true;
float a4t = 0.0;
float a4x = (rand() % 11) - 5;
float a4y = 0.0;
float a4z = -30.0;
float a40[2];
float a41[2];
float a42[2];
float a43[2];

//Fifth  asteroid controller variables
bool asteroid5 = true;
float a5t = 0.0;
float a5x = (rand() % 7) - 3;
float a5y = 0.0;
float a5z = -30.0;

//Sixth asteroid controller variables
bool asteroid6 = true;
float a6t = 0.0;
float a6x = 0.0;
float a6y = 0.0;
float a6z = -30.0;
float a60[2];
float a61[2];
float a62[2];
float a63[2];

// shield controller variables
bool shield = true;
float shz = -30.0;
float shy = (rand() % 7) - 3;
bool shieldActivated = false;
float shieldCountDown = 3000;

// Nitrous controller variables
bool nitrous = true;
float nrot = 0.0;
float nz = -30.0;
float nx = (rand() % 7) - 3;
bool nitrousActivated = false;
float nitrousCountDown = 3000;

//Life plane controller Variables
float lz = -30;
float lAngle = 0;
float lt = 0;
float ly = 0;
float lx = 0;
bool life = true;

// Light controllers
float cutOff = 70.0;
float exponent = 120.0;
float lightsAngle = 0.0;
bool cutOffSwitch = false;
bool light1 = true;
bool light2 = false;
bool light3 = false;
bool light4 = true;

//Camera controller variables
float cameraRadius = 15;
float cameraAngle = 0.0;
bool camera360 = false;

float firstPersonX = 0.0;
float firstPersonY = -2.0;
float firstPersonZ = 10.0;
float firstPcenterX = 0.0;
float firstPcenterY = 0.0;
float firstPcenterZ = 0.0;
float fpx = 0.0;
float fpy = 0.0;
bool cameraFirstPerson = false;
bool cameraThirdPerson = true;

float sceneZ = 0.0;
float cameraZ = 0.0;

// Game Controllers
int score = 0;
int numOfLives = 5;
bool won = false;
bool lose = false;
bool game = true;
float rotSun = 0.0;
float collPlane = 0.0;
bool scene = false;

GLuint space_tex;
GLuint nitrous_tex;
GLuint planet_tex;
GLUquadricObj * qobj;
char title[] = "3D Model Loader Sample";

// Model Variables
Model_3DS model_plane2;
Model_3DS model_shield;
Model_3DS model_asteroid;
Model_3DS model_winner;
Model_3DS model_life;

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
		eye.x = 15.0;
		eye.y = 0.0;
		eye.z = 0.0;
	}

	void topView() {
		eye.x = 0.0;
		eye.y = 15.0;
		eye.z = 0.8;
	}

	void firstPerson() {
		eye.x = firstPersonX;
		eye.y = firstPersonY;
		eye.z = firstPersonZ;
		center.x = firstPcenterX;
		center.y = firstPcenterY;
		center.z = firstPcenterZ;
	}

	void thirdPerson() {
		eye.x = 0.0;
		eye.y = 0.0;
		eye.z = 15.0;
		center.x = 0.0;
		center.y = 0.0;
		center.z = 0.0;
	}

	void look() {
		if (camera360) {
			eye.x = cameraRadius* cos(DEG2RAD(cameraAngle));
			eye.y = 0.03;
			eye.z = cameraRadius* sin(DEG2RAD(cameraAngle));
		}
		if (cameraFirstPerson) {
			firstPerson();
		}
		if (cameraThirdPerson) {
			thirdPerson();
		}
		gluLookAt(
			eye.x, eye.y, eye.z - sceneZ,
			center.x, center.y, center.z - sceneZ,
			up.x, up.y, up.z
		);
	}
};

Camera camera;

/* Printing on Screen Function */
void print(float x, float y, float z, char *text)
{
	glRasterPos3f(x, y, z);

	for (int i = 0; text[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
}

/* Bezier curves Function */
float* bezier(float t, float* p0, float* p1, float* p2, float* p3)
{
	float res[2];
	res[0] = pow((1 - t), 3)*p0[0] + 3 * t*pow((1 - t), 2)*p1[0] + 3 * pow(t, 2)*(1 - t)*p2[0] + pow(t, 3)*p3[0];
	res[1] = pow((1 - t), 3)*p0[1] + 3 * t*pow((1 - t), 2)*p1[1] + 3 * pow(t, 2)*(1 - t)*p2[1] + pow(t, 3)*p3[1];
	return res;
}

/* Assets Loading Function */
void LoadAssets()
{
	//Loading Model files
	model_plane2.Load("Models/plane2/plane2.3ds");
	model_shield.Load("Models/shield/shield2/CaptainAmericasShield.3ds");
	model_asteroid.Load("Models/asteroid-3DS.3DS");
	model_winner.Load("Models/trophy.3ds");
	model_life.Load("Models/Heart.3ds");

	//Loading texture files
	loadBMP(&space_tex, "textures/space.bmp", true);
	loadBMP(&nitrous_tex, "textures/logo.bmp", true);
	loadBMP(&planet_tex, "textures/planet_texture.bmp", true);
}

/* Keyboard Function */
void Keyboard(unsigned char key, int x, int y) {

	switch (key) {
	case 'f':
		camera360 = false;
		cameraFirstPerson = false;
		cameraThirdPerson = true;
		break;
	case 's':
		camera360 = false;
		cameraFirstPerson = false;
		cameraThirdPerson = false;
		camera.sideView();
		break;
	case 't':
		camera360 = false;
		cameraFirstPerson = false;
		cameraThirdPerson = false;
		camera.topView();
		break;
	case 'r':
		cameraThirdPerson = false;
		cameraFirstPerson = false;
		camera360 = true;
		break;
	case 'a':
		camera360 = false;
		cameraThirdPerson = false;
		cameraFirstPerson = true;
		break;
	}
	glutPostRedisplay();
}

/* Keyboard Special Function */
void Special(int key, int x, int y) {

	if (game) {
		switch (key) {
		case GLUT_KEY_LEFT:
			if (planeX >= -3.5) {
				if (nitrousActivated || cameraFirstPerson) {
					planeX -= 0.2;
					if (cameraFirstPerson) {
						firstPersonX -= 0.2;
						firstPcenterX -= 0.2;
						fpx -= 0.2;
					}
				}
				else {
					planeX -= 0.1;
				}
			}
			break;
		case GLUT_KEY_RIGHT:
			if (planeX <= 3.5) {
				if (nitrousActivated || cameraFirstPerson) {
					planeX += 0.2;
					if (cameraFirstPerson) {
						firstPersonX += 0.2;
						firstPcenterX += 0.2;
						fpx += 0.2;
					}
				}
				else {
					planeX += 0.1;
				}
			}
			break;
		case GLUT_KEY_UP:
			if (planeY <= 6) {
				if (nitrousActivated || cameraFirstPerson) {
					planeY += 0.2;
					if (cameraFirstPerson) {
						firstPersonY += 0.2;
						firstPcenterY += 0.2;
						fpy += 0.2;
					}
				}
				else {
					planeY += 0.1;
				}
			}
			break;
		case GLUT_KEY_DOWN:
			if (planeY >= -1) {
				if (nitrousActivated || cameraFirstPerson) {
					planeY -= 0.2;
					if (cameraFirstPerson) {
						firstPersonY -= 0.2;
						firstPcenterY -= 0.2;
						fpy -= 0.2;
					}
				}
				else {
					planeY -= 0.1;
				}
			}
			break;
		}
	}

	glutPostRedisplay();
}

/* Camera setup function */
void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(100, 1000 / 700, 0.001, 100);
	//	glOrtho(-500, 500, -350, 350, -800, 800);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
}

/* Lighting configuratio for planet */
void PlanetLights() {

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);

	GLfloat l1Diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat l1Position[] = { 0.0f, 15.0f, -20.0f, light1 };
	GLfloat l1Direction[] = { 0.0, -1.0, 0.0 };

	glLightfv(GL_LIGHT1, GL_DIFFUSE, l1Diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, l1Position);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, cutOff);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, exponent);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, l1Direction);


	glEnable(GL_LIGHT2);

	GLfloat l2Diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat l2Position[] = { 15.0f, 0.0f, -20.0f, light2 };
	GLfloat l2Direction[] = { -1.0, 0.0, 0.0 };

	glLightfv(GL_LIGHT2, GL_DIFFUSE, l2Diffuse);
	glLightfv(GL_LIGHT2, GL_POSITION, l2Position);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, cutOff);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, exponent);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, l2Direction);


	glEnable(GL_LIGHT3);

	GLfloat l3Diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat l3Position[] = { -15.0f, 0.0f, -20.0f, light3 };
	GLfloat l3Direction[] = { 1.0, 0.0, 0.0 };

	glLightfv(GL_LIGHT3, GL_DIFFUSE, l3Diffuse);
	glLightfv(GL_LIGHT3, GL_POSITION, l3Position);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, cutOff);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, exponent);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, l3Direction);


	glEnable(GL_LIGHT4);

	GLfloat l4Diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat l4Position[] = { 0.0f, -15.0f, -20.0f, light4 };
	GLfloat l4Direction[] = { 0.0, 1.0, 0.0 };

	glLightfv(GL_LIGHT4, GL_DIFFUSE, l4Diffuse);
	glLightfv(GL_LIGHT4, GL_POSITION, l4Position);
	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, cutOff);
	glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, exponent);
	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, l4Direction);

}

/* Lighting Configuration Function */
void InitLightSource()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	glEnable(GL_LIGHT0);

	GLfloat specular0[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_position0[] = { 0.0f, 10.0f, -20.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);


	GLfloat lightIntensity0[] = { 0.4, 0.4, 0.4, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity0);

}

/* Material Configuration Function */
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
	glMaterialfv(GL_BACK, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	glMaterialfv(GL_BACK, GL_SHININESS, shininess);
}

/* Animation Function */
void Anim() {
	/* object */
	// x = 99.14 -> 15.86 3ard = 3.2  life 1.2
	// y = 68 -> 10.88 ertfa3 = 0.8   life 0.3
	// z = 152.2 -> 24.4 tool = 4.4   life 1.65
	/* asteroid */
	// x = 12.06 -> 0.36 3ard = 0.98
	// y = 7.63 -> 0.228 ertfa3 = 0.7
	// z = 7.47 -> 0.22  tool = 0.68
	/* shield */
	// radius -> 0.6
	// x = 1.2
	// y = 1.2
	// z = 0.2
	/* nitrous */
	// radius -> 0.5
	// x = 1 -> 0.7
	// z = 1 -> 0.7 
	// y = 2 -> 1.4

	if (game) {

		// first asteroid collision
		if (asteroid1) {
			if (((0.116 + a1x >= planeX - 1.6  && 0.116 + a1x <= planeX + 1.6) || (1.024 + a1x >= planeX - 1.6 && 1.024 + a1x <= planeX + 1.6))
				&&
				((-2.55 + a1y >= planeY - 3.4 && -2.55 + a1y <= planeY - 2.6) || (-1.85 + a1y >= planeY - 3.4 && -1.85 + a1y <= planeY - 2.6))
				&&
				((-0.35 + a1z >= 7.8 - sceneZ && -0.35 + a1z <= 12.2 - sceneZ) || (0.35 + a1z >= 7.8 - sceneZ && 0.35 + a1z <= 12.2 - sceneZ))) {
				printf("%s\n", "d5l");
				asteroid1 = false;
				a1z = -30;
				if (!shieldActivated) {
					numOfLives--;
					score -= 20;
				}
			}
		}

		// second asteroid collision
		if (asteroid2) {
			if (((1.43 + a2x >= planeX - 1.6 && 1.43 + a2x <= planeX + 1.6) || (2.57 + a2x >= planeX - 1.6 && 2.57 + a2x <= planeX + 1.6))
				&&
				((-2.2 >= planeY - 3.4 && -2.2 <= planeY - 2.6) || (-2.2 >= planeY - 3.4 && -2.2 <= planeY - 2.6))
				&&
				((-0.35 + a2z >= 7.8 - sceneZ && -0.35 + a2z <= 12.2 - sceneZ) || (0.35 + a2z >= 7.8 - sceneZ && 0.35 + a2z <= 12.2 - sceneZ))) {
				printf("%s\n", "d5l asr");
				asteroid2 = false;
				a2z = -30;
				if (!shieldActivated) {
					numOfLives--;
					score -= 20;
				}
			}
		}

		// third asteroid collision
		if (asteroid3) {
			if (((a3x - 2.57 - 0.454 >= planeX - 1.6 && a3x - 2.57 - 0.454 <= planeX + 1.6) || (a3x - 2.57 + 0.454 >= planeX - 1.6 && a3x - 2.57 + 0.454 <= planeX + 1.6))
				&&
				((a3y - 0.35 - 2 >= planeY - 3.4 && a3y - 0.35 - 2 <= planeY - 2.6) || (a3y + 0.35 - 2 >= planeY - 3.4 && a3y + 0.35 - 2 <= planeY - 2.6))
				&&
				((a3z - 0.34 >= 7.8 - sceneZ &&  a3z - 0.34 <= 12.2 - sceneZ) || (a3z + 0.34 >= 7.8 - sceneZ && a3z + 0.34 <= 12.2 - sceneZ))) {
				printf("%s\n", "d5l as3");
				asteroid3 = false;
				a3z = -30;
				if (!shieldActivated) {
					numOfLives--;
					score -= 20;
				}
			}
		}

		//	fourth asteroid collision
		if (asteroid4) {
			if (((a4x - 0.454 >= planeX - 1.6 && a4x - 0.454 <= planeX + 1.6) || (a4x + 0.454 >= planeX - 1.6 && a4x + 0.454 <= planeX + 1.6))
				&&
				((a4y - 0.35 >= planeY - 3.4 && a4y - 0.35 <= planeY - 2.6) || (a4y + 0.35 >= planeY - 3.4 && a4y + 0.35 <= planeY - 2.6))
				&&
				((a4z - 0.34 >= 7.8 - sceneZ &&  a4z - 0.34 <= 12.2 - sceneZ) || (a4z + 0.34 >= 7.8 - sceneZ && a4z + 0.34 <= 12.2 - sceneZ))) {
				printf("%s\n", "d5l as4");
				asteroid4 = false;
				a4z = -30;
				if (!shieldActivated) {
					numOfLives--;
					score -= 20;
				}
			}
		}

		//	fifth asteroid collision
		if (asteroid5) {
			if (((a5x - 2.954 >= planeX - 1.6 && a5x - 2.954 <= planeX + 1.6) || (a5x - 2.046 >= planeX - 1.6 && a5x - 2.046 <= planeX + 1.6))
				&&
				((a5y - 2.05 >= planeY - 3.4 && a5y - 2.05 <= planeY - 2.6) || (a5y - 1.35 >= planeY - 3.4 && a5y - 1.35 <= planeY - 2.6))
				&&
				((a5z - 0.34 >= 7.8 - sceneZ &&  a5z - 0.34 <= 12.2 - sceneZ) || (a5z + 0.34 >= 7.8 && a5z + 0.34 <= 12.2 - sceneZ))) {
				printf("%s\n", "d5l as5");
				asteroid5 = false;
				a5z = -30;
				if (!shieldActivated) {
					numOfLives--;
					score -= 20;
				}
			}
		}

		//	sixth asteroid collision
		if (asteroid6) {
			if (((-a6x - 0.954 >= planeX - 1.6 && -a6x - 0.954 <= planeX + 1.6) || (-a6x - 0.046 >= planeX - 1.6 && -a6x - 0.046 <= planeX + 1.6))
				&&
				((-a6y + 1.45 >= planeY - 3.4 && -a6y + 1.45 <= planeY - 2.6) || (-a6y + 2.15 >= planeY - 3.4 && -a6y + 2.15 <= planeY - 2.6))
				&&
				((a6z - 0.34 >= 7.8 - sceneZ &&  a6z - 0.34 <= 12.2 - sceneZ) || (a6z + 0.34 >= 7.8 - sceneZ && a6z + 0.34 <= 12.2 - sceneZ))) {
				printf("%s\n", "d5l as6");
				asteroid6 = false;
				a6z = -30;
				if (!shieldActivated) {
					numOfLives--;
					score -= 20;
				}
			}
		}

		// life Collision
		if (life) {
			if (((lx - 0.6 >= planeX - 1.6 && lx - 0.6 <= planeX + 1.6) || (lx + 0.6 >= planeX - 1.6 && lx + 0.6 <= planeX + 1.6))
				&&
				((ly + 1 - 0.15 >= planeY - 3.4 && ly + 1 - 0.15 <= planeY - 2.6) || (ly + 1 + 0.15 >= planeY - 3.4 && ly + 1 + 0.15 <= planeY - 2.6))
				&&
				((lz - 0.825 >= 7.8 - sceneZ &&  lz - 0.825 <= 12.2 - sceneZ) || (lz + 0.825 >= 7.8 - sceneZ && lz + 0.825 <= 12.2 - sceneZ))) {
				printf("%s\n", "d5l life");
				life = false;
				lz = -30;
				numOfLives++;
			}
		}

		// shield collision
		if (shield) {
			if (((-0.6 >= planeX - 1.6  && -0.6 <= planeX + 1.6) || (0.6 >= planeX - 1.6 && 0.6 <= planeX + 1.6))
				&&
				((shy >= planeY - 3.4 && shy <= planeY - 2.6) || (shy + 1.2 >= planeY - 3.4 && shy + 1.2 <= planeY - 2.6))
				&&
				((-0.1 + shz >= 7.8 - sceneZ && -0.1 + shz <= 12.2 - sceneZ) || (0.1 + shz >= 7.8 - sceneZ && 0.1 + shz <= 12.2 - sceneZ))) {
				printf("%s\n", "d5l shield");
				shieldActivated = true;
				shield = false;
				scene = true;
				shz = -30;
			}
		}

		// nitrous collision
		if (nitrous) {
			if (((nx - 0.35 >= planeX - 1.6  && nx - 0.35 <= planeX + 1.6) || (nx + 0.35 >= planeX - 1.6 && nx + 0.35 <= planeX + 1.6))
				&&
				((-0.7 >= planeY - 3.4 && -0.7 <= planeY - 2.6) || (0.7 >= planeY - 3.4 && 0.7 <= planeY - 2.6))
				&&
				((nz - 0.35 >= 7.8 - sceneZ && nz - 0.35 <= 12.2 - sceneZ) || (0.35 + nz >= 7.8 - sceneZ && 0.35 + nz <= 12.2 - sceneZ))) {
				printf("%s\n", "d5l nitrous");
				nitrousActivated = true;
				nitrous = false;
				scene = true;
				nz = -30;
			}
		}

		// shield count down
		if (shieldActivated) {
			if (scene) {
				sceneZ += 7;
				scene = false;
			}
			shieldCountDown--;
			if (shieldCountDown <= 0) {
				shieldCountDown = 3000;
				shieldActivated = false;
			}
		}

		// nitrous count down
		if (nitrousActivated) {
			if (scene) {
				sceneZ += 7;
				scene = false;
			}
			nitrousCountDown--;
			if (nitrousCountDown <= 0) {
				nitrousCountDown = 3000;
				nitrousActivated = false;
			}
		}

		// checking player position
		if (7.8 - sceneZ <= -20) {
			game = false;
			won = true;
		}

		// checking player number of lives
		if (numOfLives == 0) {
			lose = true;
			game = false;
		}
	}

	glutPostRedisplay();
}

/* Timer Function */
void Timer(int value) {
	lightsAngle += 0.3;
	rotSun++;
	seconds++;

	if (seconds % 20 == 0) {
		score++;
	}
	/************************CAMERA ANIMATION***************************/
	if (camera360) {
		cameraAngle++;
	}

	if (cameraZ >= 50) {
		cameraZ = 0;
	}
	else {
		cameraZ += 0.1;
	}

	/************************LIGHT ANIMATION****************************/
	/*if (cutOff <= 0) {
	cutOffSwitch = true;
	}
	if (cutOff >= 60) {
	cutOffSwitch = false;
	}
	if (cutOffSwitch) {
	cutOff += 0.1;
	}
	else {
	cutOff -= 0.1;
	}*/

	if (seconds % 30 == 0) {
		light1 = !light1;
		light2 = !light2;
		light3 = !light3;
		light4 = !light4;
	}

	if (game) {
		/***********************FIRST ASTEROID ANIMATION**********************/
		if (seconds >= 0) {
			if (asteroid1) {
				a1z += 0.05;
				if (a1t >= 1) {
					a1t = 0.0;
				}
				else {
					a1t += 0.005;

					float* b = bezier(a1t, a10, a11, a12, a13);
					a1x = b[0];
					a1y = b[1];
				}
			}
		}

		/***********************SECOND ASTEROID ANIMATION**********************/
		if (seconds >= 150) {
			if (asteroid2) {
				a2t = a2t + 0.01;
				a2z += 0.05;
				a2x = (sin(a2t*1.15) * 6) / 3;
			}
		}

		/***********************THIRD ASTEROID ANIMATION**********************/
		if (seconds >= 300) {
			if (asteroid3) {
				a3z += 0.05;
				if (a3t >= 1) {
					a3t = 0.0;
				}
				else {
					a3t += 0.005;

					float* b = bezier(a3t, a30, a31, a32, a33);
					a3x = b[0];
				}
			}
		}

		/***********************FOURTH ASTEROID ANIMATION**********************/
		if (seconds >= 450) {
			if (asteroid4) {
				a4z += 0.05;
				if (a4t >= 1) {
					a4t = 0.0;
				}
				else {
					a4t += 0.005;

					float* b = bezier(a4t, a40, a41, a42, a43);
					a4y = b[0];
				}
			}
		}

		/***********************FIFTH ASTEROID ANIMATION**********************/
		if (seconds >= 600) {
			if (asteroid5) {
				a5z += 0.05;
				a5t = a5t + 0.01;
				a5y = (cos(a5t * 2) * 6) / 3;
			}
		}

		/***********************SIXTH ASTEROID ANIMATION**********************/
		if (seconds >= 750) {
			if (asteroid6) {
				a6z += 0.05;
				if (a6t >= 1) {
					a6t = 0.0;
					//asteroid6 = false;
				}
				else {
					a6t += 0.005;

					float* b = bezier(a6t, a60, a61, a62, a63);
					a6x = b[0];
					a6y = b[1];
				}
			}
		}

		if (seconds == 900) {
			a1z = -30;
			asteroid1 = true;
		}

		if (seconds == 1050) {
			a2z = -30;
			asteroid2 = true;
		}

		/***********************SHIELD ANIMATION*********************************/
		if (seconds >= 1200) {
			if (shield) {
				shz += 0.05;
			}
		}

		if (seconds == 1350) {
			a3z = -30;
			a3y = (rand() % 7) - 3;
			asteroid3 = true;
		}

		if (seconds == 1500) {
			a4z = -30;
			a4x = (rand() % 7) - 3;
			asteroid4 = true;
		}

		if (seconds == 1650) {
			a5z = -30;
			a5x = (rand() % 11) - 3;
			asteroid5 = true;
		}

		if (seconds == 1800) {
			a6z = -30;
			asteroid6 = true;
		}

		if (seconds == 1950) {
			a1z = -30;
			asteroid1 = true;
		}

		if (seconds == 2100) {
			a2z = -30;
			asteroid2 = true;
		}

		if (seconds == 2250) {
			a3z = -30;
			a3y = (rand() % 7) - 3;
			asteroid3 = true;
		}

		if (seconds == 2400) {
			a4z = -30;
			a4x = (rand() % 7) - 3;
			asteroid4 = true;
		}

		/***********************NITROUS ANIMATION*********************************/
		if (seconds >= 2550) {
			if (nitrous) {
				nrot += 3;
				nz += 0.05;
			}
		}

		if (seconds == 2700) {
			a5z = -30;
			a5x = (rand() % 11) - 3;
			asteroid5 = true;
		}

		if (seconds == 2850) {
			a6z = -30;
			asteroid6 = true;
		}

		if (seconds == 3000) {
			a1z = -30;
			asteroid1 = true;
		}

		if (seconds == 3150) {
			a2z = -30;
			asteroid2 = true;
		}

		if (seconds == 3300) {
			a3z = -30;
			a3y = (rand() % 7) - 3;
			asteroid3 = true;
		}

		if (seconds == 3450) {
			a4z = -30;
			a4x = (rand() % 7) - 3;
			asteroid4 = true;
		}

		if (seconds == 3600) {
			a5z = -30;
			a5x = (rand() % 11) - 3;
			asteroid5 = true;
		}

		if (seconds == 3750) {
			a6z = -30;
			asteroid6 = true;
		}

		if (seconds == 3900) {
			a1z = -30;
			asteroid1 = true;
		}

		if (seconds == 4050) {
			a2z = -30;
			asteroid2 = true;
		}

		/***********************LIFE ANIMATION*********************************/
		if (seconds >= 4200) {
			if (life) {
				lAngle++;
				lt = lt + 0.01;
				lz += 0.05;
				ly = (cos(lt*1.15) * 6) / 3;
				lx = (sin(lt*3.15) * 2) / 3;
			}
		}

		if (seconds >= 5100) {
			seconds = 0;

			// reseting booleans
			asteroid1 = true;
			asteroid2 = true;
			asteroid3 = true;
			asteroid4 = true;
			asteroid5 = true;
			asteroid6 = true;
			life = true;
			nitrous = true;
			shield = true;

			// reseting positions
			a1z = -30;
			a2z = -30;
			a3z = -30;
			a3y = (rand() % 7) - 3;
			a4z = -30;
			a4x = (rand() % 11) - 5;
			a5z = -30;
			a5x = (rand() % 7) - 3;
			a6z = -30;
			shz = -30;
			shy = (rand() % 7) - 3;
			nz = -30;
			nx = (rand() % 7) - 3;
			lz = -30;
		}
	}
	glutPostRedisplay();
	glutTimerFunc(10, Timer, 0);
}

/* Draw Life Function */
void drawLife(float x, float y, float z) {
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotated(90, 1, 0, 0);
	glRotated(30, 0, 0, 1);
	glScalef(0.004, 0.004, 0.004);
	model_plane2.Draw();
	glPopMatrix();
}

/* Draw Lives Function */
void drawLives() {
	float initialX = 6;
	for (int i = 0; i < numOfLives; i++) {
		if (cameraFirstPerson) {
			drawLife(initialX - 0.5 + fpx, 4 + fpy, 5.5 - sceneZ);
		}
		else {
			drawLife(initialX, 6, 8.5 - sceneZ);
		}
		initialX -= 1;
	}
}

/* Drawing Nitrous */
void drawNitrous() {
	glPushMatrix();
	glColor3f(1, 1, 1);
	glRotated(180, 0, 1, 0);
	//glTranslated(2, 0.5, -3);
	//glRotated(-90, 0, 1, 0);
	glRotated(90, 1, 0, 0);
	qobj = gluNewQuadric();
	glBindTexture(GL_TEXTURE_2D, nitrous_tex);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluCylinder(qobj, 0.5, 0.5, 2, 100, 100);
	gluDeleteQuadric(qobj);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslated(0, 0.5, 0);
	glRotated(180, 0, 1, 0);
	glRotated(90, 1, 0, 0);
	qobj = gluNewQuadric();
	glBindTexture(GL_TEXTURE_2D, nitrous_tex);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluCylinder(qobj, 0.2, 0.2, 0.5, 100, 100);
	gluDeleteQuadric(qobj);
	glPopMatrix();
}

/* While Player in Game Function */
void inGame() {
	glPushMatrix();
	glColor3f(1, 1, 1);
	char* text[10];
	sprintf((char *)text, "Score: %d", score);
	if (cameraFirstPerson) {
		print(-5.2 + fpx, 3.3 + fpy, 6 - sceneZ, (char *)text);
	}
	else {
		print(-5.7, 5.3, 9 - sceneZ, (char *)text);
	}
	glPopMatrix();

	glColor3f(1, 1, 1);

	if (shieldActivated) {
		glPushMatrix();
		glColor3f(1, 1, 1);
		print(-5.7, 4.3, 9 - sceneZ, "SHIELD ACTIVATED");
		glPopMatrix();
		glColor3f(1, 1, 1);
	}

	//life plane model
	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslatef(lx, ly + 1, lz);
	glRotated(lAngle, 0, 1, 0);
	glScalef(0.7, 0.7, 0.7);
	model_life.Draw();
	glPopMatrix();

	// nitrous
	glPushMatrix();
	glTranslated(nx, 0, nz);
	glTranslated(0, 0, 2);
	glRotated(nrot, 0, 1, 0);
	glScaled(0.7, 0.7, 0.7);
	drawNitrous();
	glPopMatrix();

	drawLives();

	// shield
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslated(0, shy, shz);
	glRotatef(90, 1, 0, 0);
	glScaled(0.5, 0.5, 0.5);
	model_shield.Draw();
	glPopMatrix();

	//Asteroid 1
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(a1x + 0.57, a1y - 2.2, a1z);
	glScalef(asteroidScale, asteroidScale, asteroidScale);
	model_asteroid.Draw();
	glPopMatrix();

	// Asteroid 2
	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslatef(a2x + 2.57, -2.2, a2z);
	glScalef(asteroidScale, asteroidScale, asteroidScale);
	model_asteroid.Draw();
	glPopMatrix();

	// Asteroid 3
	glPushMatrix();
	glColor3f(1, 0, 1);
	glTranslatef(a3x - 2.57, a3y - 2, a3z);
	glScalef(asteroidScale, asteroidScale, asteroidScale);
	model_asteroid.Draw();
	glPopMatrix();

	// Asteroid 4
	glPushMatrix();
	glColor3f(0, 1, 1);
	glTranslatef(a4x, a4y, a4z);
	glScalef(asteroidScale, asteroidScale, asteroidScale);
	model_asteroid.Draw();
	glPopMatrix();

	//Asteroid 5
	glPushMatrix();
	glColor3f(1, 0.7, 0.8);
	glTranslatef(a5x - 2.5, a5y - 1.7, a5z);
	glScalef(asteroidScale, asteroidScale, asteroidScale);
	model_asteroid.Draw();
	glPopMatrix();

	//Asteroid 6
	glPushMatrix();
	glColor3f(0.6, 0, 0.298);
	glTranslatef(-a6x - 0.5, -a6y + 1.8, a6z);
	glScalef(asteroidScale, asteroidScale, asteroidScale);
	model_asteroid.Draw();
	glPopMatrix();

	glColor3f(1, 1, 1);
}

/* When player loses */
void loseGame() {
	planeX = 0;
	planeY = 0;
	glPushMatrix();
	glColor3f(1, 0, 0);
	if (cameraFirstPerson) {
		print(-0.7 + fpx, 1 + fpy, 3 - sceneZ, "YOU LOST");
	}
	else {
		print(-0.2, 1, 13 - sceneZ, "YOU LOST");
	}
	glPopMatrix();
}

/* When player wins */
void winGame() {

	planeX = 0;
	planeY = 0;
	glPushMatrix();
	glColor3f(1, 0, 0);
	if (cameraFirstPerson) {
		print(-0.7 + fpx, 1 + fpy, 3 - sceneZ, "YOU WON");
	}
	else {
		print(-0.2, 1, 13 - sceneZ, "YOU WON");
	}
	glPopMatrix();

	glColor3f(1, 1, 1);

	//Winner
	//glPushMatrix();
	//glColor3f(0.854, 0.647, 0.125);
	//glTranslated(-0.7, 0, 0);
	////glRotated(180, 0, 1, 0);
	//glRotatef(90, 1, 0, 0);
	//glScaled(0.05, 0.05, 0.05);
	//model_winner.Draw();
	//glPopMatrix();

	glColor3f(1, 1, 1);
}

/* Display Function */
void myDisplay(void)
{
	setupCamera();
	InitLightSource();
	InitMaterial();

	glPushMatrix();
	glTranslated(0, 0, -20);
	glRotated(lightsAngle, 0, 0, 1);
	glTranslated(0, 0, 20);
	PlanetLights();
	glPopMatrix();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//space box
	glPushMatrix();
	//glTranslated(0, 0, -80);
	//glRotated(-rotSun, 0, 1, 0);
	glRotated(90, 1, 0, 1);
	qobj = gluNewQuadric();
	glBindTexture(GL_TEXTURE_2D, space_tex);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 25, 100, 100);
	gluDeleteQuadric(qobj);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0, -22.5);
	glRotated(rotSun, 0, 1, 0);
	qobj = gluNewQuadric();
	glBindTexture(GL_TEXTURE_2D, planet_tex);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 5, 100, 100);
	gluDeleteQuadric(qobj);
	glPopMatrix();

	//space cylinder1
	//glPushMatrix();
	//glTranslated(0, 0, -sceneZ);
	//glTranslated(0, 0, cameraZ);
	//glTranslated(0, 0, -25);
	//qobj = gluNewQuadric();
	////glRotated(90, 1, 0, 0);
	//glBindTexture(GL_TEXTURE_2D, tex);
	//gluQuadricTexture(qobj, true);
	//gluQuadricNormals(qobj, GL_SMOOTH);
	//gluCylinder(qobj, 15, 15, 50, 100, 100);
	//gluDeleteQuadric(qobj);
	//glPopMatrix();

	//space cylinder2
	//glPushMatrix();
	//glTranslated(0, 0, -sceneZ);
	//glTranslated(0, 0, cameraZ);
	//glTranslated(0, 0, -75);
	//qobj = gluNewQuadric();
	////glRotated(90, 1, 0, 0);
	//glBindTexture(GL_TEXTURE_2D, tex);
	//gluQuadricTexture(qobj, true);
	//gluQuadricNormals(qobj, GL_SMOOTH);
	//gluCylinder(qobj, 15, 15, 50, 100, 100);
	//gluDeleteQuadric(qobj);
	//glPopMatrix();

	// large plane model
	glPushMatrix();
	glTranslated(0, 0, -sceneZ);
	glTranslated(planeX, planeY, 0);
	glTranslatef(0, -3, 10);
	glScalef(0.016, 0.016, 0.016);
	model_plane2.Draw();
	glPopMatrix();

	if (game) {
		inGame();
	}
	else if (won) {
		winGame();
	}
	else {
		loseGame();
	}

	glColor3f(1, 1, 1);

	glutSwapBuffers();
}

/* Main Function */
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

	glutIdleFunc(Anim);

	glutTimerFunc(0, Timer, 0);

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	LoadAssets();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	// first asteroid bezier
	a10[0] = 0.0;
	a10[1] = 0.0;

	a11[0] = -8.0;
	a11[1] = 3.0;

	a12[0] = 8.0;
	a12[1] = 3.0;

	a13[0] = 0.0;
	a13[1] = 0.0;

	// third asteroid bezier
	a30[0] = 0.0;

	a31[0] = -3.0;

	a32[0] = 10.0;

	a33[0] = 0.0;

	// fourth asteroid bezier
	a40[0] = 0.0;

	a41[0] = -8.0;

	a42[0] = 8.0;

	a43[0] = 0.0;

	// sixth asteroid bezier
	a60[0] = 0.0;
	a60[1] = 0.0;

	a61[0] = -8.0;
	a61[1] = 3.0;

	a62[0] = 8.0;
	a62[1] = 3.0;

	a63[0] = 0.0;
	a63[1] = 0.0;

	glutMainLoop();
}