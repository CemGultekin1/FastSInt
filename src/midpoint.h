#pragma once
#include "constants.h"
#include "random.h"
#include "ioopers.h"



void random_data_point(float_type* data,int_type len,random_vector_generator& rng);

class Midpoint{        
    public:
        int_type _node_id;
        int_type _midpoint_id;
        int_type _data_id;
        float_type* _weights;
        int_type* _coords;
        Midpoint(float_type* &v,int_type* &c,int_type& data_id,int_type& node_id,int_type& midpoint_id);
        Midpoint(int_type& len,int_type& data_id,int_type& node_id,int_type& midpoint_id);
        Midpoint();
        ~Midpoint();
        void print() const;
        void set(int_type &i, float_type& w, int_type& c);
        bool is_empty();
        void from_binary(BinarySegmentReader* nd);
        void to_binary(BinarySegmentWriter* bb) const;
        int_type get_length() const;
        float_type sum() const;

};

class IncondenseMidpoint:public Midpoint{
    public:
        int_type _length;
        int_type _last;
        bool _incondense;
        IncondenseMidpoint(float_type* &weights,int_type* &coords, int_type last, int_type length,
                int_type data_id = NLLC,int_type node_id = NLLC,int_type midpoint_id = NLLC);
        IncondenseMidpoint(float_type* &weights,int_type dim,int_type depth,
                        int_type data_id= NLLC,int_type node_id= NLLC,int_type midpoint_id= NLLC);
        void concatenate(float_type& w, int_type& c);
        int_type get_condensed_length() const;
        Midpoint* condensate();
        void last_coord_adjustment();
        void print() const;
};


