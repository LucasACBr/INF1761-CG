#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include "windows.h"
#include "GL/gl.h"
#include "GL/glut.h"

static float viewer_pos[4] = { 2.0f,3.0f,4.0f,1.0f };

static float M[16] = { 1.0f,0.0f,0.0f,0.0f,
						0.0f,1.0f,0.0f,0.0f,
						0.0f,0.0f,1.0f,0.0f,
						0.0f,0.0f,0.0f,1.0f };

static float camera_h = 1.0f;

static float x0 , z0;
static float x1 , z1;
static float ya , yb;//Don't have a clue, but somehow putting this as y0 and y1 is preventing the code for running
static float p0[3] = { 0.0f,0.0f,0.0f };
static float p1[3] = { 0.0f,0.0f,0.0f };
static int h = 1200;
static int w = 1200;

void vectorialProduct(float *a, float * p0, float * p1) {

	a[0] = p0[1] * p1[2] - (p0[2] * p1[1]);
	a[1] = (p0[1] * p1[2]) - (p0[2] * p1[1]);
	a[2] = p0[0] * p1[1] - (p0[2] * p1[1]);

	return;
}

float modul(float *p) {

	return (float)sqrt(p[0]*p[0] + p[1]*p[1] + p[2]*p[2]);
}

static void drawCube(float * color) {
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);//Color of material
	glBegin(GL_QUADS);//Gonna draws Quads every 4 coordinates given

		//x+
	glNormal3f(1.0f, 0.0f, 0.0f);//Sets up the normal for the Quads
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);


	//y+
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);


	//z+
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);



	//x-
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);


	//y-
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);


	//z-
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);



	glEnd();
}

static void drawPrism(float * pos, float * dim, float * color) {
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);//Color of material
	glBegin(GL_QUADS);

	//x+
	glNormal3f(1.0f, 0.0f, 0.0f);//Sets up the normal for the Quads
	glVertex3f(pos[0] + dim[0] / 2, pos[1] - dim[1] / 2, pos[2] - dim[2] / 2);
	glVertex3f(pos[0] + dim[0] / 2, pos[1] - dim[1] / 2, pos[2] + dim[2] / 2);
	glVertex3f(pos[0] + dim[0] / 2, pos[1] + dim[1] / 2, pos[2] + dim[2] / 2);
	glVertex3f(pos[0] + dim[0] / 2, pos[1] + dim[1] / 2, pos[2] - dim[2] / 2);


	//y+
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(pos[0] - dim[0] / 2, pos[1] + dim[1] / 2, pos[2] - dim[2] / 2);
	glVertex3f(pos[0] - dim[0] / 2, pos[1] + dim[1] / 2, pos[2] + dim[2] / 2);
	glVertex3f(pos[0] + dim[0] / 2, pos[1] + dim[1] / 2, pos[2] + dim[2] / 2);
	glVertex3f(pos[0] + dim[0] / 2, pos[1] + dim[1] / 2, pos[2] - dim[2] / 2);


	//z+
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(pos[0] - dim[0] / 2, pos[1] - dim[1] / 2, pos[2] + dim[2] / 2);
	glVertex3f(pos[0] - dim[0] / 2, pos[1] + dim[1] / 2, pos[2] + dim[2] / 2);
	glVertex3f(pos[0] + dim[0] / 2, pos[1] + dim[1] / 2, pos[2] + dim[2] / 2);
	glVertex3f(pos[0] + dim[0] / 2, pos[1] - dim[1] / 2, pos[2] + dim[2] / 2);



	//x-
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(pos[0] - dim[0] / 2, pos[1] - dim[1] / 2, pos[2] - dim[2] / 2);
	glVertex3f(pos[0] - dim[0] / 2, pos[1] - dim[1] / 2, pos[2] + dim[2] / 2);
	glVertex3f(pos[0] - dim[0] / 2, pos[1] + dim[1] / 2, pos[2] + dim[2] / 2);
	glVertex3f(pos[0] - dim[0] / 2, pos[1] + dim[1] / 2, pos[2] - dim[2] / 2);


	//y-
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(pos[0] - dim[0] / 2, pos[1] - dim[1] / 2, pos[2] - dim[2] / 2);
	glVertex3f(pos[0] - dim[0] / 2, pos[1] - dim[1] / 2, pos[2] + dim[2] / 2);
	glVertex3f(pos[0] + dim[0] / 2, pos[1] - dim[1] / 2, pos[2] + dim[2] / 2);
	glVertex3f(pos[0] + dim[0] / 2, pos[1] - dim[1] / 2, pos[2] - dim[2] / 2);


	//z-
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(pos[0] - dim[0] / 2, pos[1] - dim[1] / 2, pos[2] - dim[2] / 2);
	glVertex3f(pos[0] - dim[0] / 2, pos[1] + dim[1] / 2, pos[2] - dim[2] / 2);
	glVertex3f(pos[0] + dim[0] / 2, pos[1] + dim[1] / 2, pos[2] - dim[2] / 2);
	glVertex3f(pos[0] + dim[0] / 2, pos[1] - dim[1] / 2, pos[2] - dim[2] / 2);

	glEnd();
}

static void drawSphere(float r, int lats, int longs) {
	float PI = 3.14159265;
	int i, j;
	for (i = 0; i <= lats; i++) {
		float lat0 = PI * (-0.5 + (float)(i - 1) / lats);
		float z0 = sinf(lat0);
		float zr0 = cosf(lat0);

		float lat1 = PI * (-0.5 + (float)i / lats);
		float z1 = sinf(lat1);
		float zr1 = cosf(lat1);

		glBegin(GL_QUAD_STRIP);
		for (j = 0; j <= longs; j++) {
			float lng = 2 * PI *(float)(j - 1) / longs;
			float x = cosf(lng);
			float y = sinf(lng);

			glNormal3f(x*zr0, y*zr0, z0);
			glVertex3f(r*x*zr0, r*y*zr0, r*z0);
			glNormal3f(x*zr1, y*zr1, zr1);
			glVertex3f(r*x*zr1, r*y*zr1, r*z1);
		}
		glEnd();
	}
}

void setCamera(void) {
	int vp[4]; // Viewport dimension :{x0,ya,w,h}
	float ratio;
	glGetIntegerv(GL_VIEWPORT, vp);//Returns the values of the Window
	ratio = (float)vp[2] / vp[3];//Having got the height and width of the window, it now divides them to get the ratio


	//Setting up the camera parameters
	glMatrixMode(GL_PROJECTION);//Set up the current MatrixMode
	glLoadIdentity();
	gluPerspective(45.0f, ratio, 1.0f, 20.0f);//Sets up in this order, the Angle, the Ratio, the near and far focus of the perspective

	//Setting the camera's placement and what is it looking at
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(M);
	gluLookAt(viewer_pos[0], viewer_pos[1], viewer_pos[2],//Sets up the x,y,z of where the camera is placed
		0.0f, 0.0f, 0.0f,//Sets up where the camera is pointed at
		0.0f, camera_h, 0.0f);//setps um a vetor that moves the camerea, likely after is has centered on the point it's supposed to look at, so it gains a another angle
}

void drawScene(void) {

	//setting up light source
	float amb[] = { 0.1f,0.1f,0.1f,1.0f };//Ambient light, light that sorta comes from everywhere
	float dif[] = { 0.7f,0.7f,0.7f,1.0f };//Diffuse Light, light that comes from an specific point

	glLightfv(GL_LIGHT0, GL_POSITION, viewer_pos);//Position of the light
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);//Setting up ambiente light
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);//Setting up diffuse light



//	drawCube(red);

	float pos[] = { 0.0f,0.0f,0.0f };
	float dim[] = { 1.0f,0.05f,1.0f };
	float grey[] = { 0.5f,0.5f,0.5f,0.5f };
	drawPrism(pos, dim, grey);//Table

	float pos1[] = { 0.45f,-0.5f,0.45f };
	float dim1[] = { 0.1f,1.0f,0.1f };

	drawPrism(pos1, dim1, grey);//Leg
	pos1[0] *= -1;
	drawPrism(pos1, dim1, grey);
	pos1[2] *= -1;
	drawPrism(pos1, dim1, grey);
	pos1[0] *= -1;
	drawPrism(pos1, dim1, grey);

	float pos2[] = { 0.0f,0.2f,0.0f };
	float dim2[] = { 0.3f,0.3f,0.3f };
	float red[] = { 1.0f,0.0f,0.0f,1.0f };//Red Color, no Transparency
	glScalef(0.5f, 0.5f, 0.5f);
	drawPrism(pos2, dim2, red);

	glTranslatef(0.4f, 0.3f, 0.3f);
	glScalef(0.2f, 0.2f, 0.2f);
	float blue[] = { 0.0f,0.0f,1.0f,0.7f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);//Color of material

	drawSphere(1.0f,30,30);

}

static void displayCB(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	setCamera();//Positions the Camera
	drawScene();//Draws the Scene

	glutSwapBuffers();//Updates the Screen, equivalent to glutFlush()
}

static void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();//Does the Display again, but now with the changes we added
}

static void motionCB(int x, int y) {
	float d = viewer_pos[2];
	float a[3] = { 0.0f,0.0f,0.0f };
	float theta;
	float temp;
	float ax, ay, az;
	float r = h / 2;



	x1 = (x - w / 2) / r;//Screen X becomes Arcball X
	yb = (y - h / 2) / r;//Same here
	temp = 1-(x1 * x1 + yb * yb);
	if (sqrt(temp) > 1) {
		x1 = x1 / sqrt(temp);
		yb = yb / sqrt(temp);
		z1 = 0.0f;
	}
	else {
		temp = x1 * x1 + (yb * yb);
		temp = 1- pow(sqrt(temp), 2);
		z1 = sqrt(temp);
	}
	p1[0] = x1;
	p1[1] = yb;
	p1[2] = z1;
	vectorialProduct(a,p0, p1);
	theta = 2*asin(modul(a));

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0, 0, d);
	glRotatef(theta, a[0], a[1], a[2]);
	glTranslatef(0, 0, -d);
	glMultMatrixf(M);
	glGetFloatv(GL_MODELVIEW_MATRIX, M);
	glPopMatrix();


	glutPostRedisplay();
}


static void mouseCB(int button, int state, int x, int y) {
	float temp;
	float r;
	if (button != GLUT_LEFT_BUTTON) {
		return;
	}
	if (state == GLUT_DOWN) {
		r = (float)(min(h, w)) / 2;

		x0 = (x - w / 2) / r;//Screen X becomes Arcball X
		ya = (y - h / 2) / r;//Same here
		temp = 1 - (x0 * x0 + ya * ya);
		if (sqrt(temp) > 1) {
			x0 = x0/sqrt(temp);
			ya = ya/sqrt(temp);
			z0 = 0.0f;
		}
		else {
			temp = x0 * x0 + ya * ya;
			temp = 1 - pow(sqrt(temp), 2);
			z0 = sqrt(temp);
		}


		p0[0] = x0;
		p0[1] = ya;
		p0[2] = z0;
		glutMotionFunc(motionCB);
	}
	else {
		glutMotionFunc(NULL);
	}
}

void initialize(void) {

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//Black background
	glColor3f(1.0f, 0.0f, 0.0f);//Red drawing color

	glEnable(GL_DEPTH_TEST);//Enables Depth Test

	glEnable(GL_LIGHTING);//Enables lighting calculation and computation

	glEnable(GL_LIGHT0);//Enables light source 0
}

int main(int argc, char * argv[]) {

	glutInit(&argc, argv);


	//Window Configurations
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(w, h);
	glutCreateWindow("T3");

	//initilization
	initialize();

	//Callback Configuration
	glutDisplayFunc(displayCB);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseCB);

	//Loop
	glutMainLoop();
	return 0;
}