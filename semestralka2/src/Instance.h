#ifndef INSTANCE_H_
#define INSTANCE_H_

#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <boost/dynamic_bitset.hpp>

#include "common.h"

class Solution;

class Instance {
public:
	enum Type {DUMB, SMART};
private:
	unsigned int n, z;
	unsigned int m, k;
	Type t;
	boost::dynamic_bitset<> superstring;
	std::vector<boost::dynamic_bitset<> > substrings;
	void generate();
	void readFromStream(std::istream& is);
	void writeToStream(std::ostream& os) const;
	void acceptSolution(const Solution& sol, int source_rank);
public:
	Instance(unsigned int n, unsigned int z, Type t);
	Instance(std::istream& is);
	Instance(const char * filename);
	unsigned int getM() const {return m;}
	unsigned int getK() const {return k;}
	unsigned int getN() const {return n;}
	unsigned int getZ() const {return z;}
	friend std::ostream& operator<< (std::ostream& os, const Instance& i);
	void serialize(const char* filename) const;
	void serialize(std::ostream& os) const;
	friend class Worker;
	const std::vector<boost::dynamic_bitset<> >& getSubstrings() const
		{return substrings;}
};

#endif /* INSTANCE_H_ */
