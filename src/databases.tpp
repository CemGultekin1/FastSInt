#pragma once
#include <iostream>
#include <cmath>
#include "midpoint.h"
#include "databases.h"

template <class DB>
DataBaseInterface<DB>::DataBaseInterface(DB* adb){
    _abstr_db = adb;
    _expressive_data_id_subset = 
        std::vector<int_type>();    
    _length = 0;
    _counter = 0;
    _dim = NLLC;    
}

template <class DB>
void DataBaseInterface<DB>::init_virtual_nodes(){
    _expressive_data_id_subset.resize(_dim+1, NLLC);
}

template <class DB>
DataPoint* DataBaseInterface<DB>::next(){
    DataPoint* dp = (*_abstr_db)[_counter];
    if(dp == nullptr){
        return dp;
    }
    if(_dim == NLLC){
        _dim = dp->_dim;
        init_virtual_nodes();
    }
    dp->_data_id = _counter;
    // std::cout << "dp->_node_id = _expressive_data_id_subset.size();  " <<  _expressive_data_id_subset.size() << std::endl;
    dp->_node_id = _expressive_data_id_subset.size();
    if(dp != nullptr){
        _counter ++;
        if(_length < _counter){
            _length = _counter;
        }
    }
    return dp;
}


template <class DB>
void DataBaseInterface<DB>::reset_counter(){
    _counter = 0;
}

template <class DB>
int_type DataBaseInterface<DB>::add_expressive_node(int_type data_id){
    _expressive_data_id_subset.push_back(data_id);
    return  static_cast<int_type>(_expressive_data_id_subset.size());
}




template <class DB>
DataPoint* DataBaseInterface<DB>::operator[](int_type i){
    if(i <= _dim){
        return virtual_node(i);
    }else{
        return (*_abstr_db)[i - _dim -1];
    }
}
template <class DB>
DataPoint* DataBaseInterface<DB>::virtual_node(int_type i){
    if(i > _dim){
        return nullptr;
    }
    DataPoint* dp = new DataPoint(_dim);
    float_type* w = dp->_weights;
    for(float_type* curs = w; curs-w < _dim; curs ++ ){
        *curs = 0.;
    }
    if(i < _dim){
        w[i] = _dim + 1;
    }    
    
    return dp;
}

template <class DB>
DataPoint* DataBaseInterface<DB>::midpoint2data(Midpoint* midp){
    float_type* w = midp->_weights;
    DataPoint* dp = nullptr;
    DataPoint* dp1;
    float_type* crs;
    float_type* crs1;
    for(int_type* c= midp->_coords; *c !=EOC ; c++,w++){
        if(*c == NLLC){
            continue;
        }

        dp1 = (*this)[*c];

        if(dp == nullptr){
            dp = dp1;
            dp->smultip(*w,1.,nullptr);
        }else{
            dp->smultip(1.,*w,dp1);
            delete dp1;
        }
    }   
    return dp;
}


template <class DB>
float_type DataBaseInterface<DB>::midpoint_accuracy(Midpoint* midp){
    DataPoint* dp = midpoint2data(midp);
    float_type* w1 = dp->_weights;
    DataPoint* dp1 = (*_abstr_db)[midp->_data_id];
    float_type* w0 = dp1->_weights;
    float_type err = 0;
    float_type norm = 0;
    for(int_type i = 0; i < _dim; i ++, w0++ , w1++){
        err += pow((*w0) - (*w1),2);
        norm += pow((*w1),2);
    }
    delete dp;
    delete dp1;
    return 1. - err/norm;
}







