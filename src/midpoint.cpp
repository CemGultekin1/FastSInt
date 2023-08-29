#include "midpoint.h"
#include <stdexcept>
#include <iostream>
#include <iomanip>


void random_data_point(float_type* data,int_type dim,random_vector_generator& rng){
    // _random_vector_generate random_vector_generator(false);
    return rng(data,dim);
}


Midpoint::Midpoint(float_type* &v,int_type* &c,int_type& data_id,int_type& node_id,int_type& midpoint_id):
             _weights(v),_coords(c),_node_id(node_id),_data_id(data_id),_midpoint_id(midpoint_id){}
Midpoint::Midpoint(int_type & len,int_type& data_id,int_type& node_id,int_type& midpoint_id):
            _node_id(node_id),_data_id(data_id),_midpoint_id(midpoint_id){//:_length(len){
    // _weights = (float_type*) malloc(sizeof(float_type)*_length);
    // _coords = (int_type*) malloc(sizeof(int_type)*_length);
    _weights = new float_type[len];
    _coords = new int_type[len];
}

Midpoint::Midpoint():_node_id(NLLC),_data_id(NLLC),_midpoint_id(NLLC){
    _weights = nullptr;
    _coords = nullptr;
}
bool Midpoint::is_empty(){
    return _weights != nullptr;
}
Midpoint::~Midpoint(){
    if(_weights != nullptr){
        delete [] _weights;
        delete [] _coords;
    }   
    
}

void Midpoint::print() const{
    float_type* w = _weights;
    // int width = 8;
    std::cout.precision(3);
    for(int_type* c = _coords; *c != EOC; c++,w++){
        std::cout << std::setw(4) << std::fixed <<*c  << std::setw(12) << std::scientific << *w << std::endl;
    }
    float_type s = sum();
    std::cout << std::setw(4) << "sum:   " << std::scientific << std::setw(16) << s << std::endl;
}
float_type Midpoint::sum() const{
    float_type* w = _weights;
    float_type s = 0;
    for(int_type* c = _coords; *c != EOC; c++,w++){
        if(*c!= NLLC){
            s += *w;
        }
    }
    return s;
}

void IncondenseMidpoint::print() const{
    Midpoint::print();
    std::cout << std::fixed;
    std::cout << std::setw(4) << "length: " << std::setw(16) << _length <<std:: endl;
    std::cout << std::setw(4) << "last:   " << std::setw(16) << _last <<std:: endl;

}

void Midpoint::set(int_type &i, float_type& w, int_type& c){
    _weights[i] = w;
    _coords[i] = c;
}


int_type Midpoint::get_length() const{
    int_type i = 0;
    int_type* c = _coords;
    while(*c != EOC){
        c++;
        i++;
    }
    return i;
}




void IncondenseMidpoint::concatenate(float_type& w, int_type& c){
    Midpoint::set(_last,w,c);
    _last++;
    _incondense = true;
}
IncondenseMidpoint::IncondenseMidpoint( float_type* &weights,int_type* &coords,
                            int_type last, int_type length,
                            int_type data_id,int_type node_id,int_type midpoint_id){
    /*

    */
    _length = length;
    _last = last;
    
    _node_id = node_id;
    _data_id = data_id;
    _midpoint_id = midpoint_id;

    _incondense = true;
    // _weights = (float_type*) malloc(sizeof(float_type)*length);
    // _coords = (int_type*) malloc(sizeof(int_type)*length);
    _weights = new float_type[length];
    _coords = new int_type[length];
    float_type* w = weights;
    int_type* c = coords;
    
    for(int_type i = 0; i < last; i++,c++,w++){
        _weights[i] = *w;
        _coords[i] = *c;
    }
    for(int_type i = last; i < length; i++){
        _weights[i] = 0.;
        _coords[i] = EOC;
    }
    int_type* c0 = _coords;
    float_type* w0 = _weights;
    float_type sum = 0;
    float_type invf_last = ((float_type) 1)/((float_type) _last);
    while(*c0 != EOC){
        sum += *w0;
        *w0 = (*w0)*invf_last;
        w0++;
        c0++;
    }
    sum = sum/invf_last;

    w0--;
    *w0 = 1;

}


IncondenseMidpoint::IncondenseMidpoint( float_type* &weights,
            int_type dim,int_type depth,int_type data_id,int_type node_id,int_type midpoint_id){
    /*

    */
    _length = dim+1 + depth+ 1;
    _last = dim+1;
    _node_id = node_id;
    _data_id = data_id;
    _midpoint_id = midpoint_id;

    _incondense = true;
    _weights = new float_type[_length];
    _coords = new int_type[_length];
    float_type* w = weights;
    
    for(int_type i = 0; i < dim; i++,w++){
        _weights[i] = *w;
        _coords[i] = i;
    }

    for(int_type i = dim; i < _length; i++){
        _weights[i] = 0.;
        _coords[i] = EOC;
    }
    int_type* c0 = _coords;
    float_type* w0 = _weights;
    float_type sum = 0;
    float_type invf_last = ((float_type) 1)/((float_type) (dim+1));
    while(*c0 != EOC){
        sum += *w0;
        *w0 = (*w0)*invf_last;
        w0++;
        c0++;
    }
    *w0 = 1. - sum*invf_last;
    *c0 = dim;

}



int_type IncondenseMidpoint::get_condensed_length() const{            
    int_type len = 0;
    for (int_type* i = _coords; *i != EOC; i++){
        if (*i != NLLC){
            len++;
        }
    }
    return len;
}


void IncondenseMidpoint::last_coord_adjustment(){
    int_type* c1 = _coords + (_last - 1);
    int_type* c0 = _coords;
    
    float_type* w1 = _weights + (_last - 1);
    float_type* w0 = _weights;
    
    bool place_found = false;
    while(c0 < c1){
        while(*c0==NLLC && c0 < c1){
            c0++;
        }
        if(c0 == c1){
            break;
        }
        if(*c0 > *c1){
            place_found = true;
            break;
        }
        c0++;
    }
    if(!place_found){
        return;
    }
    w0 = (c0 - _coords)+ w0;
    std::rotate(c0,c1,c1+1);
    std::rotate(w0,w1,w1+1);
}


Midpoint* IncondenseMidpoint::condensate(){
    int_type len = get_condensed_length() + 1;
    Midpoint* midp = new Midpoint(len,_data_id,_node_id,_midpoint_id);
    int_type j =0;
    int_type* c = _coords;
    float_type* w = _weights;
    for(int_type i = 0; i< _length; i++,c++,w++){
        if(*c == NLLC ){
            continue;
        }
        if(*c == EOC){
            midp->set(j,*w,*c); // the last element is EOC
            break;
        }
        midp->set(j,*w,*c);
        j++;
    }    
    return midp;
}



void Midpoint::from_binary(BinarySegmentReader* nd){
    if(nd->get_size() == 0){
        return;
    }
    int_type* num_coords = (int_type*) nd->next(sizeof(int_type));    
    int_type* int_ptr;
    
    int_ptr =  (int_type*) nd->next(sizeof(int_type));    
    _data_id = *int_ptr;
    int_ptr =  (int_type*) nd->next(sizeof(int_type));    
    _node_id = *int_ptr;
    int_ptr =  (int_type*) nd->next(sizeof(int_type));   
    _midpoint_id = *int_ptr; 

    delete int_ptr;
    int_type* _coords = (int_type*) nd->next(sizeof(int_type)*(*num_coords));
    float_type* _weights = (float_type*) nd->next(sizeof(float_type)*(*num_coords));
    delete num_coords;
}

void Midpoint::to_binary(BinarySegmentWriter* bb) const{
    if(_weights == nullptr){
        return;
    }
    int_type x= get_length();
    bb->write((char*) &x,sizeof(x));
    bb->write((char*) &_data_id,sizeof(_data_id));
    bb->write((char*) &_node_id,sizeof(_node_id));
    bb->write((char*) &_midpoint_id,sizeof(_midpoint_id));

    
    bb->write((char*) &_coords,sizeof(int_type)*x);
    bb->write((char*) &_weights,sizeof(float_type)*x);

}