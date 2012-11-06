#include "BinaryString.h"

using namespace std;

BinaryString::BinaryString()
{
	len = 0;
}

BinaryString::~BinaryString()
{}

BinaryString::Offset BinaryString::offset(unsigned int b, unsigned int e)
{
	return BinaryString::Offset(*this, b, e);
}

void BinaryString::clear()
{
	len = 0;
	data.clear();
}

void BinaryString::generate(unsigned int len) : len(len)
{
	char *temp = new char[len/8+1+1];
	
	
	
	delete temp;
}

ostream& operator<<(ostream& os, const BinaryString& bs)
{
	unsigned int i;
	for(i=0; i<bs.data.size(); ++i)
	{
		os << bs.data[i];
	}
	return os;
}
