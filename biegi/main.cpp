//Filip Gromul - 247947
//PGK - Zadanie 4

#include <stdlib.h>
#include <math.h>
#include "GL/glut.h"
#include "biegacz.h"
#include "drawing.h"
#include "utils.h"

#define ZAW 20
#define TCOUNT 20
#define LANES 8

int VIEWPORT_W = 500;
int VIEWPORT_H = 500;
float ratio;
Biegacz *biegacz;
Biegacz *zawodnicy[ZAW];
Tree *drzewa[TCOUNT];
Stadium *stadion;
std::vector<Biegacz*> lane[LANES];

const float rad = 0.01745;
int view = 0;

float test = 0;

void idle()
{
	for (int i = 0; i < ZAW; i++)
	{
		zawodnicy[i]->move();
		if (test>1) zawodnicy[i]->collide(lane);
	}
	test += 0.01f;
	glutPostRedisplay();
}

void camera()
{
	glRotatef(90, 0, 1, 0);
	
	if (view==0)
	{
		glRotatef(-biegacz->rotation, 0, 1, 0);
		glTranslatef(-biegacz->position.x, -1-biegacz->height, -biegacz->position.y);
	}
	if (view==1)
	{
		glTranslatef(.5, -0.1, .4);

		glRotatef(-biegacz->rotation, 0, 1, 0);

		glTranslatef(-biegacz->position.x, -1-biegacz->height, -biegacz->position.y);
	}
	if (view==2)
	{
		int c = 3;

		glTranslatef(2, 0, 0);

		glRotatef(biegacz->rotation, 0, -1, 0);

		glTranslatef(-biegacz->position.x, -2, -biegacz->position.y);
	}
	if (view==3)
	{
		glRotatef(90, 0, -1, 0);
		gluLookAt(0,10,15,0,0,0,0,1,0);
		glRotatef(test*100, 0,1,0);
	}
}

void drawMainView(){

	glScissor(0, 0, (GLsizei) VIEWPORT_W, (GLsizei) VIEWPORT_H);
	glViewport(0, 0, (GLsizei) VIEWPORT_W, (GLsizei) VIEWPORT_H);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(60, (float)VIEWPORT_W/(float)VIEWPORT_H, .1, 200);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	camera();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if (view!=0) {
		glPushMatrix();
			glRotatef(90, 1, 0 ,0);
			glTranslatef(biegacz->position.x, biegacz->position.y, 0);
			glRotatef(90, -1, 0 ,0);
			glRotatef(biegacz->rotation, 0, 1, 0);
			biegacz->draw();		
		glPopMatrix();
	}

	for (int i = 0; i < ZAW; i++)
	{
		if (zawodnicy[i] != biegacz) {
		glPushMatrix();
			glRotatef(90, 1, 0 ,0);
			glTranslatef(zawodnicy[i]->position.x, zawodnicy[i]->position.y, 0);
			glRotatef(90, -1, 0 ,0);
			glRotatef(zawodnicy[i]->rotation, 0, 1, 0);
			zawodnicy[i]->draw();		
		glPopMatrix();
		}
	}

	for (int i = 0; i < TCOUNT; i++)
	{
		drzewa[i]->draw();
	}

	glPushMatrix();
	glRotatef(90, 1, 0 ,0);
	glLineWidth(4);
	stadion->draw();
	glPopMatrix();
}

void drawMinimap()
{
	glScissor((GLsizei) VIEWPORT_W*3/4, 0, (GLsizei) VIEWPORT_W/4, (GLsizei) VIEWPORT_H/5);
	glViewport((GLsizei) VIEWPORT_W*3/4, 0, (GLsizei) VIEWPORT_W/4, (GLsizei) VIEWPORT_H/5);
	//glViewport(0, 0, (GLsizei) VIEWPORT_W, (GLsizei) VIEWPORT_H);
	
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrtho(-1,1,-1,1,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //glutSolidCone(100,100,10,10);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glScalef(.03,.03*ratio,-1);
	glLineWidth(1);
    
	for (int i = 0; i < ZAW; i++)
	{
		glPushMatrix();
		glTranslatef(zawodnicy[i]->position.x, zawodnicy[i]->position.y, 0);
		if (zawodnicy[i] != biegacz)
			glColor3f(1,1,0);
		else
			glColor3f(0,1,0);
		if (!zawodnicy[i]->alive)
			glColor3f(0,0,0);
		glutSolidCube(1);
		glPopMatrix();
	}
	
	stadion->draw();


}

void display()
{
	drawMainView();
	drawMinimap();

	/*glViewport(0,0,VIEWPORT_W/2,VIEWPORT_H/2);
	glutSolidCone(10,10,10,10);

	glViewport(0,0,VIEWPORT_W,VIEWPORT_H);*/

	glutSwapBuffers();

}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
	case 'q':
	case 'Q':
		exit(0);
		break;
	}
	if (key>='0' && key <='3') view = key - '0';
}

void special(int key, int x, int y)
{
	biegacz->resolveinput(key);
}

void reshape(int w, int h)
{
	VIEWPORT_W = w;
	VIEWPORT_H = h;
	ratio = (float)VIEWPORT_W/(float)VIEWPORT_H;
}

void initialize()
{
	stadion = new Stadium(100,LANES,1);

	for (int i = 0; i < ZAW; i++)
	{
		zawodnicy[i] = new Biegacz(stadion, rand()%stadion->lanes+1, i, randFloat(), randFloat(0.01, .35), 0);
	}
	biegacz = zawodnicy[rand()%ZAW];

	for (int i = 0; i < TCOUNT; i++)
	{
		drzewa[i] = new Tree(randFloat(1,4), randFloat(-stadion->length/8,stadion->length/8),0,randFloat(-stadion->rin,stadion->rin));
	}

	for (int i = 0; i < ZAW; i++)
	{
		lane[zawodnicy[i]->lane-1].push_back(zawodnicy[i]); 
	}
}

int main(int argc,char *argv[])
{
	initialize();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(VIEWPORT_W, VIEWPORT_H);
	glutCreateWindow(argv[0]);
	glutDisplayFunc(display);
    glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutIdleFunc(idle);
	glClearColor(.2,.3,1,0);
	glClearDepth(1.0f);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glLineWidth(5);
	glutMainLoop();

	for (int i = 0; i < ZAW; i++)
	{
		delete zawodnicy[i];
	}

	return 0;
}