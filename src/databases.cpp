#include "databases.h"
#include "random.h"
#include "linalg.h"
#include <iostream>
#include <iomanip>


DataPoint* allocate_random_dp(int_type dim,random_vector_generator& rvg){
    DataPoint* dp = new DataPoint(dim);
    rvg(dp->_weights,dim);
    dp->_data_id = NLLC;
    return dp;
}
DataPoint::DataPoint(int_type dim){
    _weights = new float_type[dim];
    _dim = dim;
    _data_id = NLLC;
    _node_id = NLLC;
}
void DataPoint::print(){
    float_type* crs = _weights;
    std::cout.precision(3);
    std::cout <<  std::setw(4) << std::fixed << "data_id:"<<std::setw(4) << _data_id << std::endl;
    std::cout <<  std::setw(4) << std::fixed << "node_id:"<<std::setw(4) << _node_id << std::endl;
    for(;crs - _weights < _dim; crs++){
        std::cout << "\t" << std::setw(4) << std::scientific << *crs <<std::setw(4) << std::endl;
    }
    std::cout << std::endl;
}
DataPoint::~DataPoint(){
    if(_weights != nullptr) {
        delete [] _weights;
    }
}


void DataPoint::smultip(float_type sc0,float_type sc1, DataPoint *dp){
    if(dp != nullptr){
        float_type* w = _weights;
        float_type* w1 = dp->_weights;
        for(int_type i= 0; i < _dim; i++,w++,w1++){
            *w = (*w)*sc0 + (*w1)*sc1; 
        }
    }else if(sc0 == 1.){
        return;
    }else{
        float_type* w = _weights;
        for(int_type i= 0; i < _dim; i++,w++){
            *w = (*w)*sc0; 
        }
    }
    
}

