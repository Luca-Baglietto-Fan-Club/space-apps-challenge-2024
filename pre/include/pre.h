#ifndef __SAC24_PRE_H
#define __SAC24_PRE_H

#include <defs.h>

#include <vector>

typedef struct {
    float time;
    ld velocity;
} data_point_t;

void parse12(std::vector<data_point_t> &out);

#endif /* __SAC24_PRE_H */