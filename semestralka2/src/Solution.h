#ifndef SOLUTION_H_
#define SOLUTION_H_

#include <iostream>
#include <vector>
#include <boost/dynamic_bitset.hpp>
#include <string>
#include <sstream>
#include "Instance.h"

class Solution
{
private:
	const Instance& i;
	std::vector<int> offsets;
	boost::dynamic_bitset<> superstring;
	bool solved;
	void writeToStream(std::ostream& os) const;
	void readFromStream(std::istream& is);
public:
	Solution(const Instance& i) : i(i) {solved=false;}
	Solution(const Instance& i, std::istream& is) : i(i) {solved=false;
		readFromStream(is);}
	friend std::ostream& operator<<(std::ostream& os, const Solution& s);
	const Instance& getInstance() const {return i;}
	void setSolution(const boost::dynamic_bitset<>& sol, 
		const std::vector<int>& offs)
		{superstring = sol; offsets = offs; solved = (superstring.size() > 0);}
	bool isSolved(void) {return solved;}
	const boost::dynamic_bitset<>& getSolution(void) const {return superstring;}
	std::string serialize() const;
};

#endif /* SOLUTION_H_ */
