#include <iostream>
#include <stdexcept>
#include "midpoint.h"
#include "iotree.h"
#include <cmath>

class Simplex{
    Simplex* mother;
    Simplex* children;
    Midpoint* midpoint;
};

int midpoint_test(){
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


int main(){
    int_type depth = 3;
    int_type width = 2;
    TreeCounter* tc = new TreeCounter(depth,width);
    TreeCoordinate* tcrd = tc->next();
    int i = 0;
    while(tcrd != nullptr){/*} && i < 100){*/
        tcrd = tc->next();
        i += 1;
    }
}