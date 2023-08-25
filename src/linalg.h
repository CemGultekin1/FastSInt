#include "constants.h"

void scaled_vector_sum(float_type sc, float_type* vec0,int_type len, float_type*vec1){
    for(int_type i = 0; i < len; vec0++, vec1++){
        *vec1 = (*vec0)*sc +  (*vec1);
    }
}