#include "common.h"

int RAND(int min, int max)
{
	return min + rand() % (max+1-min);
}
