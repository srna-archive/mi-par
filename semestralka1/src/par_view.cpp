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
	if(argc < 2)
		return usage(argv[0]);

	try
	{
		Instance i(argv[1]);		
		cout << i << endl;
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
	cout << "Usage: " << bin << " file" << endl;
	cout << "file\tstring\t\t\tInput file" << endl;
	return 1;
}
