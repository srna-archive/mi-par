#ifndef WORKER_H_
#define WORKER_H_

#include "common.h"
#include "Solution.h"
#include "mpi.h"
#include <list>
#include <sstream>


#define MSG_FREQ		100

#define MSG_SOLUTION	1001
#define MSG_REQUEST		1002
#define MSG_WORK		1003
#define MSG_NOWORK		1004
#define MSG_TOKEN_WHITE	1005
#define MSG_TOKEN_BLACK	1006
#define MSG_FINISH		1007

class Worker
{
private:
	Solution *ps;
	Instance *pi;
	enum State {
		BROADCASTING_INSTANCE = 1,
		WAITING_FOR_INSTANCE,
		NOJOB,
		WAITING_FOR_JOB,
		WORKING,
		FINISHED
	};
	enum Color {UNDEFINED = 0, WHITE = 1, BLACK = 2, FINISH = 3};
	friend std::ostream& operator<< (std::ostream& os, const State& s)
	{
		char _State[][50] = {"BROADCASTING_INSTANCE",
			"WAITING_FOR_INSTANCE", "NOJOB", "WAITING_FOR_JOB", 
			"WORKING", "FINISHED"};
		os << _State[s-1];
		return os;
	}
	State state;
	Color color;
	Color token;
	int p_num, my_rank;
	int _nextDonor;
	std::list<boost::dynamic_bitset<>*> s;
	void changeState(State s);
	void broadcast_instance();
	void receive_instance();
	void expand();
	void checkMessages(void);
	void request_work(void);
	void acceptSolution(Solution *sol, int src);
	int nextDonor(void);
public:
	Worker(Instance *i = NULL);
	~Worker() {delete ps; delete pi;}
	bool test(const boost::dynamic_bitset<>& bs);
	bool is_substring(const boost::dynamic_bitset<>& substr,
		const boost::dynamic_bitset<>& superstr, unsigned int offset) const;
	void work();
	Solution *getSolution() {return ps;}
};

#endif /* WORKER_H_ */
