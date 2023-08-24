#include "random.h"
#include <cstdlib>
#include <ctime>

float_type random_float(){
    return static_cast <float_type> (rand()) / static_cast <float_type> (RAND_MAX);
}

random_vector_generator::random_vector_generator(unsigned seed){
    srand (static_cast <unsigned> (seed));
}

void random_vector_generator::operator()(float_type* data,int_type len){
    for(float_type* dat = data; (dat - data) < len; dat ++){
        *dat = random_float();
    }
}

