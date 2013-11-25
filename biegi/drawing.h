#include "GL/glut.h"
static const float PI = 3.1415926;
const float DEG2RAD = 3.14159/180.0;
const int sphereLOD = 7;

void linem(GLdouble len);
void line(GLdouble len);
void DrawEllipse(float radiusX, float radiusY);
void DrawSegment(float length, int type);
void DrawOval(float size, float r, bool filled);