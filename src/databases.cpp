#include "databases.h"
#include "random.h"
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

