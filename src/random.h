#pragma once
#include "constants.h"
#include <cstdlib>
#include <ctime>

float_type random_float();

class random_vector_generator{    
    public:
        random_vector_generator(unsigned seed);
        void operator()(float_type* data,int_type len);
    private:
        // bool _init_once;
        int seed;
};