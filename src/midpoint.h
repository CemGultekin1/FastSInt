#include "constants.h"


class Midpoint{
    public:
        float_type* _weights;
        int_type* _coords;
        int_type _length;
        Midpoint(float_type* &v,int_type* &c, int_type& len);
        Midpoint(int_type& len);
        Midpoint();
        ~Midpoint();
        void print();
        void set(int_type &i, float_type& w, int_type& c);
        bool is_empty();
};


class IncondenseMidpoint:public Midpoint{
    public:
        int_type _last;
        bool _incondense;
        IncondenseMidpoint( float_type* &weights,int_type* &coords, int_type& last, int_type& length);
        void add(float_type& w, int_type& c);
        int_type get_condensed_length() const;
        Midpoint* condensate() const;
};