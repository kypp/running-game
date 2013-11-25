#include <stdlib.h>
#include <math.h>
#include "GL/glut.h"
#include "drawing.h"
#include "GL/freeglut_ext.h"
#include "utils.h"


void line(GLdouble len)
{
	glBegin(GL_LINES);
		glVertex2d(0,0);
		glVertex2d(0,len);
	glEnd();
}

void linem(GLdouble len)
{
	line(len);
	glTranslatef(0,len,0);
}

void DrawEllipse(float radiusX, float radiusY)
{
	int i;

	glBegin(GL_LINE_LOOP);

	for(i=0;i<360;i++)
	{
		float rad = i*DEG2RAD;
		glVertex2f(cos(rad)*radiusX,
			sin(rad)*radiusY);
	}

	glEnd();
}

void DrawSegment(float length, int type)
{
	switch (type)
	{
	case 0:
		linem(length);
		break;
	case 1:
		glTranslatef(0,length/2,0);
		glPushMatrix();
			glScalef(.5, 1, .5);
			glutSolidSphere(length/2, sphereLOD, sphereLOD);
		glPopMatrix();
		glTranslatef(0,length/2,0);		
		break;
	case 2:
		glTranslatef(0,length/2,0);
		glPushMatrix();
			glScalef(.5, 1, .5);
			glutWireSphere(length/2, sphereLOD, sphereLOD);
		glPopMatrix();
		glTranslatef(0,length/2,0);		
		break;
	case 3:
		glTranslatef(0,length/2,0);
		glPushMatrix();
			glScalef(.5, 2, .5);
			glutSolidCube(length/2);
		glPopMatrix();
		glTranslatef(0,length/2,0);		
		break;
	default:
		break;
	}
}


void DrawOval(float size, float r, bool filled)
{
	glPushMatrix();
		if (filled) {
			glBegin(GL_QUADS);
				glVertex2f(-size, r);
				glVertex2f(size, r);
				glVertex2f(size, -r);
				glVertex2f(-size, -r);
			glEnd();
			glBegin(GL_TRIANGLE_FAN);
				glVertex2f(size, 0);
				for(int i=0;i<181;i++)
				{
					float rad = i*DEG2RAD;
					glVertex2f(size+sin(rad)*r, cos(rad)*r);
				}
			glEnd();
			glBegin(GL_TRIANGLE_FAN);
				glVertex2f(size, 0);
				for(int i=0;i<181;i++)
				{
					float rad = i*DEG2RAD;
					glVertex2f(-size-sin(rad)*r, -cos(rad)*r);
				}
			glEnd();
		}
		else
		{
			glBegin(GL_LINE_STRIP);
				glVertex2f(size, r);
				for(int i=1;i<181;i++)
				{
					float rad = i*DEG2RAD;
					glVertex2f(size+sin(rad)*r, cos(rad)*r);
				}
				glVertex2f(size, -r);
				for(int i=1;i<181;i++)
				{
					float rad = i*DEG2RAD;
					glVertex2f(-size-sin(rad)*r, -cos(rad)*r);
				}
				glVertex2f(size, r);
			glEnd();
		}

	glPopMatrix();
}

