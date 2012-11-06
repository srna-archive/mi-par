#ifndef COMMON_H_
#define COMMON_H_

#include <cstdlib>

int RAND(int min, int max);

enum Tag {INSTANCE = 1001, JOB, PRIORITY, NOJOB};
const size_t MAX_INST_SIZE = 1024;
const size_t MAX_MSG_SIZE = 1024;

#endif /* COMMON_H_ */
