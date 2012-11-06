#include "Solution.h"

using namespace std;

Solution::Solution(const Instance& i) : i(i)
{
	solved = false;
	permute();
}

ostream& operator<<(std::ostream& os, const Solution& s)
{
	unsigned int i,j;

	if(s.solved)
	{
		const vector<boost::dynamic_bitset<> >& substrings = 
			s.getInstance().getSubstrings();
		os << s.superstring << endl;
		for(i=0; i<s.superstring.size(); ++i)
			os << "-";
		os << endl;
		for(i=0; i<substrings.size(); ++i)
		{
			for(j=0; 
				j<(s.superstring.size()-s.offsets[i]-substrings[i].size()); 
				++j)
				os << " ";
			os << substrings[i] << endl;
		}
	}
	else
		os << "No solution found.";
	
	return os;
}

void Solution::permute(void)
{
	stack<boost::dynamic_bitset<>*> s;
	boost::dynamic_bitset<> *bs0, *bs1;
	
	s.push(new boost::dynamic_bitset<>());
	
	while(!s.empty())
	{
		if(s.top()->size() >= i.getK())
		{
			delete s.top();
			s.pop();
			continue;
		}
		bs0 = new boost::dynamic_bitset<>(*s.top());
		bs1 = new boost::dynamic_bitset<>(*s.top());
		
		delete s.top();
		s.pop();
		
		bs0->push_back(false);
		bs1->push_back(true);
		
		test(*bs0);
		test(*bs1);
		
		s.push(bs0);
		s.push(bs1);
	}
}

bool Solution::test(boost::dynamic_bitset<>& bs)
{
	// Results for current superstring
	vector<int> offs;
	unsigned int j,o;
	
	// We have to test all substrings
	for(j=0; j<i.substrings.size(); ++j)
	{
		// Set it to -1 (not found)
		offs.push_back(-1);
		const boost::dynamic_bitset<>& substr = i.substrings[j];
		
		// Have to try all possible offsets
		for(o=0; bs.size() >= substr.size() && 
			o<=(bs.size()-substr.size()) && offs[j]<0; ++o)
		{
			// Test, if we found substring
			if(is_substring(substr, bs, o))
				offs[j] = o;
		}
	}
	
	// Test, if all offsets are found
	vector<int>::const_iterator it;
	for(it=offs.begin(); it != offs.end(); ++it)
	{
		if(*it < 0)
			return false;
	}
	
	// Now we have a(t least one) solution
	solved = true;
	
	// Get the shortest superstring
	if(superstring.size() < 1 || bs.size() < superstring.size())
	{
		superstring = bs;
		offsets = offs;
	}
	
	return true;
}

bool Solution::is_substring(const boost::dynamic_bitset<>& substr,
	const boost::dynamic_bitset<>& superstr, unsigned int offset)
{
	unsigned int i;
	if(superstr.size()-offset < substr.size())
		return false;
	
	for(i=0; i<substr.size(); ++i)
	{
		if(superstr.test(i+offset) != substr.test(i))
			return false;
	}
	
	return true;
}
