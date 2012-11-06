#include "Worker.h"

using namespace std;

Worker::Worker(Instance *i) : pi(i)
{
	if(pi == NULL)
		changeState(WAITING_FOR_INSTANCE);
	else
		changeState(BROADCASTING_INSTANCE);
	
	// Get my rank
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	// Get process count
	MPI_Comm_size(MPI_COMM_WORLD, &p_num);
	
	if(pi != NULL)
		ps = new Solution(*pi);
	else ps = NULL;
	
	_nextDonor = (my_rank + 1) % p_num;
	color = WHITE;
	if(my_rank == 0)
		token = WHITE;
	else
		token = UNDEFINED;
}

void Worker::changeState(State s)
{
	state = s;
#ifdef DEBUG
int my_rank;
MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	cout << "Worker of process " << my_rank << " changing state to "
			<< state << endl;
#endif
	// ADUV: Passing the token
	if(s == NOJOB && token != UNDEFINED)
	{
		int dest = (my_rank+1)%p_num;
		
		if(token == WHITE)
			MPI_Send(NULL, 0, MPI_CHAR, dest, MSG_TOKEN_WHITE, MPI_COMM_WORLD);
		else
			MPI_Send(NULL, 0, MPI_CHAR, dest, MSG_TOKEN_BLACK, MPI_COMM_WORLD);
		
		color = WHITE;
		token = UNDEFINED;
	}
}

void Worker::acceptSolution(Solution *sol, int src)
{
#ifdef DEBUG
	if(my_rank == 0)
	{
		cout << "Received solution from process " << src << endl;
		cout << *sol;
	}
#endif
	if(my_rank == 0)
	{
		if(ps->getSolution().size() == 0 ||
			sol->getSolution().size() < ps->getSolution().size())
		{
			delete ps;
			ps = sol;
		}
		else
			delete sol;
	}
	// If we're not the solution collecting process, send it to them
	else
	{
		// Serialize
		string ser = sol->serialize();
		MPI_Send((void*)ser.c_str(), ser.size(), MPI_CHAR, 0,
				MSG_SOLUTION, MPI_COMM_WORLD);
	}
}

bool Worker::test(const boost::dynamic_bitset<>& bs)
{
	if(!pi)
		return false;
	
	Instance& i = *pi;
	
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
	
	// Found a solution
	Solution *sol = new Solution(i);
	sol->setSolution(bs, offs);
	acceptSolution(sol, my_rank);
	
	return true;
}

bool Worker::is_substring(const boost::dynamic_bitset<>& substr,
	const boost::dynamic_bitset<>& superstr, unsigned int offset) const
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

void Worker::broadcast_instance()
{
#ifdef DEBUG
	cout << "Process " << my_rank << " is broadcasting instance:" << endl;
	cout << *pi << endl;
#endif
	stringstream sbuf(stringstream::out);
	pi->serialize(sbuf);
	if(sbuf.str().size() > MAX_INST_SIZE)
		throw "Instance too large.";
	char buf[MAX_INST_SIZE];
	memset(buf, 0, MAX_INST_SIZE);
	memcpy(buf, sbuf.str().c_str(), sbuf.str().size());
	int res = MPI_Bcast(buf, MAX_INST_SIZE, 
				MPI_CHAR, my_rank, MPI_COMM_WORLD);
	
	if(res != MPI_SUCCESS)
		throw "Error broadcasting the instance.";
	
	s.push_back(new boost::dynamic_bitset<>());
	changeState(WORKING);
}

void Worker::receive_instance()
{
	char buf[MAX_INST_SIZE];
	memset(buf, 0, sizeof(buf));
	int res = MPI_Bcast(buf, MAX_INST_SIZE, MPI_CHAR, 
				0, MPI_COMM_WORLD);
	if(res != MPI_SUCCESS)
		throw "Error receiving instance broadcast.";
	stringstream sbuf(string(buf, MAX_INST_SIZE), stringstream::in);
	pi = new Instance(sbuf);
	
#ifdef DEBUG
	cout << "Process " << my_rank << " is receiving instance:" << endl;
	cout << *pi << endl;
#endif
	changeState(NOJOB);
}

void Worker::expand()
{
	boost::dynamic_bitset<> *bs0, *bs1;
	
	if(s.empty())
	{
		changeState(NOJOB);
		return;
	}
	// If the top bs reaches k, don't deal with it
	if(s.back()->size() >= pi->getK())
	{
		delete s.back();
		s.pop_back();
		return;
	}
	
	// Else, do our job
	bs0 = new boost::dynamic_bitset<>(*s.back());
	bs1 = new boost::dynamic_bitset<>(*s.back());
	
	delete s.back();
	s.pop_back();
	
	bs0->push_back(false);
	bs1->push_back(true);
	
	test(*bs0);
	test(*bs1);
	
	s.push_back(bs0);
	s.push_back(bs1);
}

void Worker::checkMessages(void)
{
	int flag = 0, count = 0;
	MPI_Status status;
	char buf[MAX_MSG_SIZE];
	
	MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, 
		   &flag, &status);
	if(flag)
	{
		MPI_Get_count(&status, MPI_CHAR, &count);
		MPI_Recv(buf, MAX_MSG_SIZE, MPI_CHAR, 
			 MPI_ANY_SOURCE, MPI_ANY_TAG,
			 MPI_COMM_WORLD, &status);
		// Ignore message from myself!
		if(status.MPI_SOURCE == my_rank)
			return;
#ifdef DEBUGMSG
		cout << "MSG [Size: " << count << "] [Tag: "<<status.MPI_TAG<<"] [" 
				<< status.MPI_SOURCE << "->" << my_rank << "]" << endl;
		for (int i = 0; i < count; ++i)
			cout.write(&buf[i], 1);
		cout << endl;
#endif
		switch(status.MPI_TAG)
		{
		case MSG_SOLUTION:
		{
			stringstream sstream(string(buf, count));
			acceptSolution(new Solution(*pi, sstream), status.MPI_SOURCE);
			break;
		}
		case MSG_REQUEST:
		{
			// Request for work, let's send response
#ifdef DEBUG
			cout << "Work request received: [" << status.MPI_SOURCE 
					<< "->" << my_rank << "]" << endl;
#endif
			// Do we have work?
			if(s.size() < 2)
			{
				// We don't.
				MPI_Send(NULL, 0, MPI_CHAR, status.MPI_SOURCE, 
							MSG_NOWORK, MPI_COMM_WORLD);
			}
			else
			{
				// We do
				boost::dynamic_bitset<> *wts; // Work to send
				wts = s.front();
				s.pop_front();
				
				stringstream sstream;
				sstream << *wts;
				delete wts;
				
				const string& sts = sstream.str();
				char buf[MAX_MSG_SIZE];
				memcpy(buf, sts.c_str(), sts.size());
				MPI_Send(buf, sts.size(), MPI_CHAR, status.MPI_SOURCE,
							MSG_WORK, MPI_COMM_WORLD);
				
				// ADUV: If donor pi sends work to pj and i>j, 
				//	donor becomes black
				if(my_rank > status.MPI_SOURCE)
					color = BLACK;
			}
			break;
		}
		case MSG_NOWORK:
		{
			changeState(NOJOB);
			break;
		}
		case MSG_WORK:
		{
			boost::dynamic_bitset<>*rbs = new boost::dynamic_bitset<>();
			char c;
			int j;
			stringstream is(string(buf, count), stringstream::in);
			
			rbs->resize(count);
			for(j=0;j<count;++j)
			{
				is.read((char*)&c, 1);
				if(c == '0') (*rbs)[count-j-1] = false;
				else if(c == '1') (*rbs)[count-j-1] = true;
			}
#ifdef DEBUG
			cout << "Received work: [" << status.MPI_SOURCE 
				<< "->" << my_rank << "] [" << *rbs << "]" << endl;
#endif
			s.push_front(rbs);
			changeState(WORKING);
			break;
		}
		case MSG_TOKEN_BLACK: // ADUV
		{
			token = BLACK;
#ifdef DEBUG
			cout << "Received token: [" << status.MPI_SOURCE 
				<< "->" << my_rank << "] [BLACK]" << endl;
#endif
			if(my_rank == 0)
			{
				// Start with new white token
				token = WHITE;
			}
			break;
		}
		case MSG_TOKEN_WHITE: // ADUV
		{
			token = WHITE;
#ifdef DEBUG
			cout << "Received token: [" << status.MPI_SOURCE 
				<< "->" << my_rank << "] [WHITE]" << endl;
#endif
			if(color == BLACK)
				token = BLACK;
			
			if(my_rank == 0)
			{
				MPI_Send(NULL, 0, MPI_CHAR, 1, MSG_FINISH, MPI_COMM_WORLD);
				changeState(FINISHED);
			}
			
			break;
		}
		case MSG_FINISH:
		{
			int dest = (my_rank+1)%p_num;
			if(dest != 0)
			{
				MPI_Send(NULL, 0, MPI_CHAR, dest, MSG_FINISH, MPI_COMM_WORLD);
			}
			changeState(FINISHED);
		}
		}
	}
}

void Worker::request_work()
{
	int nd = nextDonor();
	MPI_Send(NULL, 0, MPI_CHAR, nd, MSG_REQUEST, MPI_COMM_WORLD);
	changeState(WAITING_FOR_JOB);
}

int Worker::nextDonor()
{
	int r = _nextDonor;
	_nextDonor = (_nextDonor+1)%p_num;
	// We cannot ask ourselves for job!
	if(_nextDonor == my_rank)
		_nextDonor = (_nextDonor+1)%p_num;
	return r;
}

void Worker::work()
{
	// c = continue
	bool c = true;
	int i = 0;
	
	while(c)
	{
		if(i++ % MSG_FREQ == 0)
			checkMessages();
		
		switch(state)
		{
		case BROADCASTING_INSTANCE:
			broadcast_instance();
			break;
		case WAITING_FOR_INSTANCE:
			receive_instance();
			break;
		case WORKING:
			expand();
			break;
		case NOJOB:
			request_work();
			break;
		case WAITING_FOR_JOB:
			MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, NULL);
			i = 0;
			continue;
		case FINISHED:
		default:
			c = false;
			break;
		}
	}
}

