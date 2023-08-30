#pragma once
#include "constants.h"
#include <vector>
#include "random.h"
#include "midpoint.h"

class DataPoint{
    public:
        int_type _data_id;
        float_type* _weights;
        int_type _dim;
        int_type _node_id;
        DataPoint(int_type dim);
        ~DataPoint();
        void print();
        void smultip(float_type sc0,float_type sc1, DataPoint *dp);
};


// template <const int_type& dim,const int_type& length>
class RandomDataBase{
    public:        
        int_type _dim;
        int_type _length;
        RandomDataBase(int_type dim, int_type length): _dim(dim),_length(length){};
        DataPoint* operator[](int_type);
        
};




template <class DB>
class DataBaseInterface{
    DB* _abstr_db;
    std::vector<int_type> _expressive_data_id_subset;
    int_type _counter;
    int_type _dim;
    int_type _length;
    public:
        DataBaseInterface(DB* adb);
        DataPoint* next();
        void reset_counter();
        void init_virtual_nodes();
        int_type add_expressive_node(int_type data_id);
        DataPoint* midpoint2data(Midpoint* mipd);
        float_type midpoint_accuracy(Midpoint* midp);
        DataPoint* operator[](int_type i);
        DataPoint* virtual_node(int_type i);
};




DataPoint* allocate_random_dp(int_type dim,random_vector_generator& rvg);


#include "databases.tpp"