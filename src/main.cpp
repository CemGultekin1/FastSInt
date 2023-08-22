#include <iostream>
#include <stdexcept>
#include "midpoint.h"
#include "ntree.h"
#include <cmath>
#include "ioopers.h"

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


int ntree_indexer_test(){
    int_type depth = 3;
    int_type width = 2;
    NTreeIndexer* tc = new NTreeIndexer(depth,width);
    NTreeIndex* tcrd = tc->next();
    int i = 0;
    while(tcrd != nullptr){
        tcrd = tc->next();
        i += 1;
    }
    return 0;
}



void write_to_file(){
    std::string rootdir = "data/binaries";
    BinaryBuffer bb(rootdir);
    BinarySegmentWriter* bsw = bb.new_segment();

    // char b = 'b';
    for(int i = 0; i < 3; i++){
        bsw->write((char*) &i, sizeof(i));
    }
    delete bsw;


    bsw = bb.new_segment();
    // char a = 'a';
    for(int i = 3; i < 10; i++){
        bsw->write((char*) &i, sizeof(i));
    }
    delete bsw;

    bb.print();
    bb.to_file();
}

void read_from_file(){
    std::string rootdir = "data/binaries";
    BinaryBuffer bb(rootdir);
    bb.from_file();
    std::printf("num segments = %d\n",(int) bb.num_segments());
    BinarySegmentReader* bsr = bb.read_segment(1);
    int* i;
    while((i = (int*) bsr->next(sizeof(int))) != nullptr){
        std::cout << *i <<", ";
    }
    std::cout<<std::endl;

    // bsr = bb.read_segment(1);
    // while((i = (int*) bsr->next(sizeof(int))) != nullptr){
    //     std::cout << *i <<", ";
    // }
    // std::cout<<std::endl;
}

int main(int argc, char** argv){
    std::string arg = argv[1];
    if (arg == "--write"){    
        write_to_file();
    }else if (arg == "--read"){
        read_from_file();
    }else{
        std::printf("argument %s not understood\n", arg.c_str());
    }
}