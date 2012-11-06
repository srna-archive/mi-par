#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <ctime>

#include "Instance.h"

using namespace std;

int usage(char *bin);

int main(int argc, char **argv)
{
	unsigned int n, z;

	if(argc < 4)
		return usage(argv[0]);

	n = atoi(argv[2]);
	z = atoi(argv[3]);

	if(n == 0 || z == 0)
		return usage(argv[0]);
	
	Instance::Type t;
	
	if(argc > 4)
	{
		if(strcasecmp("DUMB", argv[4]) == 0)
			t = Instance::DUMB;
		else if (strcasecmp("SMART", argv[4]) == 0)
			t = Instance::SMART;
		else
			return usage(argv[0]);
	}
	else
		t = Instance::SMART;
	
	try
	{
		Instance i(n, z, t);
		
		cout << i << endl;
		i.serialize(argv[1]);
	}
	catch (const char * e)
	{
		cout << e << endl;
		return 1;
	}

	return 0;
}

int usage(char *bin)
{
	cout << "Usage: " << bin << " file n z [t]" << endl;
	cout << "file\tstring\t\t\tOutput file" << endl;
	cout << "n\tinteger, n >= 10\tNumber of strings" << endl;
	cout << "z\tinteger, z >  log2(n)\tMaximum length of strings" << endl;
	cout << "t\tSMART | DUMB\t\tType of generated problem" << endl;
	return 1;
}
