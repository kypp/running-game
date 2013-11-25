#include "objects.h"
#include "GL/glut.h"
#include "drawing.h"
#include "utils.h"
#include "GL/freeglut_ext.h"

Stadium::Stadium( float length, int lanes, float lanesize )
{
	this->length = length;
	this->lanes = lanes;
	this->lanesize = lanesize;
	rin = length/(4*PI);
}

void Stadium::draw()
{
	float rout = rin+lanes*lanesize;
	float mv = length/8;
	glColor3f(1,0,0);
	DrawOval(mv, rout, true);	
	glTranslatef(0,0,-0.01);
	glColor3f(0.3,.8,0.2);
	DrawOval(mv, rin, true);
	glColor3f(0,0,0);	
	glTranslatef(0,0,-0.01);
	for (int i = 0; i <= lanes; i++)
	{
		DrawOval(mv, rin+i*lanesize, false);
	}
}


Tree::Tree(float size, float x, float y, float z )
{
	position.x = x;
	position.y = y;
	position.z = z;
	leafcolor.x = randFloat(0,.3);
	leafcolor.y = randFloat(.7,1);
	leafcolor.z = randFloat(0,.3);
	basecolor.x = randFloat(.5,.9);
	basecolor.y = randFloat(.2,.3);
	basecolor.z = randFloat(0,.2);

	bcount = rand()%MaxBranches;
	branches = new Vector3[bcount];
	for (int i = 0; i < bcount; i++)
	{
		branches[i].x = randFloat(size/4, size*3/4);	//wysokosc galezi
		branches[i].y = rand()%360;						//rotacja
		branches[i].z = randFloat(size/6, size/3);		//wielkosc galezi
	}
	this->size = size;
}

Tree::~Tree()
{
	delete [] branches;
}

void Tree::draw()
{
	glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		drawTree(size);
		for (int i = 0; i < bcount; i++)
		{
			glPushMatrix();
				glTranslatef(0,branches[i].x,0);
				glRotatef(branches[i].y, 0, 1, 0);
				glRotatef(90, 0, 0, 1);
				drawTree(branches[i].z);
			glPopMatrix();
		}		
	glPopMatrix();
}

void Tree::drawTree(float size)
{
	glPushMatrix();
		glRotatef(90, -1, 0 ,0);
		glColor3f(basecolor.x,basecolor.y,basecolor.z);
		if (DrawType<=2)
			glutSolidCylinder(size/15, size, sphereLOD, sphereLOD);
		else {
			glPushMatrix();
				glScalef(2./15.,2./15.,2);
				glutSolidCube(size);
			glPopMatrix();
		}
		glTranslatef(0,0,size);
		glColor3f(leafcolor.x,leafcolor.y,leafcolor.z);
		if (DrawType<=2)
			glutSolidSphere(size/3, sphereLOD, sphereLOD);
		else 
			glutSolidCube(2*size/3);
	glPopMatrix();
}
