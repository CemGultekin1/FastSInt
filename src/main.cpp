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





void write_to_file(){
    std::string path = "data/tree_binaries";
    BinaryBuffer bb;
    
    BinarySegmentWriter* bsw;
    for(int j = 0; j < 5; j ++){
        bsw = bb.new_segment();
        for(int i = 0; i < j; i++){
            bsw->write((char*) &i, sizeof(i));
        }
        delete bsw;
    }

    bb.print();
    bb.to_file(path);
}

void read_from_file(){
    std::string path = "data/tree_binaries";
    BinaryBuffer* bb = new BinaryBuffer();
    bb->from_file(path);
    std::printf("num segments = %d\n",(int) bb->num_segments());
    BinarySegmentReader* bsr;
    for(int j = 0; j < 5; j ++){
        bsr = bb->read_segment(j);
        int* i;
        while((i = (int*) bsr->next(sizeof(int))) != nullptr){
            std::cout << *i <<", ";
        }
        std::cout<<std::endl;
        delete bsr;
    }
    delete bb;
}


void tree_write(){
    NTree ntree;
    NodeType* node = ntree._nodes[0];
    NodeType* node1 = ntree.branch_from_node(node, 3);
    NodeType* node2 = ntree.branch_from_node(node1,4);
    NodeType* node3 =ntree.branch_from_node(node2,5);


    ntree.branch_from_node(node3,2);
    ntree.to_file("data/tree_binaries");
    ntree.print();
}

void tree_read(){
    int_type nbranch = 5;
    NTree ntree(nbranch);
    ntree.from_file("data/tree_binaries");
    ntree.print();
    std::cout << "-----------------------" << std::endl;
    NodeType* node = ntree._nodes[14];
    ntree.branch_from_node(node, 7);
    ntree.print();
}


int main(int argc, char** argv){
    std::string arg = argv[1];
    if (arg == "--write_tree"){    
        tree_write();
    }else if (arg == "--read_tree"){
        tree_read();
    }else if (arg == "--write"){    
        write_to_file();
    }else if (arg == "--read"){
        read_from_file();
    }else{
        std::printf("argument %s not understood\n", arg.c_str());
    }
}