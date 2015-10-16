// Core Libraries
#include <iostream>
#include <string>
#include <math.h>
#include "camera.h"
#include <fstream>
using namespace std;
// 3rd Party Libraries
#include <GLUT\glut.h>
#include <IL\ilut.h>

// Defines and Core variables
#define FRAMES_PER_SECOND 60
const int FRAME_DELAY = 1000 / FRAMES_PER_SECOND; // Miliseconds per frame

int windowWidth = 800;
int windowHeight = 800;
float asp = (float)windowWidth / (float)windowHeight;
int wx = 0;
int sx = 0;
int ax = 0;
int dx = 0;

int mousepositionX;
int mousepositionY;

Camera cam0(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
Camera cam1(10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
Camera cam2(0.0f, 10.0f, 0.00001f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
Camera cam3(5.0f, 2.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

GLfloat xRot = 0.0f;
GLfloat yRot = 0.0f;
GLfloat zstep = 0.0f;
GLboolean light = true;
GLboolean model = true;
GLint polyMode = GL_FILL;

GLUquadricObj *quadratic;

int viewSelect = 0;

/* This is our texture handle */
GLuint textureHandle;

// A few conversions to know
float degToRad = 3.14159f / 180.0f;
float radToDeg = 180.0f / 3.14159f;

// Data structure

typedef struct tagVERTEX {
	float x, y, z;
	float r, g, b;
	float u, v;
} VERTEX;

typedef struct tagTRIANGLE {
	VERTEX vertex[3];
} TRIANGLE;

typedef struct tagOBJECT {
	int numtriangles;
	TRIANGLE* triangle;
} OBJECT;

OBJECT obj;

void parseFile() 
{

	// Open a file for reading
	ifstream infile;
	infile.open("obj.txt");

	char tag;
	float x, y, z, r, g, b;
	int nTriangles;

	cout << "Reading from file" << endl;

	infile >> nTriangles;
	obj.triangle = new TRIANGLE[nTriangles];
	obj.numtriangles = nTriangles;

	for (int t = 0; t < nTriangles; t++) 
	{
		for (int vert = 0; vert < 3; vert++) 
		{
			infile >> tag;
			if (tag == 'v') 
			{
				infile >> x >> y >> z;
				obj.triangle[t].vertex[vert].x = x;
				obj.triangle[t].vertex[vert].y = y;
				obj.triangle[t].vertex[vert].z = z;
				cout << obj.triangle[t].vertex[vert].x << " "
					 << obj.triangle[t].vertex[vert].y << " "
					 << obj.triangle[t].vertex[vert].z << endl;
			}

		}
	}
	// close the file
	infile.close();  

}// end parseFile

/*void drawSceneObjects(void)
{
	glPushMatrix(); //red cube
		glTranslatef(0.0f, 0.0f, 0.0f);
		glColor4f(1.0, 0.0, 0.0, 1.0);
		glutSolidCube(0.1);
	glPopMatrix();

	glPushMatrix(); //blue sphere
		glTranslatef(0.0f, 0.0f, -2.0f);
		glColor4f(0.0, 0.5, 1.0, 1.0);
		glutSolidSphere(1, 30, 30);
	glPopMatrix(); 

	glPushMatrix(); //orange sphere
		glTranslatef(3.0f, 0.0f, 0.0f);
		glColor4f(1.0, 0.5, 0.0, 1.0);
		glutSolidSphere(1, 30, 30);
	glPopMatrix();

	glPushMatrix(); //pink whosum
		glTranslatef(-3.0f, 0.0f, -2.0f);
		glColor4f(1.0, 0.5, 1.0, 1.0);
		glutSolidTorus(0.1, 0.5, 15, 15);
	glPopMatrix();

	glPushMatrix(); //yellow whassum
		glTranslatef(-3.0f, 0.0f, -20.0f);
		glColor4f(1.0, 1.0, 0.0, 1.0);
		glutSolidDodecahedron();
	glPopMatrix();
} 
*/

/* function DisplayCallbackFunction(void)
* Description:
*  - this is the openGL display routine
*  - this draws the sprites appropriately
*/
void DisplayCallbackFunction(void)
{
	/* clear the screen */
	glClearColor(0.2f, 0.2f, 0.2f, 0.8f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#pragma region viewports

	/* Move the Camera */

	static unsigned int oldTimeSinceStart = 0;
	unsigned int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	unsigned int deltaT = timeSinceStart - oldTimeSinceStart;
	oldTimeSinceStart = timeSinceStart;

	/* This is where we draw things */

	///Bottom Right Viewport///
	glViewport(windowWidth / 2, 0, windowWidth / 2, windowHeight / 2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); // clear our the transform matrix

	// reset perspective
	glFrustum(-100.0f, 100.0f, -100.0f, 100.0f, 0.1f, 10000.0f);
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//drawSceneObjects();

	//gluLookAt(5.0f, 3.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	cam3.Update((float)deltaT / 1000.0);
	cam3.applyCameraTransformations();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	for (int t = 0; t < obj.numtriangles; t++)
	{
		glBegin(GL_TRIANGLES);
		for (int vert = 0; vert < 3; vert++)
			glVertex3f(obj.triangle[t].vertex[vert].x,
			obj.triangle[t].vertex[vert].y,
			obj.triangle[t].vertex[vert].z);
	
		glEnd();
	}
	

	///Bottom Left Viewport///
	glViewport(0, 0, windowWidth / 2, windowHeight / 2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-30.0f, 30.0f, -30.0f, 30.0f, 1.0f, 100.0f);
	

	//gluLookAt(5.0f, 3.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);


	cam1.Update((float)deltaT / 1000.0);
	cam1.applyCameraTransformations();

	for (int t = 0; t < obj.numtriangles; t++)
	{
		glBegin(GL_TRIANGLES);
		for (int vert = 0; vert < 3; vert++)
			glVertex3f(obj.triangle[t].vertex[vert].x,
			obj.triangle[t].vertex[vert].y,
			obj.triangle[t].vertex[vert].z);
	
		glEnd();
	}

	//drawSceneObjects();

	///Top Left Viewport///
	glViewport(0, windowHeight / 2, windowWidth / 2, windowHeight / 2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-20.0f, 20.0f, -20.0f, 20.0f, 1.0f, 100.0f);
	;

	//gluLook(5.0f, 3.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	
	cam0.Update((float)deltaT / 1000.0);
	cam0.applyCameraTransformations();

	for (int t = 0; t < obj.numtriangles; t++)
	{
		glBegin(GL_TRIANGLES);
		for (int vert = 0; vert < 3; vert++)
			glVertex3f(obj.triangle[t].vertex[vert].x,
			obj.triangle[t].vertex[vert].y,
			obj.triangle[t].vertex[vert].z);
	
		glEnd();
	}

	//drawSceneObjects();

	///Top Right Viewport///
	glViewport(windowWidth / 2, windowHeight / 2, windowWidth / 2, windowHeight / 2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-5.0f, 5.0f, -5.0f, 5.0f, 1.0f, 100.0f);
	
	gluLookAt(5.0f, 3.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	cam2.Update((float)deltaT / 1000.0);
	cam2.applyCameraTransformations();

	for (int t = 0; t < obj.numtriangles; t++)
	{
		glBegin(GL_TRIANGLES);
		for (int vert = 0; vert < 3; vert++)
			glVertex3f(obj.triangle[t].vertex[vert].x,
			obj.triangle[t].vertex[vert].y,
			obj.triangle[t].vertex[vert].z);
	
		glEnd();
	}


	//drawSceneObjects();

	glViewport(0, 0, windowWidth, windowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

#pragma endregion

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

#pragma region bordersTL
	if (viewSelect == 0) 
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f); 
	else
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

	glBegin(GL_QUADS);  //top
		glVertex3f(-1.0f, 1.0f, 0.0f); //bottom left
		glVertex3f(0.0f, 1.0f, 0.0f); //bottom right
		glVertex3f(0.0f, 0.98f, 0.0f); //top right
		glVertex3f(-1.0f, 0.98f, 0.0f); //top left
	glEnd();

	glBegin(GL_QUADS); //bottom
		glVertex3f(-1.0f, 0.0f, 0.0f); //bottom left
		glVertex3f(0.0f, 0.0f, 0.0f); //bottom right
		glVertex3f(0.0f, 0.02f, 0.0f); //top right
		glVertex3f(-1.0f, 0.02f, 0.0f); //top left
	glEnd();

	glBegin(GL_QUADS); //left
		glVertex3f(-1.0f, 0.0f, 0.0f); //bottom left
		glVertex3f(-0.98f, 0.0f, 0.0f); //bottom right
		glVertex3f(-0.98f, 1.0f, 0.0f); //top right
		glVertex3f(-1.0f, 1.0f, 0.0f); //top left
	glEnd();

	glBegin(GL_QUADS); //right
		glVertex3f(-0.02f, 0.0f, 0.0f); //bottom left
		glVertex3f(0.0f, 0.0f, 0.0f); //bottom right
		glVertex3f(0.0f, 1.0f, 0.0f); //top right
		glVertex3f(-0.02f, 1.0f, 0.0f); //top left
	glEnd();
#pragma endregion

#pragma region bordersBL
	if (viewSelect == 1)
		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	else
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

	glBegin(GL_QUADS); //top
		glVertex3f(-1.0f, -0.02f, 0.0f); //bottom left
		glVertex3f(0.0f, -0.02f, 0.0f); //bottom right
		glVertex3f(0.0f, 0.0f, 0.0f); //top right
		glVertex3f(-1.0f, 0.0f, 0.0f); //top left
	glEnd();

	glBegin(GL_QUADS); //bottom
		glVertex3f(-1.0f, -1.0f, 0.0f); //bottom left
		glVertex3f(0.0f, -1.0f, 0.0f); //bottom right
		glVertex3f(0.0f, -0.98f, 0.0f); //top right
		glVertex3f(-1.0f, -0.98f, 0.0f); //top left
	glEnd();

	glBegin(GL_QUADS); //left
		glVertex3f(-1.0f, -1.0f, 0.0f); //bottom left
		glVertex3f(-0.98f, -1.0f, 0.0f); //bottom right
		glVertex3f(-0.98f, 0.0f, 0.0f); //top right
		glVertex3f(-1.0f, 0.0f, 0.0f); //top left
	glEnd();

	glBegin(GL_QUADS); //right
		glVertex3f(-0.02f, -1.0f, 0.0f); //bottom left
		glVertex3f(0.0f, -1.0f, 0.0f); //bottom right
		glVertex3f(0.0f, 0.0f, 0.0f); //top right
		glVertex3f(-0.02f, 0.0f, 0.0f); //top left
	glEnd();
#pragma endregion

#pragma region bordersTR
	if (viewSelect == 2)
		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

	else
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

	glBegin(GL_QUADS); //top
	glVertex3f(0.0f, 0.98f, 0.0f); //bottom left
	glVertex3f(1.0f, 0.98f, 0.0f); //bottom right
	glVertex3f(1.0f, 1.0f, 0.0f); //top right
	glVertex3f(0.0f, 1.0f, 0.0f); //top left
	glEnd();

	glBegin(GL_QUADS); //bottom
	glVertex3f(0.0f, 0.0f, 0.0f); //bottom left
	glVertex3f(1.0f, 0.0f, 0.0f); //bottom right
	glVertex3f(1.0f, 0.02f, 0.0f); //top right
	glVertex3f(0.0f, 0.02f, 0.0f); //top left
	glEnd();

	glBegin(GL_QUADS); //left
	glVertex3f(0.0f, 0.0f, 0.0f); //bottom left
	glVertex3f(0.02f, 0.0f, 0.0f); //bottom right
	glVertex3f(0.02f, 1.0f, 0.0f); //top right
	glVertex3f(0.0f, 1.0f, 0.0f); //top left
	glEnd();

	glBegin(GL_QUADS); //right
	glVertex3f(0.98f, 0.0f, 0.0f); //bottom left
	glVertex3f(1.0f, 0.0f, 0.0f); //bottom right
	glVertex3f(1.0f, 1.0f, 0.0f); //top right
	glVertex3f(0.98f, 1.0f, 0.0f); //top left
	glEnd();
#pragma endregion

#pragma region bordersBR
	if (viewSelect == 3)
		glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	else
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

	glBegin(GL_QUADS); //top
	glVertex3f(0.0f, -0.02f, 0.0f); //bottom left
	glVertex3f(1.0f, -0.02f, 0.0f); //bottom right
	glVertex3f(1.0f, 0.0f, 0.0f); //top right
	glVertex3f(0.0f, 0.0f, 0.0f); //top left
	glEnd();

	glBegin(GL_QUADS); //bottom
	glVertex3f(0.0f, -1.0f, 0.0f); //bottom left
	glVertex3f(1.0f, -1.0f, 0.0f); //bottom right
	glVertex3f(1.0f, -0.98f, 0.0f); //top right
	glVertex3f(0.0f, -0.98f, 0.0f); //top left
	glEnd();

	glBegin(GL_QUADS); //left
	glVertex3f(0.0f, -1.0f, 0.0f); //bottom left
	glVertex3f(0.02f, -1.0f, 0.0f); //bottom right
	glVertex3f(0.02f, 0.0f, 0.0f); //top right
	glVertex3f(0.0f, 0.0f, 0.0f); //top left
	glEnd();

	glBegin(GL_QUADS); //right
	glVertex3f(0.98f, -1.0f, 0.0f); //bottom left
	glVertex3f(1.0f, -1.0f, 0.0f); //bottom right
	glVertex3f(1.0f, 0.0f, 0.0f); //top right
	glVertex3f(0.98f, 0.0f, 0.0f); //top left
	glEnd();
#pragma endregion


	/* Swap Buffers to Make it show up on screen */
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	glutSwapBuffers();
}

/* function void KeyboardCallbackFunction(unsigned char, int,int)
* Description:
*   - this handles keyboard input when a button is pressed
*/
void KeyboardCallbackFunction(unsigned char key, int x, int y)
{
	std::cout << "Key Down:" << (int)key << std::endl;

	switch (key)
	{
	case 32: // the space bar
		break;
	case 27: // the escape key
	case 'q': // the 'q' key
		exit(0);
		break;
		//If statements for when the Viewport is clicked to activate keyboard controls 
	case 'w':
		wx = 1;
		if (viewSelect == 3)
		cam3.heldW = true;
		if (viewSelect == 2)
		cam2.heldW = true;
		if (viewSelect == 1)
		cam1.heldW = true;
		if (viewSelect == 0)
		cam0.heldW = true;
		break;
	case 's':
		sx = 1;
		if (viewSelect == 3)
		cam3.heldS = true;
		if (viewSelect == 2)
		cam2.heldS = true;
		if (viewSelect == 1)
		cam1.heldS = true;
		if (viewSelect == 0)
		cam0.heldS = true;
		break;
	case 'a':
		ax = 1;
		if (viewSelect == 3)
		cam3.heldA = true;
		if (viewSelect == 2)
		cam2.heldA = true;
		if (viewSelect == 1)
		cam1.heldA = true;
		if (viewSelect == 0)
		cam0.heldA = true;

		break;
	case 'd':
		dx = 1;
		if (viewSelect == 3)
		cam3.heldD = true;
		if (viewSelect == 2)
		cam2.heldD = true;
		if (viewSelect == 1)
		cam1.heldD = true;
		if (viewSelect == 0)
		cam0.heldD = true;
		break;
		//If statements to control polygon fill mode, l is line and o is fill
	case 'l':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'o':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	}
}

/* function void KeyboardUpCallbackFunction(unsigned char, int,int)
* Description:
*   - this handles keyboard input when a button is lifted
*/
void KeyboardUpCallbackFunction(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		wx = 0;
		cam3.heldW = false;
		cam2.heldW = false;
		cam1.heldW = false;
		cam0.heldW = false;
		break;
		sx = 0;
	case 's':
		sx = 0;
		cam3.heldS = false;
		cam2.heldS = false;
		cam1.heldS = false;
		cam0.heldS = false;
		break;
	case 'a':
		ax = 0;
		cam3.heldA = false;
		cam2.heldA = false;
		cam1.heldA = false;
		cam0.heldA = false;
		break;
		dx = 0;
	case 'd':
		cam3.heldD = false;
		cam2.heldD = false;
		cam1.heldD = false;
		cam0.heldD = false;
		break;
	}
}


/* function TimerCallbackFunction(int value)
* Description:
*  - this is called many times per second
*  - this enables you to animate things
*  - no drawing, just changing the state
*  - changes the frame number and calls for a redisplay
*  - FRAME_DELAY is the number of milliseconds to wait before calling the timer again
*/
void TimerCallbackFunction(int value)
{
	/* this call makes it actually show up on screen */
	glutPostRedisplay();
	/* this call gives it a proper frame delay to hit our target FPS */
	glutTimerFunc(FRAME_DELAY, TimerCallbackFunction, 0);
}

/* function WindowReshapeCallbackFunction()
* Description:
*  - this is called whenever the window is resized
*  - and sets up the projection matrix properly
*  - currently set up for an orthographic view (2D only)
*/
void WindowReshapeCallbackFunction(int w, int h)
{
	// switch to projection because we're changing projection
	float asp = (float)w / (float)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	//gluPerspective(90.0f, 1.0f, 1.0f, 100.f);

	//switch back to modelview
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void MouseClickCallbackFunction(int button, int state, int x, int y)
{
	// Handle mouse clicks
	if (state == GLUT_DOWN)
	{
		//Displays the x and y coordinates of where the mouse is clicked
		std::cout << "Mouse x:" << x << " y:" << y << std::endl;

		if (mousepositionX <= windowWidth / 2 && mousepositionY <= windowHeight / 2) //top left
			viewSelect = 0;

		if (mousepositionX <= windowWidth / 2 && mousepositionY > windowHeight / 2) //bottom left
			viewSelect = 1;

		if (mousepositionX > windowWidth / 2 && mousepositionY <= windowHeight / 2) //top right
			viewSelect = 2;

		if (mousepositionX > windowWidth / 2 && mousepositionY > windowHeight / 2) //bottom right
			viewSelect = 3;
	}
}


/* function MouseMotionCallbackFunction()
* Description:
*   - this is called when the mouse is clicked and moves
*/
void MouseMotionCallbackFunction(int x, int y)
{
}

/* function MousePassiveMotionCallbackFunction()
* Description:
*   - this is called when the mouse is moved in the window
*/
void MousePassiveMotionCallbackFunction(int x, int y)
{
	int changeInX = mousepositionX - x;
	int changeInY = mousepositionY - y;

	mousepositionX = x;
	mousepositionY = y;

	float fovX = 90.0 * asp; //close approximation

	//get the rotation in relation to screen size
	float percentXchange = (float) changeInX / windowWidth /2;
	float perceivedYaw = 50 * -percentXchange * degToRad*fovX;

	float percentYchange = (float)changeInY / (float)windowHeight;
	float perceivedPitch = 50 * percentYchange * degToRad*90.0;
	
	//If cam 3 (bottom right) is selected, set pitch and yaw to the mouse
	if (viewSelect == 3)
		
	{
		if (x != windowWidth / 2 && y != windowHeight / 2)
		{
			cam3.setPitch(perceivedPitch);
			cam3.setYaw(perceivedYaw);
		}
	}

}

void init()
{
	///// INIT OpenGL /////
	// Set color clear value
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);

	// Enable Z-buffer read and write (for hidden surface removal)
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D); // textures for future use

	// example06 //////
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat mat_diffuse[] = { 0.0, 1.0, 0.0, 1.0 };
	GLfloat mat_ambient[] = { 1.0, 0.0, 0.0, 1.0 };

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);

	GLfloat light_position[] = { 100.0, 100.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//glEnable(GL_COLOR_MATERIAL); // final polygon color will be based on glColor and glMaterial

	quadratic = gluNewQuadric();          // Create A Pointer To The Quadric Object
	gluQuadricNormals(quadratic, GLU_SMOOTH);   // Create Smooth Normals

	///////////////// 
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
}

/* function main()
* Description:
*  - this is the main function
*  - does initialization and then calls glutMainLoop() to start the event handler
*/
int main(int argc, char **argv)
{
	/* initialize the window and OpenGL properly */
	glutInit(&argc, argv);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Viewport Program");

	/* set up our function callbacks */
	glutDisplayFunc(DisplayCallbackFunction);
	glutKeyboardFunc(KeyboardCallbackFunction);
	glutKeyboardUpFunc(KeyboardUpCallbackFunction);
	glutReshapeFunc(WindowReshapeCallbackFunction);
	glutMouseFunc(MouseClickCallbackFunction);
	glutMotionFunc(MouseMotionCallbackFunction);
	glutPassiveMotionFunc(MousePassiveMotionCallbackFunction);
	glutTimerFunc(1, TimerCallbackFunction, 0);

	/* Call some OpenGL parameters */
	//glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	

	/* Turn on the lights! */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);

	/* Init Image Library */
	glEnable(GL_TEXTURE_2D);
	ilInit();
	parseFile();
	iluInit();
	ilutRenderer(ILUT_OPENGL);
	

	/* Load a texture */
//	textureHandle = ilutGLLoadImage("..//img//win.png");
//	glBindTexture(GL_TEXTURE_2D, textureHandle);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//	glBindTexture(GL_TEXTURE_2D, NULL);

	/* start the event handler */
	glutMainLoop();
	return 0;
}