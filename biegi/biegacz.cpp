#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "drawing.h"
#include "biegacz.h"


Biegacz::Biegacz( Stadium *stadion,int lane, int num, float ph/*=0 */,float vel/*=.05*/,float acc/*=0*/ )
{
	phase = ph;
	alive = true;
	velocity = vel;
	acceleration = acc;
	this->stadion = stadion;
	this->lane = lane;
	newlane = lane;
	lineposition = 0;
	position.x = 0;
	position.y = 0;
	position.z = 0;
	rotation = 0;
	r = stadion->length/(4*PI)+((float)lane-0.5)*stadion->lanesize;
	number = num;
}

Biegacz::~Biegacz()
{
}

void Biegacz::move()
{	
	phase += velocity + (phase+velocity>2*PI?-phase:0);	

	if (!alive) return;

	height = fabs(sin(phase+.6))/10;

	if (lane != newlane)
	{
		r += (newlane>lane?1:-1)*((float)lane-0.5)*stadion->lanesize/100;

		if (newlane>lane && 
			r>=stadion->length/(4*PI)+((float)newlane-0.5)*stadion->lanesize
			||
			newlane<lane && 
			r<=stadion->length/(4*PI)+((float)newlane-0.5)*stadion->lanesize) {

			lane = newlane;
			r = stadion->length/(4*PI)+((float)lane-0.5)*stadion->lanesize;
		}
	}

	lineposition += velocity;
	float l = stadion->length/4;
	float arclen = r*PI;
	if (lineposition<l)
	{
		position.x = lineposition-l/2;
		position.y = -r;
		rotation = 0;
	}
	else if (lineposition < l+arclen)
	{
		position.x = l/2+r*sin((lineposition-l)/(r));
		position.y = -r*cos((lineposition-l)/(r));
		rotation = -180*(lineposition-l)/(PI*r);
	}
	else if (lineposition < 2*l+arclen)
	{
		position.x = -(lineposition-1.5*l-arclen);
		position.y = r;
		rotation = 180;
	}
	else
	{
		position.x = -l/2-r*sin((lineposition-2*l-arclen)/(r));
		position.y = r*cos((lineposition-2*l-arclen)/(r));
		rotation =180 - 180*(lineposition-(2*l+arclen))/(PI*r);
	}
	//printf("%f\n", arclen);
	if (lineposition > 2*l+2*arclen) 
		lineposition -= 2*l+2*arclen;
}

void Biegacz::draw()
{
	glPushMatrix();
		if (!alive) glRotatef(180,0,0,1);
		else
			glTranslatef(0, .85, 0);
		glColor3f(0,0,0);
		glTranslatef(0,-.25+height,0);
	
		//glColor3f(0.3,0.3,0.3);
		noga(0, true);						//Noga 1
		noga(PI, false);					//Noga 2
		glRotated(-5*velocity*20*HA*(1+(abs(sin(phase)))/3),0,0,1);			//Pochylenie tulowia
		DrawSegment(.45, DrawType);				//Tolow
		reka(0, true);						//Reka 1
		reka(PI, false);					//Reka 2
		glColor3f(0,0,0);
		DrawSegment(.03, DrawType);				//Szyja
		glowa(.11);				//Glowa
	glPopMatrix();
}

void Biegacz::reka( GLdouble phaseshift, bool lewa )
{	
	glPushMatrix();
		glRotatef(30, (lewa?1:-1),0,0);
		glRotatef(sin(phase+phaseshift)*hands[0][0]+hands[0][1], 0,0,1);
		DrawSegment(.3, DrawType);
		glPushMatrix();
			glRotatef(sin(phase+phaseshift)*hands[1][0]*(velocity>.1?velocity*10:1)+hands[1][1], 0,0,1);			
			DrawSegment(.2, DrawType);
			glPushMatrix();
				glRotatef(sin(phase+phaseshift)*hands[2][0]+hands[2][1], 0,0,1);
				DrawSegment(.1, DrawType);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}

void Biegacz::noga( GLdouble phaseshift, bool lewa )
{
	glPushMatrix();
		glRotatef(10, (lewa?1:-1),0,0);
		glRotatef(sin(phase+phaseshift)*legs[0][0]*(velocity>.1?velocity*10:1)+legs[0][1], 0,0,1);
		DrawSegment(.3, DrawType);
		glPushMatrix();
			//glRotatef(10, (lewa?1:-1),0,0);
			glRotatef(sin(phase+phaseshift)*legs[1][0]+legs[1][1], 0,0,1);
			DrawSegment(.3, DrawType);
			glPushMatrix();
				glRotatef(sin(phase+phaseshift)*legs[2][0]+legs[2][1], 0,0,1);
				DrawSegment(.1, DrawType);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}

void Biegacz::glowa(GLdouble size)
{
	glPushMatrix();
		glTranslatef(0,size,0);
		glutSolidSphere(size, sphereLOD, sphereLOD);
	glPopMatrix();
}

void Biegacz::accelerate(float vel)
{
	velocity += vel;
}

void Biegacz::resolveinput( int c )
{

	float l = stadion->length/4;
	float arclen = r*PI;

	switch (c)
	{
	case GLUT_KEY_UP:
		accelerate(.002);
		break;
	case GLUT_KEY_DOWN:
		accelerate(-.002);
		break;
	case GLUT_KEY_LEFT:		
		if (lane<stadion->lanes) 
		{
			/*float nextr = (stadion->length/(4*PI)+((float)lane+0.5)*stadion->lanesize);

			if (lineposition < l) {}
			else if (lineposition < l+arclen)
			{
				lineposition = l + (lineposition-l)*nextr/r;
			}
			else if (lineposition < 2*l+arclen)
			{
				lineposition = 2*l + (lineposition-2*l)*nextr/r;
			}
			else
			{
				lineposition = 2*l + nextr + (lineposition-2*l-arclen)*nextr/r;
			}	
			lane++;*/
			if (lane==newlane) newlane++;
		}

		break;	
	case GLUT_KEY_RIGHT:		
		if (lane>1)
		{/*
			float prevr = (stadion->length/(4*PI)+((float)lane-1.5)*stadion->lanesize);

			if (lineposition < l) {}
			else if (lineposition < l+arclen)
			{
				lineposition = l + (lineposition-l)*prevr/r;
			}
			else if (lineposition < 2*l+arclen)
			{
				lineposition = 2*l + (lineposition-2*l)*prevr/r;
			}
			else
			{
				lineposition = 2*l + prevr + (lineposition-2*l-arclen)*prevr/r;
			}		
				
			lane--;*/
			if (lane==newlane) newlane--;
		}
		break;
	}

	//r = stadion->length/(4*PI)+((float)lane-0.5)*stadion->lanesize;
}

void Biegacz::collide( std::vector<Biegacz*> *lane )
{
	std::vector<Biegacz*> *actlane = &lane[this->newlane-1];
	std::vector<Biegacz*> *oldlane = &lane[this->lane-1];
	int k = -1;
	if (this->lane!=this->newlane)
	{
		for (int i = 0; i < oldlane->size(); i++)
			if((*oldlane)[i]->number==this->number)
				k=i;
		if (k>=0)
		{
			printf("%d\n", k);
			oldlane->erase(oldlane->begin()+k);
			actlane->push_back(this);
		}
	}
	bool a = false;
	
	for (int i = 0; i < actlane->size(); i++)
	{
		if ((*actlane)[i]->lineposition>lineposition && (*actlane)[i]->lineposition < lineposition+2 && ((*actlane)[i]->velocity<velocity || !(*actlane)[i]->alive)) {
			a = true;
			break;
		}
	}	

	if (a) 
	{
		alive = false;
	}
}
