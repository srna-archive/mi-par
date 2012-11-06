#ifndef SOLUTION_H_
#define SOLUTION_H_

#include <iostream>
#include <vector>
#include <stack>
#include <boost/dynamic_bitset.hpp>
#include "Instance.h"

class Solution
{
private:
	const Instance& i;
	std::vector<int> offsets;
	boost::dynamic_bitset<> superstring;
	bool solved;
public:
	Solution(const Instance& i);
	friend std::ostream& operator<<(std::ostream& os, const Solution& s);
	void permute(void);
	bool test(boost::dynamic_bitset<>& bs);
	bool is_substring(const boost::dynamic_bitset<>& substr,
		const boost::dynamic_bitset<>& superstr, unsigned int offset);
	const Instance& getInstance() const {return i;}
};

#endif /* SOLUTION_H_ */
