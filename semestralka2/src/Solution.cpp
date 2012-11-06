#include "Solution.h"

using namespace std;

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
		os << "No solution.";
	
	return os;
}

void Solution::writeToStream(std::ostream& os) const
{
	unsigned int j;
	
	// Header
	os << "SOL";
	
	//Write offsets
	for(j = 0; j < offsets.size(); ++j)
		os.write((char*)&offsets[j], sizeof(int));
	
	unsigned int l = superstring.size();
	os.write((char*)&l, sizeof(unsigned int));
	os << superstring;
}

void Solution::readFromStream(std::istream& is)
{
	unsigned int bssize;
	boost::dynamic_bitset<> sstr;
	std::vector<int> offs;
	// Check the SOL header
	char check[4];
	char c;
	check[3] = '\0';
	is.read(check, 3);
	if(string(check) != "SOL")
		throw "Invalid stream format.";
	
	offs.resize(i.getN());
	
	unsigned int j;
	for(j = 0; j < i.getN(); ++j)
		is.read((char*)&offs[j], sizeof(int));
	
	is.read((char*)&bssize, sizeof(bssize));
	
	sstr.resize(bssize);
	for(j=0;j<bssize;++j)
	{
		is.read((char*)&c, 1);
		if(c == '0') sstr[bssize-j-1] = false;
		else if(c == '1') sstr[bssize-j-1] = true;
	}
	
	setSolution(sstr, offs);
}

string Solution::serialize(void) const
{
	stringstream ss;
	writeToStream(ss);
	return ss.str();
}
