#include <iostream>
#include <stdexcept>
#include "midpoint.h"
#include "dag.h"
#include <cmath>
#include "ioopers.h"

class Simplex{
    Simplex* parent;
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
    std::string path = "data/dag_binaries";
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
    std::string path = "data/dag_binaries";
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


void dag_write(){
    DAG dag;
    GraphEdgeType* edge = dag._edges[0];
    GraphEdgeType* edge1 = dag.branch_from_edge(edge, 3);
    GraphEdgeType* edge2 = dag.branch_from_edge(edge1,4);
    GraphEdgeType* edge3 =dag.branch_from_edge(edge2,5);


    dag.branch_from_edge(edge3,2);
    dag.to_file("data/dag_binaries");
    dag.print();
}

void dag_read(){
    int_type width = 5;
    DAG dag(width);
    dag.from_file("data/dag_binaries");
    dag.print();
    std::cout << "-----------------------" << std::endl;
    GraphEdgeType* edge = dag._edges[14];
    dag.branch_from_edge(edge, 7);
    dag.print();
}


int main(int argc, char** argv){
    std::string arg = argv[1];
    if (arg == "--write_dag"){    
        dag_write();
    }else if (arg == "--read_dag"){
        dag_read();
    }else if (arg == "--write"){    
        write_to_file();
    }else if (arg == "--read"){
        read_from_file();
    }else{
        std::printf("argument %s not understood\n", arg.c_str());
    }
}