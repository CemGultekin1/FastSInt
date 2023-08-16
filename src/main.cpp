#include <iostream>
#include <stdexcept>



typedef long long int_type;
typedef float    float_type; 



const int_type  EOC = -999;
const int_type  NLLC = -1;



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
};

Midpoint::Midpoint(float_type* &v,int_type* &c,int_type & len): _weights(v),_coords(c),_length(len){}
Midpoint::Midpoint(int_type & len):_length(len){
    _weights = (float_type*) malloc(sizeof(float_type)*_length);
    _coords = (int_type*) malloc(sizeof(int_type)*_length);
}
Midpoint::Midpoint(){
    _weights = nullptr;
    _coords = nullptr;
    _length = 0;
}
Midpoint::~Midpoint(){
    delete [] _weights;
    delete [] _coords;
}

void Midpoint::print(){
    for(int_type i =0 ; i < _length; i++){
        std::cout <<"\t" << _coords[i] << ":\t" << _weights[i] << std::endl;
    }
}
void Midpoint::set(int_type &i, float_type& w, int_type& c){
    if(i >= _length){
        throw std::out_of_range("out of range");
    }
    _weights[i] = w;
    _coords[i] = c;
}




class IncondenseMidpoint:public Midpoint{
    public:
        int_type _last;
        bool _incondense;
        IncondenseMidpoint( float_type* &weights,int_type* &coords, int_type& last, int_type& length);
        void add(float_type& w, int_type& c);
        int_type get_condensed_length() const;
        Midpoint* condensate() const;
};
void IncondenseMidpoint::add(float_type& w, int_type& c){
    Midpoint::set(_last,w,c);
    _last++;
    _incondense = true;
}
IncondenseMidpoint::IncondenseMidpoint( float_type* &weights,int_type* &coords, int_type& last, int_type& length){
    _length = length;
    _last = last;

    if (last == length){        
        _weights = weights;
        _coords = coords;        
        _incondense = false;
    }else{
        _incondense = true;
        _weights = (float_type*) malloc(sizeof(float_type)*length);
        _coords = (int_type*) malloc(sizeof(int_type)*length);
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
    }    
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
Midpoint* IncondenseMidpoint::condensate() const{
    int_type len = get_condensed_length();
    std::cout << " get_condensed_length() = " << len << std::endl;
    Midpoint* midp = new Midpoint(len);
    int_type j =0;
    int_type* c = _coords;
    float_type* w = _weights;
    for(int_type i = 0; i< _length; i++,c++,w++){
        if(*c == NLLC ){
            continue;
        }
        if(*c == EOC){
            break;
        }
        midp->set(j,*w,*c);
        j++;
    }
    return midp;
}



class Simplex{
    int mother;
    int* children;    
};

int main(){
    int_type len = 16 + 1;
    int_type last = 12;
    float_type* weights = (float_type*) malloc(sizeof(float_type)*len);
    int_type* coords = (int_type*) malloc(sizeof(int_type)*len);
    for(int_type i = 0; i < len; i++){
        weights[i] = 0.1;
        coords[i] = i;
    }
    coords[2] = -1;
    coords[4] = -1;
    coords[len - 1] = EOC;
    std::cout << "coords = " << coords << std::endl;
    IncondenseMidpoint* inmidp = new IncondenseMidpoint(weights,coords,last,len);
    std::cout << "inmidp->_coords = " << inmidp->_coords << std::endl;
    std::cout << "inmidp->_last = " << inmidp->_last << std::endl;
    std::cout << "inmidp->_length = " << inmidp->_length << std::endl;
    inmidp->print();    
    
    std::cout << std::endl;

    Midpoint* midp = inmidp->condensate();

    delete inmidp;
    midp->print();
    delete midp;

    return 0;
}