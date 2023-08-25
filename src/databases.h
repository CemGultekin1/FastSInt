#pragma once
#include "constants.h"
#include <vector>
#include "random.h"

/*
class AbstractDataBase{
    public:
        virtual float_type* operator[](int_type) = 0;
        virtual int_type get_dimension() = 0;
        virtual int_type get_length() = 0;
};
*/

class DataPoint{
    public:
        int_type _data_id;
        float_type* _weights;
        int_type _dim;
        DataPoint(int_type dim);
        ~DataPoint();
        void print();
};


template <int_type dim,int_type length>
class RandomDataBase{
    public:        
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
};




DataPoint* allocate_random_dp(int_type dim,random_vector_generator& rvg);


#include "databaseinterface.tpp"