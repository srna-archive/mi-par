#include "Instance.h"

using namespace std;
using namespace boost;

Instance::Instance(unsigned int n, unsigned int z, Type t) : n(n), z(z), t(t) {
	if(n < 10)
		throw "Condition failed: n >= 10";
	if(!(z > log2(n)))
		throw "Condition failed: z > log2(n)";
	m = 0;
	k = 0;
	generate();
}

ostream& operator << (ostream& os, const Instance& i)
{
	os << "n = " << i.n << endl;
	os << "z = " << i.z << endl;
	os << "t = " << (i.t == Instance::SMART ? "SMART" : "DUMB") << endl;
	os << "m = " << i.m << endl;
	os << "k = " << i.k << endl;
	os << "substrings:" << endl;
	vector<dynamic_bitset<> >::const_iterator it;
	for(it=i.substrings.begin(); it!=i.substrings.end(); ++it)
	{
		cout << *it << endl;
	}
	os << "superstring:" << endl;
	if(i.superstring.size() > 0)
		os << i.superstring << endl;
	else
		os << "(undefined)" << endl;
	return os;
}

void Instance::generate()
{
	unsigned int i,j;
	unsigned int seed;
	superstring.clear();
	substrings.clear();
	
	FILE* f = fopen("/dev/urandom", "r");
	fread(&seed, sizeof(unsigned int), 1, f);
	fclose(f);
	
	srand(seed);
	
	// Length of generated string in bits
	unsigned int l;
	// Temporary BitSet
	boost::dynamic_bitset<> bs;
	
	if(t == DUMB)
	{
		// Generate substrings
		for(i=0; i<n; ++i)
		{
			l = RAND((int)log2(n)+1, z);
			bs.resize(0);
			for(j=0;j<l;++j)
				bs.push_back(RAND(0,1));
			if(m < bs.size())
				m = bs.size();
			k += bs.size();
			substrings.push_back(bs);
		}
		k = RAND(m+1, k/2-1);
	}
	else
	{
		// Generating the smart way is a bit harder...
		
		// At first, generate lengths, count m and k
		vector<unsigned int> lengths;
		for(i=0; i<n; ++i)
		{
			lengths.push_back(l = RAND((int)log2(n)+1, z));
			if(m < l)
				m = l;
			k+=l;
		}
		k = RAND(m+1, k/2-1);
		
		// Then generate superstring of length k
		for(j=0;j<k;++j)
			superstring.push_back(RAND(0,1));
		
		// And finally, generate the substrings
		unsigned int s; // start
		for(i=0; i<n; ++i)
		{
			bs.resize(0);
			s = RAND(0,k-lengths[i]);
			for(j=0;j<lengths[i];++j)
				bs.push_back(superstring.test(j+s));
			substrings.push_back(bs);
		}
	}
}

Instance::Instance(const char * filename)
{
	ifstream ifs;
	ifs.open(filename, ios::binary);
	if(!ifs.is_open())
		throw "Error opening input file.";
	
	// Check the PAR header
	char check[4];
	check[3] = '\0';
	ifs.read(check, 3);
	if(string(check) != "PAR")
		throw "Invalid file format.";
	
	// Read instance type
	char _t;
	ifs.get(_t);
	if(_t == 'D')
		t = DUMB;
	else if(_t == 'S')
		t = SMART;
	else
		throw "Invalid instance type.";
	
	// Read n, z, m, k
	ifs.read((char*)&n, sizeof(unsigned int));
	ifs.read((char*)&z, sizeof(unsigned int));
	ifs.read((char*)&m, sizeof(unsigned int));
	ifs.read((char*)&k, sizeof(unsigned int));
	
	unsigned int nextl, i, j;
	unsigned char nextc;
	for(i=0; i<n; ++i)
	{
		ifs.read((char*)&nextl, sizeof(unsigned int));
		if(nextl > 0)
		{
			boost::dynamic_bitset<> bs;
			bs.resize(nextl);
			for(j=0; j<nextl; ++j)
			{
				ifs.read((char*)&nextc, 1);
				if(nextc == '0') bs[nextl-j-1] = false;
				else if(nextc == '1') bs[nextl-j-1] = true;
				else
					throw "Read error";
			}
			substrings.push_back(bs);
		}
	}
	
	if(ifs.good() && t == SMART)
	{
		ifs.read((char*)&nextl, sizeof(unsigned int));
		if(nextl > 0)
		{
			for(j=0; j<nextl; ++j)
			{
				ifs.read((char*)&nextc, 1);
				superstring.resize(nextl);
				if(nextc == '0') superstring[nextl-j-1] = false;
				else if(nextc == '1') superstring[nextl-j-1] = true;
				else
					throw "Read error";
			}
		}
	}
	
	ifs.close();
}

void Instance::serialize(const char* filename) const
{
	ofstream ofs;
	ofs.open(filename, ios::out | ios::binary | ios::trunc);
	if(!ofs.is_open())
		throw "Error opening output file.";
	ofs << "PAR";
	
	if(t == SMART)
		ofs << "S";
	else
		ofs << "D";
	
	ofs.write((char*)&n, sizeof(int));
	ofs.write((char*)&z, sizeof(int));
	ofs.write((char*)&m, sizeof(int));
	ofs.write((char*)&k, sizeof(int));
	
	unsigned int l;
	
	vector<boost::dynamic_bitset<> >::const_iterator it;
	for(it=substrings.begin(); it != substrings.end(); ++it)
	{
		// Now we focus on bitset (*it)
		const boost::dynamic_bitset<>& bs = *it;
		l = bs.size();
		
		// Write the bit length of bitset
		ofs.write((char*)&l, sizeof(unsigned int));
		
		ofs << bs;
	}
	
	// If we have, write superstring
	if((l = superstring.size()) > 0)
	{
		ofs.write((char*)&l, sizeof(unsigned int));
		ofs << superstring;
	}
	
	ofs.close();
}
