#include <iostream>
#include <stdexcept>
#include <cmath>
#include <iomanip>

#include "midpoint.h"
#include "dag.h"
#include "ioopers.h"
#include "constants.h"
#include "midptransform.h"
#include "databases.h"

class Simplex{
    Simplex* parent;
    Simplex* children;
    Midpoint* midpoint;
};





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


void rotate_demo(){
    float_type* x = new float_type[10];
    for(float_type* c = x; c - x < 10; c ++){
        *c = c-x;
        std::cout << *c << std::endl;
    }
    std::cout << std::endl;
    std::cout << "std::rotate(x," << *(x+9) << ", x+10,);" <<std::endl;
    std::rotate(x, x+9,x+10);
    for(float_type* c = x; c - x < 10; c ++){
        std::cout << *c << std::endl;
    }
    delete[] x;
}


void midpoint_demo(){
    const int_type dim = 10;    
    const int_type length = 5;
    int_type depth = 4;
    int_type node_id = dim+2;
    // _RandomDataBase rdb(dim,length);

    typedef RandomDataBase<dim,length> myrdb;
    myrdb rdb;
    DataBaseInterface<myrdb> dbi(&rdb);
    DataPoint* dp1 = dbi.next();
    IncondenseMidpoint* imidp1 = new IncondenseMidpoint(dp1->_weights,dp1->_dim,dp1->_node_id,depth);
    Midpoint* midp = imidp1->condensate();
    int_type new_node_id = dbi.add_expressive_node(dp1->_data_id);
    // midp->_node_id = new_node_id;
    float_type acc = dbi.midpoint_accuracy(midp,0);
    std::cout << "first midpoint accuracy = " << acc << std::endl;
    DataPoint* dp1_ = dbi.midpoint2data(midp);
    dp1_->print();
    dp1->print();
    delete dp1_;
    // delete dp1;


    


    DataPoint* dp2 = dbi.next();
    IncondenseMidpoint* imidp2 = new IncondenseMidpoint(dp2->_weights,dp2->_dim,depth,dp2->_node_id);
    


    // imidp2->print();
    MidpointTransform mt(midp,imidp2);
    int_type ei = mt.exit_index();
    imidp2 = mt.run_transform();
    Midpoint* midp2 = imidp2->condensate();
    std::cout  << "exit_index = " << ei << std::endl;
    // imidp2->print();
    
    acc = dbi.midpoint_accuracy(midp2,1);
    // DataPoint* dp2_ = dbi.midpoint2data(imidp2);
    // dp2_->print();
    // dp2->print();
    // delete dp2_;

    std::cout.precision(8);
    std::cout << std::scientific;
    std::cout  << "accuracy = " << acc << std::endl;

    delete dp1;
    delete dp2;
    delete midp;
    delete midp2;  
    delete imidp1;
    delete imidp2;      
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
    }else if (arg == "--midpoint"){
        midpoint_demo();
    }else{
        std::printf("argument %s not understood\n", arg.c_str());
    }
}