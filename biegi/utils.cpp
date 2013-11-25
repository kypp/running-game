#include <stdlib.h>




float randFloat(float min=0.0, float max=1.0)
{
	float random = ((float) rand()) / (float) RAND_MAX;
	return min + (random * (max-min));
}