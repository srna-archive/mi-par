#ifndef BINARYSTRING_H_
#define BINARYSTRING_H_

#include <boost/dynamic_bitset.hpp>
#include <iostream>

#include "common.h"

class BinaryString
{
private:
	boost::dynamic_bitset<> data;
	unsigned int len;
	class Offset
	{
	private:
		unsigned int b, e;
		const BinaryString& bs;
	public:
		Offset(const BinaryString& bs, unsigned int b, unsigned int e) 
			: b(b), e(e), bs(bs) {if(b>=e) throw "b must be less than e";}
		unsigned int begin() {return b;}
		unsigned int end() {return e;}
	};
public:
	BinaryString();
	virtual ~BinaryString();
	Offset offset(unsigned int b, unsigned int e);
	void clear();
	void generate(unsigned int len);
	friend std::ostream& operator<<(std::ostream& os, const BinaryString& bs);
};

#endif /* BINARYSTRING_H_ */
