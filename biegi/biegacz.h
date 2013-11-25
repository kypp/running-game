#include "GL/glut.h"
#include "objects.h"
#include <vector>

#define HA 1
const float hands[][2] = {
	{30*HA, 170},				//ramie
	{10*HA, 110},				//lokiec
	{1*HA, 10}					//nadgarstek
};
const float legs[][2] = {
	{25*HA, 180},				//udo
	{60*HA, -60},				//kolano
	{-10*HA, 90}				//stopa
};


class Biegacz
{
public:
	Biegacz(Stadium *stadion, int lane, int num, float ph=0 ,float vel=.05,float acc=0);
	~Biegacz();
	void move();
	void draw();
	void accelerate(float);
	void collide( std::vector<Biegacz*> *lane );
	Vector3 position;
	float rotation;
	float height;
	void resolveinput(int c);
	int lane;
	int newlane;
	float lineposition;
	bool alive;
	int number;

private:
	void reka(GLdouble phaseshift, bool lewa);
	void noga(GLdouble phaseshift, bool lewa);
	void glowa(GLdouble size);
	float phase; //0
	float velocity; //.05
	float acceleration;
	float r;
	Stadium *stadion;
};